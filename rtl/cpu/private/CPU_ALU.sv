/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_ALU(
	input [3:0] i_op,

	input [31:0] i_op1,
	input [31:0] i_op2,

	output [31:0] o_result,
	output [31:0] o_shift_result,
	output [31:0] o_signed_sum_result,
	output o_compare_result
);

	wire [31:0] signed_sum = $signed(i_op1) + $signed(i_op2);
	wire [31:0] unsigned_sum = i_op1 + i_op2;
	wire [31:0] signed_diff = $signed(i_op1) - $signed(i_op2);
	wire [31:0] and_result = i_op1 & i_op2;
	wire [31:0] or_result = i_op1 | i_op2;
	wire [31:0] xor_result = i_op1 ^ i_op2;
	wire [31:0] shl_result = i_op1 << i_op2[4:0];
	wire [31:0] shr_result = i_op1 >> i_op2[4:0];
	wire [31:0] ashr_result = $signed(i_op1) >>> i_op2[4:0];

	wire signed_lt_result = ($signed(i_op1) < $signed(i_op2)) ? 1'b1 : 1'b0;
	wire unsigned_lt_result = (i_op1 < i_op2) ? 1'b1 : 1'b0;
	wire equal_result = (i_op1 == i_op2) ? 1'b1 : 1'b0;
	wire not_equal_result = (i_op1 != i_op2) ? 1'b1 : 1'b0;
	wire signed_get_result = ($signed(i_op1) >= $signed(i_op2)) ? 1'b1 : 1'b0;
	wire unsigned_get_result = (i_op1 >= i_op2) ? 1'b1 : 1'b0;

	assign o_result =
		i_op == `OP_SIGNED_ADD ? signed_sum :
		i_op == `OP_UNSIGNED_ADD ? unsigned_sum :
		i_op == `OP_SIGNED_SUB ? signed_diff :
		i_op == `OP_AND ? and_result :
		i_op == `OP_OR ? or_result :
		i_op == `OP_XOR ? xor_result :
		32'd0;

	assign o_shift_result =
		i_op == `OP_SHIFT_LEFT ? shl_result :
		i_op == `OP_SHIFT_RIGHT ? shr_result :
		i_op == `OP_ARITHMETIC_SHIFT_RIGHT ? ashr_result :
		32'd0;
		
	assign o_signed_sum_result = signed_sum;

	assign o_compare_result =
		i_op == `OP_SIGNED_LESS_THAN ? signed_lt_result :
		i_op == `OP_UNSIGNED_LESS_THAN ? unsigned_lt_result :
		i_op == `OP_EQUAL ? equal_result :
		i_op == `OP_NOT_EQUAL ? not_equal_result :
		i_op == `OP_SIGNED_GREATER_EQUAL ? signed_get_result :
		i_op == `OP_UNSIGNED_GREATER_EQUAL ? unsigned_get_result :
		1'b0;
		
endmodule
