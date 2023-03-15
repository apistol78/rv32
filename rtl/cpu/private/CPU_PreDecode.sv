/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_PreDecode(
	input i_reset,
	input i_clock,

	// Input
	input fetch_data_t i_data,

	// Output
	output fetch_data_t o_data
);
	assign o_data = data;

	fetch_data_t data = 0;

	// 
	`undef INSTRUCTION
	`define INSTRUCTION i_data.instruction
	`include "private/generated/Instructions_decode.sv"

	wire have_RS1 = is_B | is_I | is_R | is_S | is_CSR | is_R4;
	wire have_RS2 = is_B | is_R | is_S | is_R4;
	wire have_RS3 = is_R4;
	wire have_RD  = is_I | is_J | is_R | is_U | is_CSR | is_R4;

	wire [31:0] inst_B_imm = { { 20{ `INSTRUCTION[31] } }, `INSTRUCTION[7], `INSTRUCTION[30:25], `INSTRUCTION[11:8], 1'b0 };
	wire [31:0] inst_I_imm = { { 21{ `INSTRUCTION[31] } }, `INSTRUCTION[30:20] };
	wire [31:0] inst_J_imm = { { 12{ `INSTRUCTION[31] } }, `INSTRUCTION[19:12], `INSTRUCTION[20], `INSTRUCTION[30:21], 1'b0 };
	wire [31:0] inst_S_imm = { { 21{ `INSTRUCTION[31] } }, `INSTRUCTION[30:25], `INSTRUCTION[11:7] };
	wire [31:0] inst_U_imm = { `INSTRUCTION[31:12], 12'b0 };
	wire [31:0] inst_R_imm = { 26'b0, `INSTRUCTION[25:20] };
	wire [31:0] inst_CSR_imm = { 20'b0, `INSTRUCTION[31:20] };
	
	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			data <= 0;
		end
		else begin
			data <= i_data;

			// Decode register indices here since we
			// need those for fetching registers while
			// we are decoding rest of instruction.
			data.inst_rs1 <= register_t'(have_RS1 ? { `INSTRUCTION[19:15] } : 5'h0);
			data.inst_rs2 <= register_t'(have_RS2 ? { `INSTRUCTION[24:20] } : 5'h0);
			//data.inst_rs3 <= register_t'(have_RS3 ? { `INSTRUCTION[31:27] } : 5'h0);
			data.inst_rd  <= register_t'(have_RD  ? { `INSTRUCTION[ 11:7] } : 5'h0);
			
			data.imm <=
				is_B ? inst_B_imm :
				is_I ? inst_I_imm :
				is_J ? inst_J_imm :
				is_S ? inst_S_imm :
				is_U ? inst_U_imm :
				is_R ? inst_R_imm :
				is_CSR ? inst_CSR_imm :
				32'h0;
		end
	end

endmodule
