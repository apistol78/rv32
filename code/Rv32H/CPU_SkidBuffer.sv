`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_SkidBuffer #(
	parameter DW = 8
) (
	input i_reset,
	input i_clock,

	input [`TAG_SIZE] i_tag,
	input [DW-1:0] i_data,
	output reg o_busy,

	output reg [`TAG_SIZE] o_tag,
	output reg [DW-1:0] o_data,
	input i_busy
);

	reg [`TAG_SIZE] tag = 0;

	reg [`TAG_SIZE] r_tag = 0;
	reg [DW-1:0] r_data = 0;
	reg r_valid = 0;

	always_comb begin
		o_busy = i_busy || r_valid;
	end

	always_ff @(posedge i_clock) begin

		if (i_tag != tag) begin
			if (!i_busy) begin
				o_tag <= i_tag;
				o_data <= i_data;
			end
			else begin
				r_tag <= i_tag;
				r_data <= i_data;
				r_valid <= 1;
			end

			tag <= i_tag;
		end

		if (!i_busy && r_valid) begin
			o_data <= r_data;
			r_valid <= 0;
		end

		
	end

endmodule
