`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_Fetch(
	input wire i_reset,
	input wire i_clock,

	// Control
	input wire i_decode_busy,
	output reg o_valid,
	input wire i_jump,
	input wire [31:0] i_jump_pc,

	// Debug
	output reg [`TAG_SIZE] o_tag,

	// Interrupt
	input wire i_irq_pending,
	input wire [31:0] i_irq_pc,
	output reg o_irq_dispatched,
	output reg [31:0] o_irq_epc,

	// Bus
	output wire o_bus_request,
	input wire i_bus_ready,
	output wire [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,

	// Output
	output reg [31:0] o_instruction,
	output reg [31:0] o_pc
);

	reg [2:0] state;
	reg [31:0] pc;

	// ICache
	wire [31:0] icache_rdata;
	wire icache_ready;
	reg icache_stall;

	CPU_ICache icache(
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

	// 
	`undef INSTRUCTION
	`define INSTRUCTION icache_rdata
	`include "Instructions_decode.sv"

	initial begin
		state = 0;
		pc = 32'h00000000;
		o_valid = 0;
		o_tag = 0;
		o_instruction = 0;
		o_pc = 0;
	end

	always @(*) begin
		icache_stall = /*i_decode_busy ||*/ !(state == 0);
	end

	always @(posedge i_clock) begin
		o_valid <= 0;

		o_irq_dispatched <= 0;
		o_irq_epc <= 0;

		// Jump to interrupt if interrupt are pending.
		if (i_irq_pending) begin
			o_irq_dispatched <= 1;
			o_irq_epc <= pc;
			pc <= i_irq_pc;
			state <= 0;
		end
		else begin
			case (state)
				0: begin
					if (!i_decode_busy && icache_ready) begin
						// Output to pipeline instruction and PC.
						o_valid <= 1;
						o_tag <= o_tag + 1;
						o_instruction <= icache_rdata;
						o_pc <= pc;

						if (is_JUMP || is_JUMP_CONDITIONAL || is_MRET || is_WFI) begin
							// Branch instruction, need to wait
							// for an explicit "goto" signal before
							// we can continue feeding the pipeline.
							state <= 1;
						end
						else begin
							// Move PC to next instruction, will
							// enable to icache to start loading
							// next instruction.
							pc <= pc + 4;
						end
					end
				end

				1: begin
					// Wait for "goto" signal.
					if (i_jump) begin
						pc <= i_jump_pc;
						state <= 0;
					end				
				end
			endcase
		end
	end

endmodule
