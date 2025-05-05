/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

`timescale 1ns/1ns

module DualPort(
	input i_reset,
	input i_clock,

	// Bus
	output bit o_bus_rw,				// Data read/write
	output bit o_bus_request,			// IO request.
	input i_bus_ready,					// IO request ready.
	output bit [31:0] o_bus_address,	// Address
	input [31:0] i_bus_rdata,			// Read data
	output bit [31:0] o_bus_wdata,		// Write data,

	// Port B
	input i_pb_rw,
	input i_pb_request,
	output bit o_pb_ready,
	input [31:0] i_pb_address,
	output bit [31:0] o_pb_rdata,
	input [31:0] i_pb_wdata,

	// Port C
	input i_pc_rw,
	input i_pc_request,
	output bit o_pc_ready,
	input [31:0] i_pc_address,
	output bit [31:0] o_pc_rdata,
	input [31:0] i_pc_wdata
);

	bit [1:0] state = 0;
	bit [1:0] next_state;

	assign o_pb_ready = i_pb_request && (state == 2'd1) ? i_bus_ready : 1'b0;
	assign o_pc_ready = i_pc_request && (state == 2'd2) ? i_bus_ready : 1'b0;

	assign o_pb_rdata = i_bus_rdata;
	assign o_pc_rdata = i_bus_rdata;

	always_ff @(posedge i_clock) begin
		state <= next_state;
	end

	always_comb begin

		next_state = state;

		o_bus_request = 0;
		o_bus_rw = 0;
		o_bus_address = 0;
		o_bus_wdata = 0;
		
		case (state)

			// Wait for any request.
			2'd0: begin
				if (i_pb_request) begin
					o_bus_rw = i_pb_rw;
					o_bus_address = i_pb_address;
					o_bus_wdata = i_pb_wdata;
					next_state = 2'd1;
				end
				else if (i_pc_request) begin
					o_bus_rw = i_pc_rw;
					o_bus_address = i_pc_address;
					o_bus_wdata = i_pc_wdata;
					next_state = 2'd2;
				end					
			end

			// Wait until request has been processed.
			2'd1: begin
				o_bus_request = i_pb_request;
				o_bus_rw = i_pb_rw;
				o_bus_address = i_pb_address;
				o_bus_wdata = i_pb_wdata;
				if (i_bus_ready) begin
					next_state = 2'd0;
				end
			end
			2'd2: begin
				o_bus_request = i_pc_request;
				o_bus_rw = i_pc_rw;
				o_bus_address = i_pc_address;
				o_bus_wdata = i_pc_wdata;
				if (i_bus_ready) begin
					next_state = 2'd0;
				end
			end

			default:
				next_state = 2'd0;

		endcase
	end

endmodule
