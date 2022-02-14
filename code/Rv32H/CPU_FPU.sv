`include "CPU_Defines.sv"

`timescale 1ns/1ns

// https://github.com/dawsonjon/fpu

module CPU_FPU(
	input i_reset,
	input i_clock,

	input i_request,

	input [3:0] i_op,
	input [31:0] i_op1,
	input [31:0] i_op2,
	input [31:0] i_op3,

	output o_ready,
	output [31:0] o_result
);

	wire fadd_ready;
	wire [31:0] fadd_result;
	CPU_FPU_Add fadd(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(i_request),
		.i_op1(i_op1),
		.i_op2(i_op2),
		.o_ready(fadd_ready),
		.o_result(fadd_result)
	);

	wire fsub_ready;
	wire [31:0] fsub_result;
	CPU_FPU_Add fsub(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(i_request),
		.i_op1(i_op1),
		.i_op2(32'h80000000 ^ i_op2),
		.o_ready(fsub_ready),
		.o_result(fsub_result)
	);

	wire fmul_ready;
	wire [31:0] fmul_result;
	CPU_FPU_Mul fmul(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(i_request),
		.i_op1(i_op1),
		.i_op2(i_op2),
		.o_ready(fmul_ready),
		.o_result(fmul_result)
	);

	wire fdiv_ready;
	wire [31:0] fdiv_result;
	CPU_FPU_Div fdiv(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(i_request),
		.i_op1(i_op1),
		.i_op2(i_op2),
		.o_ready(fdiv_ready),
		.o_result(fdiv_result)
	);

	wire fint_ready;
	wire [31:0] fint_result;
	CPU_FPU_Int fint(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(i_request),
		.i_op1(i_op1),
		.o_ready(fint_ready),
		.o_result(fint_result)		
	);

	wire fcmp_ready = fsub_ready;
	wire [31:0] fcmp_equal = { 31'h0, ((fsub_result & 32'h7fffffff) == 32'h0) };
	wire [31:0] fcmp_less  = { 31'h0, fsub_result[31] };

	assign o_ready =
		i_op == `FPU_OP_ADD			? fadd_ready	:
		i_op == `FPU_OP_SUB			? fsub_ready	:
		i_op == `FPU_OP_MUL			? fmul_ready	:
		i_op == `FPU_OP_DIV			? fdiv_ready	:
		i_op == `FPU_OP_INT			? fint_ready	:
		i_op == `FPU_OP_MOV			? 1'b1			:
		i_op == `FPU_OP_CMP_EQUAL	? fcmp_ready	:
		i_op == `FPU_OP_CMP_LESS	? fcmp_ready	:
		i_op == `FPU_OP_CMP_LEQUAL	? fcmp_ready	:
		0;

	assign o_result =
		i_op == `FPU_OP_ADD			? fadd_result				:
		i_op == `FPU_OP_SUB			? fsub_result				:
		i_op == `FPU_OP_MUL			? fmul_result				:
		i_op == `FPU_OP_DIV			? fdiv_result				:
		i_op == `FPU_OP_INT			? fint_result				:
		i_op == `FPU_OP_MOV			? i_op1						:
		i_op == `FPU_OP_CMP_EQUAL	? fcmp_equal				:
		i_op == `FPU_OP_CMP_LESS	? fcmp_less					:
		i_op == `FPU_OP_CMP_LEQUAL	? fcmp_equal | fcmp_less	:
		0;

endmodule
