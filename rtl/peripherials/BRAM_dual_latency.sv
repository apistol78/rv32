/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

`timescale 1ns/1ns

module BRAM_dual_latency #(
	parameter WIDTH = 32,
	parameter SIZE = 32'h400,
	parameter ADDR_LSH = 2,
	parameter LATENCY = 10
)(
	input i_clock,

    // Port A
	input i_pa_request,
	input i_pa_rw,
	input [31:0] i_pa_address,
	input [WIDTH - 1:0] i_pa_wdata,
	output logic [WIDTH - 1:0] o_pa_rdata,
	output bit o_pa_ready,

    // Port B
	input i_pb_request,
	input i_pb_rw,
	input [31:0] i_pb_address,
	input [WIDTH - 1:0] i_pb_wdata,
	output logic [WIDTH - 1:0] o_pb_rdata,
	output bit o_pb_ready
);
    (* ram_style = "block" *)
	logic [WIDTH - 1:0] data [0:SIZE - 1];
	bit [15:0] pa_counter;
	bit [15:0] pb_counter;

	initial begin
        o_pa_ready = 0;
        o_pb_ready = 0;
    end

	always_ff @(posedge i_clock) begin
		if (i_pa_request) begin
			if (!i_pa_rw) begin
				o_pa_rdata <= data[i_pa_address >> ADDR_LSH];
			end
			else begin
				data[i_pa_address >> ADDR_LSH] <= i_pa_wdata;
			end
			pa_counter <= pa_counter + 1;
		end
		else
			pa_counter <= 0;
		o_pa_ready <= i_pa_request && (pa_counter >= LATENCY);
	end

	always_ff @(posedge i_clock) begin
		if (i_pb_request) begin
			if (!i_pb_rw) begin
				o_pb_rdata <= data[i_pb_address >> ADDR_LSH];
			end
			else begin
				data[i_pb_address >> ADDR_LSH] <= i_pb_wdata;
			end
			pb_counter <= pb_counter + 1;
		end
		else
			pb_counter <= 0;
		o_pb_ready <= i_pb_request && (pb_counter >= LATENCY);
	end

endmodule