/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

`timescale 1ns/1ns

module BRAM_1r1w #(
	parameter WIDTH = 32,
	parameter SIZE = 32'h400,
	parameter ADDR_LSH = 2
)(
	input i_clock,

    // Port A (read)
	input i_pa_request,
	input [31:0] i_pa_address,
	output logic [WIDTH - 1:0] o_pa_rdata,
	output bit o_pa_ready,

    // Port B (write)
	input i_pb_request,
	input [31:0] i_pb_address,
	input [WIDTH - 1:0] i_pb_wdata,
	output bit o_pb_ready
);
    (* ram_style = "block" *)
	logic [WIDTH - 1:0] data [0:SIZE - 1];

	initial begin
        o_pa_ready = 0;
        o_pb_ready = 0;
    end

	always_ff @(posedge i_clock) begin
		o_pa_ready <= 1'b0;
		o_pb_ready <= 1'b0;
		if (i_pa_request) begin
			o_pa_rdata <= data[i_pa_address >> ADDR_LSH];
			o_pa_ready <= 1'b1;
		end
		if (i_pb_request) begin
			data[i_pb_address >> ADDR_LSH] <= i_pb_wdata;
			o_pb_ready <= 1'b1;
		end
	end

endmodule