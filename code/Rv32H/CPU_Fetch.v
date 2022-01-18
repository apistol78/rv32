`include "CPU_Defines.v"

`timescale 1ns/1ns

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
	output reg [31:0] o_pc,

	// Debug
	output wire [31:0] o_icache_hit_count,
	output wire [31:0] o_icache_miss_count
);

	reg [2:0] state;
	reg [31:0] pc;
	
	// ICache
	wire [31:0] icache_output_pc;
	wire [31:0] icache_rdata;

	CPU_ICache icache(
		.i_reset(i_reset),
		.i_clock(i_clock),

		// Input
		.i_input_pc(pc),
		.o_output_pc(icache_output_pc),

		// Output
		.o_rdata(icache_rdata),

		// Bus
		.o_bus_request(o_bus_request),
		.i_bus_ready(i_bus_ready),
		.o_bus_address(o_bus_address),
		.i_bus_rdata(i_bus_rdata),

		// Debug
		.o_icache_hit_count(o_icache_hit_count),
		.o_icache_miss_count(o_icache_miss_count)		
	);

	// 
	`undef INSTRUCTION
	`define INSTRUCTION icache_rdata
	`include "Instructions_decode.v"

	initial begin
		state = 0;
		pc = 32'h00000000;
		o_tag = 0;
		o_instruction = 0;
		o_pc = 0;
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			state <= 0;
			pc <= 32'h00000000;
			o_tag <= 0;
			o_instruction <= 0;
			pc <= 32'h00000000;
		end
		else begin
			case (state)
				0: begin
					if (!i_stall && icache_output_pc == pc) begin
						// Output to pipeline instruction and PC.
						o_tag <= o_tag + 1;
						o_instruction <= icache_rdata;
						o_pc <= pc;

						if (is_JUMP || is_JUMP_CONDITIONAL) begin
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
