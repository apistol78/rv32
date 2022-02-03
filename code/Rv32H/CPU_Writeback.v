`include "CPU_Defines.v"

`timescale 1ns/1ns

module CPU_Writeback(
	input wire i_reset,
	input wire i_clock,

	// Control
	input wire i_memory_valid,
	output reg o_busy,
	output reg o_valid,

	// Debug
	input wire [`TAG_SIZE] i_tag,
	output reg [`TAG_SIZE] o_tag,

	// Input
	input wire [4:0] i_inst_rd,
	input wire [31:0] i_rd,

	// Output
	output reg [4:0] o_inst_rd,
	output reg [31:0] o_rd
);

	initial begin
		o_busy = 0;
		o_valid = 0;
		o_tag = 0;
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			o_busy <= 0;
			o_valid <= 0;
			o_tag <= 0;
		end
		else begin
			if (i_memory_valid) begin
				o_valid <= 1'b1;
				o_tag <= i_tag;
				o_inst_rd <= i_inst_rd;
				o_rd <= i_rd;
			end
		end
	end

endmodule
