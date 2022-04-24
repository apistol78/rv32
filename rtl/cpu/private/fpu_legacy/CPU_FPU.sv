`include "CPU_Defines.sv"

`timescale 1ns/1ns

// https://github.com/dawsonjon/fpu

module CPU_FPU(
	input i_reset,
	input i_clock,

	input i_request,

	input [4:0] i_op,
	input [31:0] i_op1,
	input [31:0] i_op2,
	input [31:0] i_op3,

	output o_ready,
	output [31:0] o_result
);

	wire faddsub_request = (i_op == `FPU_OP_ADD || i_op == `FPU_OP_SUB) && i_request; 
	wire faddsub_ready;
	wire [31:0] faddsub_result;
	wire [31:0] faddsub_op2 = (i_op == `FPU_OP_ADD) ? i_op2 : (32'h80000000 ^ i_op2);
	CPU_FPU_Add faddsub(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(faddsub_request),
		.i_op1(i_op1),
		.i_op2(faddsub_op2),
		.o_ready(faddsub_ready),
		.o_result(faddsub_result)
	);

	wire fmul_request = (i_op == `FPU_OP_MUL) && i_request; 
	wire fmul_ready;
	wire [31:0] fmul_result;
	CPU_FPU_Mul fmul(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(fmul_request),
		.i_op1(i_op1),
		.i_op2(i_op2),
		.o_ready(fmul_ready),
		.o_result(fmul_result)
	);

	wire fdiv_request = (i_op == `FPU_OP_DIV) && i_request; 
	wire fdiv_ready;
	wire [31:0] fdiv_result;
	CPU_FPU_Div fdiv(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(fdiv_request),
		.i_op1(i_op1),
		.i_op2(i_op2),
		.o_ready(fdiv_ready),
		.o_result(fdiv_result)
	);

	wire fmaddsub_request = (i_op == `FPU_OP_MADD || i_op == `FPU_OP_NMADD || i_op == `FPU_OP_MSUB || i_op == `FPU_OP_NMSUB) && i_request; 
	wire fmaddsub_ready;
	wire [31:0] fmaddsub_result;
	wire [31:0] fnmaddsub_result = (32'h80000000 ^ fmaddsub_result);
	wire [31:0] fmaddsub_op3 = (i_op == `FPU_OP_MADD || i_op == `FPU_OP_NMADD) ? i_op3 : (32'h80000000 ^ i_op3);
	CPU_FPU_MulAdd fmaddsub(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(fmaddsub_request),
		.i_op1(i_op1),
		.i_op2(i_op2),
		.i_op3(fmaddsub_op3),
		.o_ready(fmaddsub_ready),
		.o_result(fmaddsub_result)
	);

	wire f2i_request = (i_op == `FPU_OP_F2I) && i_request; 
	wire f2i_ready;
	wire [31:0] f2i_result;
	CPU_FPU_Int f2i(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(f2i_request),
		.i_op1(i_op1),
		.o_ready(f2i_ready),
		.o_result(f2i_result)		
	);

	wire i2f_request = (i_op == `FPU_OP_I2F || i_op == `FPU_OP_UI2F) && i_request; 
	wire i2f_ready;
	wire [31:0] i2f_result;
	CPU_FPU_Float i2f(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(i2f_request),
		.i_op1(i_op1),
		.i_signed(i_op == `FPU_OP_I2F),
		.o_ready(i2f_ready),
		.o_result(i2f_result)		
	);

	wire cmp_request = (
		i_op == `FPU_OP_CMP_EQUAL ||
		i_op == `FPU_OP_CMP_LESS ||
		i_op == `FPU_OP_CMP_LEQUAL ||
		i_op == `FPU_OP_MIN ||
		i_op == `FPU_OP_MAX
	) && i_request;
	wire cmp_ready;
	wire cmp_less;
	wire cmp_equal;
	wire [31:0] cmp_min;
	wire [31:0] cmp_max;
	CPU_FPU_Compare cmp(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(cmp_request),
		.i_op1(i_op1),
		.i_op2(i_op2),
		.o_ready(cmp_ready),
		.o_less(cmp_less),
		.o_equal(cmp_equal),
		.o_min(cmp_min),
		.o_max(cmp_max)
	);

	wire [31:0] fcmp_equal = { 31'h0, cmp_equal };
	wire [31:0] fcmp_less = { 31'h0, cmp_less };
	wire [31:0] fcmp_lequal = { 31'h0, cmp_equal | cmp_less };

	wire [31:0] fsgnj_result = { i_op2[31], i_op1[30:0] };
	wire [31:0] fsgnjn_result = { ~i_op2[31], i_op1[30:0] };
	wire [31:0] fsgnjx_result = { i_op1[31] ^ i_op2[31], i_op1[30:0] };

	wire [31:0] fmin_result = cmp_min;
	wire [31:0] fmax_result = cmp_max;

	assign o_ready =
		i_request & (
			i_op == `FPU_OP_ADD			? faddsub_ready		:
			i_op == `FPU_OP_SUB			? faddsub_ready		:
			i_op == `FPU_OP_MUL			? fmul_ready		:
			i_op == `FPU_OP_DIV			? fdiv_ready		:
			i_op == `FPU_OP_MADD		? fmaddsub_ready	:
			i_op == `FPU_OP_MSUB		? fmaddsub_ready	:
			i_op == `FPU_OP_NMADD		? fmaddsub_ready	:
			i_op == `FPU_OP_NMSUB		? fmaddsub_ready	:
			i_op == `FPU_OP_F2I			? f2i_ready			:
			i_op == `FPU_OP_I2F			? i2f_ready			:
			i_op == `FPU_OP_UI2F		? i2f_ready			:
			i_op == `FPU_OP_MOV			? 1'b1				:
			i_op == `FPU_OP_CMP_EQUAL	? cmp_ready			:
			i_op == `FPU_OP_CMP_LESS	? cmp_ready			:
			i_op == `FPU_OP_CMP_LEQUAL	? cmp_ready			:
			i_op == `FPU_OP_SGNJ		? 1'b1				:
			i_op == `FPU_OP_SGNJN		? 1'b1				:
			i_op == `FPU_OP_SGNJX		? 1'b1				:
			i_op == `FPU_OP_MIN			? cmp_ready			:
			i_op == `FPU_OP_MAX			? cmp_ready			:
			0
		);

	assign o_result =
		i_op == `FPU_OP_ADD			? faddsub_result	:
		i_op == `FPU_OP_SUB			? faddsub_result	:
		i_op == `FPU_OP_MUL			? fmul_result		:
		i_op == `FPU_OP_DIV			? fdiv_result		:
		i_op == `FPU_OP_MADD		? fmaddsub_result	:
		i_op == `FPU_OP_MSUB		? fmaddsub_result	:
		i_op == `FPU_OP_NMADD		? fnmaddsub_result	:
		i_op == `FPU_OP_NMSUB		? fnmaddsub_result	:
		i_op == `FPU_OP_F2I			? f2i_result		:
		i_op == `FPU_OP_I2F			? i2f_result		:
		i_op == `FPU_OP_UI2F		? i2f_result		:
		i_op == `FPU_OP_MOV			? i_op1				:
		i_op == `FPU_OP_CMP_EQUAL	? fcmp_equal		:
		i_op == `FPU_OP_CMP_LESS	? fcmp_less			:
		i_op == `FPU_OP_CMP_LEQUAL	? fcmp_lequal		:
		i_op == `FPU_OP_SGNJ		? fsgnj_result		:
		i_op == `FPU_OP_SGNJN		? fsgnjn_result		:
		i_op == `FPU_OP_SGNJX		? fsgnjx_result		:
		i_op == `FPU_OP_MIN			? fmin_result		:
		i_op == `FPU_OP_MAX			? fmax_result		:
		0;

endmodule
