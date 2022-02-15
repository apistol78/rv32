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

	wire fadd_request = (i_op == `FPU_OP_ADD) && i_request; 
	wire fadd_ready;
	wire [31:0] fadd_result;
	CPU_FPU_Add fadd(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(fadd_request),
		.i_op1(i_op1),
		.i_op2(i_op2),
		.o_ready(fadd_ready),
		.o_result(fadd_result),
		.o_a(),
		.o_b()
	);

	wire fsub_request = (
		i_op == `FPU_OP_SUB ||
		i_op == `FPU_OP_CMP_EQUAL ||
		i_op == `FPU_OP_CMP_LESS ||
		i_op == `FPU_OP_CMP_LEQUAL ||
		i_op == `FPU_OP_MIN ||
		i_op == `FPU_OP_MAX
	) && i_request; 
	wire fsub_ready;
	wire [31:0] fsub_result;
	wire [31:0] fsub_a;
	wire [31:0] fsub_b;
	CPU_FPU_Add fsub(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(fsub_request),
		.i_op1(i_op1),
		.i_op2(32'h80000000 ^ i_op2),
		.o_ready(fsub_ready),
		.o_result(fsub_result),
		.o_a(fsub_a),
		.o_b(fsub_b)
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

	wire fmadd_request = (i_op == `FPU_OP_MADD || i_op == `FPU_OP_NMADD) && i_request; 
	wire fmadd_ready;
	wire [31:0] fmadd_result;
	wire [31:0] fnmadd_result = 32'h80000000 ^ fmadd_result;
	CPU_FPU_MulAdd fmadd(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(fmadd_request),
		.i_op1(i_op1),
		.i_op2(i_op2),
		.i_op3(i_op3),
		.o_ready(fmadd_ready),
		.o_result(fmadd_result)
	);

	wire fmsub_request = (i_op == `FPU_OP_MSUB || i_op == `FPU_OP_NMSUB) && i_request; 
	wire fmsub_ready;
	wire [31:0] fmsub_result;
	wire [31:0] fnmsub_result = 32'h80000000 ^ fmsub_result;
	CPU_FPU_MulAdd fmsub(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(fmsub_request),
		.i_op1(i_op1),
		.i_op2(i_op2),
		.i_op3(32'h80000000 ^ i_op3),
		.o_ready(fmsub_ready),
		.o_result(fmsub_result)
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

	wire fcmp_ready = fsub_ready;
	wire [31:0] fcmp_equal = { 31'h0, ((fsub_result & 32'h7fffffff) == 32'h0) };
	wire [31:0] fcmp_less  = { 31'h0, fsub_result[31] };

	wire [31:0] fsgnj_result = { i_op2[31], i_op1[30:0] };
	wire [31:0] fsgnjn_result = { ~i_op2[31], i_op1[30:0] };
	wire [31:0] fsgnjx_result = { i_op1[31] ^ i_op2[31], i_op1[30:0] };

	wire [31:0] fminmax_a = fsub_a;
	wire [31:0] fminmax_b = 32'h80000000 ^ fsub_b;
	wire fmin_ready = fsub_ready;
	wire [31:0] fmin_result = fsub_result[31] ? fminmax_a : fminmax_b;
	wire fmax_ready = fsub_ready;
	wire [31:0] fmax_result = fsub_result[31] ? fminmax_b : fminmax_a;

	assign o_ready =
		i_request & (
			i_op == `FPU_OP_ADD			? fadd_ready	:
			i_op == `FPU_OP_SUB			? fsub_ready	:
			i_op == `FPU_OP_MUL			? fmul_ready	:
			i_op == `FPU_OP_DIV			? fdiv_ready	:
			i_op == `FPU_OP_MADD		? fmadd_ready	:
			i_op == `FPU_OP_MSUB		? fmsub_ready	:
			i_op == `FPU_OP_NMADD		? fmadd_ready	:
			i_op == `FPU_OP_NMSUB		? fmsub_ready	:
			i_op == `FPU_OP_F2I			? f2i_ready		:
			i_op == `FPU_OP_I2F			? i2f_ready		:
			i_op == `FPU_OP_UI2F		? i2f_ready		:
			i_op == `FPU_OP_MOV			? 1'b1			:
			i_op == `FPU_OP_CMP_EQUAL	? fcmp_ready	:
			i_op == `FPU_OP_CMP_LESS	? fcmp_ready	:
			i_op == `FPU_OP_CMP_LEQUAL	? fcmp_ready	:
			i_op == `FPU_OP_SGNJ		? 1'b1			:
			i_op == `FPU_OP_SGNJN		? 1'b1			:
			i_op == `FPU_OP_SGNJX		? 1'b1			:
			i_op == `FPU_OP_MIN			? fmin_ready	:
			i_op == `FPU_OP_MAX			? fmax_ready	:
			0
		);

	assign o_result =
		i_op == `FPU_OP_ADD			? fadd_result				:
		i_op == `FPU_OP_SUB			? fsub_result				:
		i_op == `FPU_OP_MUL			? fmul_result				:
		i_op == `FPU_OP_DIV			? fdiv_result				:
		i_op == `FPU_OP_MADD		? fmadd_result				:
		i_op == `FPU_OP_MSUB		? fmsub_result				:
		i_op == `FPU_OP_NMADD		? fnmadd_result				:
		i_op == `FPU_OP_NMSUB		? fnmsub_result				:
		i_op == `FPU_OP_F2I			? f2i_result				:
		i_op == `FPU_OP_I2F			? i2f_result				:
		i_op == `FPU_OP_UI2F		? i2f_result				:
		i_op == `FPU_OP_MOV			? i_op1						:
		i_op == `FPU_OP_CMP_EQUAL	? fcmp_equal				:
		i_op == `FPU_OP_CMP_LESS	? fcmp_less					:
		i_op == `FPU_OP_CMP_LEQUAL	? fcmp_equal | fcmp_less	:
		i_op == `FPU_OP_SGNJ		? fsgnj_result				:
		i_op == `FPU_OP_SGNJN		? fsgnjn_result				:
		i_op == `FPU_OP_SGNJX		? fsgnjx_result				:
		i_op == `FPU_OP_MIN			? fmin_result				:
		i_op == `FPU_OP_MAX			? fmax_result				:
		0;

endmodule
