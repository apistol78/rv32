`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_SkidBuffer #(
	parameter DW
) (
	input i_reset,
	input i_clock,
	
	input i_busy,
	input [DW-1:0] i_data,
	output bit [DW-1:0] o_data
);

	bit [DW-1:0] r_data = 0;

	always_comb begin
		o_data = !i_busy ? i_data : r_data;
	end

	always_ff @(posedge i_clock) begin
		if (!i_busy) begin
			r_data <= i_data;
		end
	end

endmodule
