`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_Writeback(
	input wire i_reset,
	input wire i_clock,

	// Input
	input memory_data_t i_data,

	// Output
	output writeback_data_t o_data,
	output [31:0] o_retired
);
	assign o_data = data;
	assign o_retired = retired;

	writeback_data_t data = 0;
	logic [31:0] retired = 0;

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			data <= 0;
			retired <= 0;
		end
		else begin
			if (i_data.tag != data.tag) begin
				data.tag <= i_data.tag;
				data.rd <= i_data.rd;
				retired <= retired + 1;
			end
		end
	end

endmodule
