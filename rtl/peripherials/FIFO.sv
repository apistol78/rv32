
`timescale 1ns/1ns

module FIFO #(
	parameter DEPTH,
	parameter WIDTH
)(
	input i_clock,
	output o_empty,
	output o_full,
	input i_write,
	input [WIDTH-1:0] i_wdata,
	input i_read,
	output [WIDTH-1:0] o_rdata,
	output [$clog2(DEPTH) - 1:0] o_queued
);

	bit [WIDTH-1:0] rdata = 0;
	bit [WIDTH-1:0] data [0:DEPTH - 1];
	bit [$clog2(DEPTH) - 1:0] in = 0;
	bit [$clog2(DEPTH) - 1:0] out = 0;

	assign o_empty = (in == out) ? 1'b1 : 1'b0;
	assign o_full = (((in + 1) & (DEPTH - 1)) == out) ? 1'b1 : 1'b0;
	assign o_rdata = rdata;
	assign o_queued = (in >= out) ? in - out : (DEPTH - out) + in;

	always_ff @ (posedge i_clock) begin
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