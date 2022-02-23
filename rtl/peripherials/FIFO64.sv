
`timescale 1ns/1ns

module FIFO64 #(
	parameter DEPTH = 64
)(
	input i_clock,
	output o_empty,
	output o_full,
	input i_write,
	input [63:0] i_wdata,
	input i_read,
	output [63:0] o_rdata
);

	logic [63:0] rdata = 0;
	logic [63:0] data [0:DEPTH - 1];
	logic [$clog2(DEPTH) - 1:0] in = 0;
	logic [$clog2(DEPTH) - 1:0] out = 0;

	assign o_empty = (in == out) ? 1'b1 : 1'b0;
	assign o_full = (((in + 1) & (DEPTH - 1)) == out) ? 1'b1 : 1'b0;
	assign o_rdata = rdata;

	always @ (posedge i_clock) begin
		if (i_write) begin
			data[in] <= i_wdata;
			in <= (in + 1) & (DEPTH - 1);
		end
		if (i_read) begin
			rdata <= data[out];
			out <= (out + 1) & (DEPTH - 1);
		end
	end

endmodule
