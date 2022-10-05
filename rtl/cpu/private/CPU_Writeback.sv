`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_Writeback(
	input i_reset,
	input i_clock,

	// Input
	input memory_data_t i_data,

	// Output
	output writeback_data_t o_data,
	output [63:0] o_retired
);
	assign o_data = data;
	assign o_retired = retired;

	writeback_data_t data = 0;
	bit [63:0] retired = 0;
	bit last_strobe = 0;

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			data <= 0;
			retired <= 0;
		end
		else begin
			data.inst_rd <= i_data.inst_rd;
			data.rd <= i_data.rd;

			if (i_data.strobe != last_strobe) begin
				retired <= retired + 1;
			end
			last_strobe <= i_data.strobe;
		end
	end

endmodule
