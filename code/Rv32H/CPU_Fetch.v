`include "CPU_Defines.v"

module CPU_Fetch(
	input wire i_reset,
	input wire i_clock,
	input wire i_stall,

	// Bus
	output wire o_bus_request,
	input wire i_bus_ready,
	output wire [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,

	// Input
	input wire [`TAG_SIZE] i_tag,
	input wire [31:0] i_pc_next,

	// Output
	output reg [`TAG_SIZE] o_tag,
	output reg [31:0] o_instruction,
	output reg [31:0] o_pc
);

	reg [2:0] state;
	reg [31:0] pc;
	
	reg [`TAG_SIZE] icache_input_tag;
	wire [`TAG_SIZE] icache_output_tag;
	wire [31:0] icache_rdata;

	CPU_ICache icache(
		.i_reset(i_reset),
		.i_clock(i_clock),

		.i_input_tag(icache_input_tag),
		.o_output_tag(icache_output_tag),
		.i_address(pc),
		.o_rdata(icache_rdata),

		.o_bus_request(o_bus_request),
		.i_bus_ready(i_bus_ready),
		.o_bus_address(o_bus_address),
		.i_bus_rdata(i_bus_rdata)
	);

	// 
	`define INSTRUCTION icache_rdata
	`include "Instructions_decode.v"

	initial begin
		state <= 0;
		pc <= 0;
		icache_input_tag <= 0;
		o_tag <= 0;
		o_instruction <= 0;
		o_pc <= 0;
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			state <= 0;
			pc <= 0;
			icache_input_tag <= 0;
			o_tag <= 0;
			o_instruction <= 0;
			o_pc <= 0;
		end
		else begin
			case (state)
				0: begin
					if (!i_stall) begin
						icache_input_tag <= icache_input_tag + 1;
						state <= 1;
					end
				end

				1: begin
					if (icache_output_tag == icache_input_tag) begin

						o_tag <= icache_output_tag;
						o_instruction <= icache_rdata;
						o_pc <= pc;

						pc <= pc + 4;

						if (is_BRANCH) begin
							// Branch instruction, need to wait
							// for an explicit "goto" signal before
							// we can continue feeding the pipeline.
							state <= 2;
						end
						else begin
							// Issue another request directly without going back
							// to state 0.
							if (!i_stall)
								icache_input_tag <= icache_input_tag + 1;
							else
								state <= 0;
						end
					end
				end

				2: begin
					// Wait for branch result.
					if (i_tag == o_tag) begin
						pc <= i_pc_next;
						state <= 0;
					end
				end
			endcase
		end
	end

endmodule
