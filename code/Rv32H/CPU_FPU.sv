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

	assign o_ready =
		i_op == `FPU_OP_ADD ? fadd_ready :
		i_op == `FPU_OP_SUB ? fsub_ready :
		i_op == `FPU_OP_MUL ? fmul_ready :
		i_op == `FPU_OP_DIV ? fdiv_ready :
		0;

	assign o_result =
		i_op == `FPU_OP_ADD ? fadd_result :
		i_op == `FPU_OP_SUB ? fsub_result :
		i_op == `FPU_OP_MUL ? fmul_result :
		i_op == `FPU_OP_DIV ? fdiv_result :
		0;

endmodule
