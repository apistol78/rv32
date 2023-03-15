/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

`timescale 1ns/1ns

module GPIO (
	input i_clock,
	input i_request,
	input i_rw,
	input [1:0] i_address,
	input [31:0] i_wdata,
	output bit [31:0] o_rdata,
	output bit o_ready,

	inout [35:0] GPIO
);
	localparam DIR_IN = 1'b0;
	localparam DIR_OUT = 1'b1;

	bit [7:0] ctrl = 8'h00;		// All pins as inputs initially.
	bit [7:0] outd = 8'h00;
	
	assign GPIO[0] = ctrl[0] ? outd[0] : 1'bz;
	assign GPIO[1] = ctrl[1] ? outd[1] : 1'bz;

	always_ff @(posedge i_clock) begin
		if (i_request) begin
			if (!i_rw) begin
				if (i_address == 2'h0) begin		// ctrl
					o_rdata <= { 24'h0, ctrl };
				end
				else if (i_address == 2'h1) begin	// output
					o_rdata <= { 24'h0, outd };
				end			
			end
			else begin	// write
				if (i_address == 2'h0) begin		// ctrl
					ctrl <= i_wdata[7:0];
				end
				else if (i_address == 2'h1) begin	// output
					outd <= i_wdata[7:0];
				end
			end
		end
	end

	always_ff @(posedge i_clock)
		o_ready <= i_request;

endmodule
