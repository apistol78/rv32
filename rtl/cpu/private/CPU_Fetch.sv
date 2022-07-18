`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_Fetch #(
	parameter RESET_VECTOR,
	parameter ICACHE_SIZE,
	parameter ICACHE_REGISTERED
)(
	input				i_reset,
	input				i_clock,

	// Control
	input				i_jump,
	input [31:0]		i_jump_pc,

	// Interrupt
	input				i_irq_pending,		//!< Interrupt pending.
	input [31:0]		i_irq_pc,			//!< Interrupt handler vector.
	output bit			o_irq_dispatched,	//!< Interrupt dispatched signal.
	output bit [31:0]	o_irq_epc,			//!< Interrupt return vector.

	// Bus
	output				o_bus_request,
	input				i_bus_ready,
	output [31:0]		o_bus_address,
	input [31:0]		i_bus_rdata,

	// Output
	input				i_decode_busy,
	output fetch_data_t	o_data
);

	typedef enum bit [1:0]
	{
		WAIT_ICACHE,
		WAIT_JUMP,
		WAIT_IRQ
	} state_t;

	state_t state = WAIT_ICACHE;
	bit [31:0] pc = RESET_VECTOR;
	fetch_data_t data = 0;
	bit [31:0] starve = 0;

	// ICache
	wire [31:0] icache_rdata;
	wire icache_ready;
	bit icache_stall;

	generate if (ICACHE_REGISTERED != 0) begin

		CPU_ICache_Reg #(
			.SIZE(ICACHE_SIZE)
		) icache(
			.i_reset(i_reset),
			.i_clock(i_clock),

			// Input
			.i_input_pc(pc),

			// Output
			.o_rdata(icache_rdata),
			.o_ready(icache_ready),
			.i_stall(icache_stall),

			// Bus
			.o_bus_request(o_bus_request),
			.i_bus_ready(i_bus_ready),
			.o_bus_address(o_bus_address),
			.i_bus_rdata(i_bus_rdata)
		);

	end endgenerate

	generate if (ICACHE_REGISTERED == 0) begin

		CPU_ICache #(
			.SIZE(ICACHE_SIZE)
		) icache(
			.i_reset(i_reset),
			.i_clock(i_clock),

			// Input
			.i_input_pc(pc),

			// Output
			.o_rdata(icache_rdata),
			.o_ready(icache_ready),
			.i_stall(icache_stall),

			// Bus
			.o_bus_request(o_bus_request),
			.i_bus_ready(i_bus_ready),
			.o_bus_address(o_bus_address),
			.i_bus_rdata(i_bus_rdata)
		);

	end endgenerate

	// 
	`undef INSTRUCTION
	`define INSTRUCTION icache_rdata
	`include "private/generated/Instructions_decode.sv"

	assign o_data = data;

	always_comb begin
		icache_stall = i_decode_busy || !(state == 0);
	end

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			state <= WAIT_ICACHE;
			pc <= RESET_VECTOR;
			data <= 0;
		end
		else begin
			case (state)
				WAIT_ICACHE: begin
					// Jump to interrupt if interrupt are pending, only do
					// this in this state as we need to finish any pending
					// branches first.
					o_irq_dispatched <= i_irq_pending;
					if ({ o_irq_dispatched, i_irq_pending } == 2'b01) begin
						o_irq_epc <= pc;
						pc <= i_irq_pc;
					end
					else begin
						if (icache_ready) begin
							data.tag <= data.tag + 1;
							data.instruction <= icache_rdata;
							data.pc <= pc;

							if (is_JUMP || is_JUMP_CONDITIONAL /*|| is_MRET*/) begin
								// Branch instruction, need to wait
								// for an explicit "goto" signal before
								// we can continue feeding the pipeline.
								state <= WAIT_JUMP;
							end
							// else if (is_ECALL || is_WFI) begin
							// 	// Software interrupt, need to wait
							// 	// for IRQ signal before continue.
							// 	state <= WAIT_IRQ;
							// end
							//else begin
								// Move PC to next instruction, will
								// enable to icache to start loading
								// next instruction.
								pc <= pc + 4;
							//end
						end
`ifdef __VERILATOR__					
						else if (!icache_stall)
							starve <= starve + 1;
`endif
					end
				end

				WAIT_JUMP: begin
					// Wait for "goto" signal.
					if (i_jump) begin
						pc <= i_jump_pc;
						state <= WAIT_ICACHE;
					end				
				end

				// WAIT_IRQ: begin
				// 	// Wait for IRQ signal.
				// 	o_irq_dispatched <= i_irq_pending;
				// 	if ({ o_irq_dispatched, i_irq_pending } == 2'b01) begin
				// 		o_irq_epc <= pc + 4;
				// 		pc <= i_irq_pc;
				// 		state <= WAIT_ICACHE;
				// 	end					
				// end

				default:
					state <= WAIT_ICACHE;
			endcase
		end
	end

endmodule
