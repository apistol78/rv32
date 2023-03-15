/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

`timescale 1ns/1ns

module Timer#(
    parameter FREQUENCY
)(
	input i_reset,
	input i_clock,
	input i_request,
	input i_rw,
	input [3:0] i_address,
	input [31:0] i_wdata,
	output bit [31:0] o_rdata,
	output bit o_ready,

	output bit o_interrupt
);
	localparam PRESCALE = FREQUENCY / 1000;
	localparam PRESCALE_WIDTH = $clog2(PRESCALE);

	bit [PRESCALE_WIDTH - 1:0] prescale = 0;
	bit [63:0] cycles = 64'd0;
	bit [63:0] compare = { 64{1'b1} };
	bit [31:0] countdown = 32'd0;
	bit [31:0] ms = 32'd0;
	bit request = 1'b0;
	bit [1:0] mode = 2'd0;

	initial begin
		o_ready = 1'b0;
		o_interrupt = 1'b0;
	end

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			ms <= 0;
			prescale <= 0;
		end
		else begin
			prescale <= prescale + 1;
			if (prescale >= PRESCALE) begin
				ms <= ms + 1;
				prescale <= 0;
			end
		end
	end

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			cycles <= 64'd0;
		end
		else begin
			// Increment cycle counter.
			cycles <= cycles + 64'd1;

			if (countdown > 0) begin
				countdown <= countdown - 32'd1;
			end

			// Interrupt mode.
			case (mode)
				1: o_interrupt <= (cycles == compare) ? 1'b1 : 1'b0;
				2: o_interrupt <= (countdown == 32'd1) ? 1'b1 : 1'b0;
				default: o_interrupt <= 1'b0;
			endcase

			// Handle CPU requests.
			request <= i_request;
			if (i_request && !request) begin
				if (!i_rw) begin
					case (i_address)
						4'h0: o_rdata <= ms;
						4'h1: o_rdata <= cycles[31:0];
						4'h2: o_rdata <= cycles[63:32];
						4'h3: o_rdata <= compare[31:0];
						4'h4: o_rdata <= compare[63:32];
						4'h5: o_rdata <= countdown;
						default: o_rdata <= 0;
					endcase
				end
				else begin
					case (i_address)
						4'h3: begin compare[31:0] <= i_wdata; mode <= 1; end
						4'h4: begin compare[63:32] <= i_wdata; mode <= 1; end
						4'h5: begin countdown <= i_wdata; mode <= 2; end
						default:;
					endcase
				end
				o_ready <= 1'b1;
			end
			else if (!i_request)
				o_ready <= 1'b0;
				
		end
	end
	
endmodule
