/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

`timescale 1ns/1ns

module FIFO_BRAM #(
	parameter DEPTH,
	parameter WIDTH
)(
	input i_reset,
	input i_clock,

	output o_empty,
	output o_full,

	input i_write,
	input [WIDTH-1:0] i_wdata,

	input i_read,
	output [WIDTH-1:0] o_rdata,

	output [$clog2(DEPTH) - 1:0] o_queued
);
	localparam DB = $clog2(DEPTH) - 1;

	BRAM_1r1w #(
		.WIDTH(WIDTH),
		.SIZE(DEPTH),
		.ADDR_LSH(0)
	) bram(
		.i_clock(i_clock),

		.i_pa_request(i_read),
		.i_pa_address(out),
		.o_pa_rdata(o_rdata),
		.o_pa_ready(),

		.i_pb_request(i_write),
		.i_pb_address(in),
		.i_pb_wdata(i_wdata),
		.o_pb_ready()
	);

	bit [DB:0] in = 0;
	bit [DB:0] out = 0;

	assign o_empty = (in == out) ? 1'b1 : 1'b0;
	assign o_full = (((in + 1) & (DEPTH - 1)) == out) ? 1'b1 : 1'b0;
	assign o_queued = (in >= out) ? in - out : (DEPTH - out) + in;

	always_ff @ (posedge i_clock) begin
		if (!i_reset) begin
			if (i_write) begin
				in <= (in + 1) & (DEPTH - 1);
			end
			if (i_read) begin
				out <= (out + 1) & (DEPTH - 1);
			end
		end
		else begin
			in <= 0;
			out <= 0;
		end
	end

endmodule
