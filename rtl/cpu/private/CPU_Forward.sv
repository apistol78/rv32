/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
`include "CPU_Types.sv"

`timescale 1ns/1ns

module CPU_Forward (
	input decode_data_t i_decode_data,
	input execute_data_t i_execute_data,
	input memory_data_t i_memory_data,
	input writeback_data_t i_writeback_data,

	input [31:0] i_rs1,
	input [31:0] i_rs2,
	input [31:0] i_rs3,

	output [31:0] o_rs1,
	output [31:0] o_rs2,
	output [31:0] o_rs3
);
	assign o_rs1 = rs1;
	assign o_rs2 = rs2;
	assign o_rs3 = rs3;

	bit [31:0] rs1;
	bit [31:0] rs2;
	bit [31:0] rs3;

	// Forward RS1 from pipeline.
	always_comb begin
		rs1 = 32'h0;
		if (i_decode_data.have_rs[0]) begin
			rs1 =
				(i_decode_data.inst_rs1 == i_execute_data.inst_rd) ? i_execute_data.rd :
				(i_decode_data.inst_rs1 == i_memory_data.inst_rd) ? i_memory_data.rd :
				(i_decode_data.inst_rs1 == i_writeback_data.inst_rd) ? i_writeback_data.rd :
				i_rs1;
		end
	end

	// Forward RS2 from pipeline.
	always_comb begin
		rs2 = 32'h0;
		if (i_decode_data.have_rs[1]) begin
			rs2 =
				(i_decode_data.inst_rs2 == i_execute_data.inst_rd) ? i_execute_data.rd :
				(i_decode_data.inst_rs2 == i_memory_data.inst_rd) ? i_memory_data.rd :
				(i_decode_data.inst_rs2 == i_writeback_data.inst_rd) ? i_writeback_data.rd :
				i_rs2;
		end
	end

	// Forward RS3 from pipeline.
	always_comb begin
		rs3 = 32'h0;
		if (i_decode_data.have_rs[2]) begin
			rs3 =
				(i_decode_data.inst_rs3 == i_execute_data.inst_rd) ? i_execute_data.rd :
				(i_decode_data.inst_rs3 == i_memory_data.inst_rd) ? i_memory_data.rd :
				(i_decode_data.inst_rs3 == i_writeback_data.inst_rd) ? i_writeback_data.rd :
				i_rs3;
		end
	end

endmodule
