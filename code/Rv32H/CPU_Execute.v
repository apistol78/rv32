`include "CPU_Defines.v"

`timescale 1ns/1ns

module CPU_Execute (
	input wire i_reset,
	input wire i_clock,
	input wire i_stall,

	// Input
	input wire [`TAG_SIZE] i_tag,
	input wire [31:0] i_pc,
	input wire [31:0] i_instruction,
	input wire [31:0] i_rs1,
	input wire [31:0] i_rs2,
	input wire [4:0] i_inst_rd,
	input wire [31:0] i_imm,

	input wire i_arithmetic,
	input wire i_jump,
	input wire i_jump_conditional,

	input wire [3:0] i_alu_operation,
	input wire [2:0] i_alu_operand1,
	input wire [2:0] i_alu_operand2,

	input wire i_memory_read,
	input wire i_memory_write,
	input wire [2:0] i_memory_width,
	input wire i_memory_signed,

	input wire [4:0] i_op,
	
	// Output
	output reg [`TAG_SIZE] o_tag,
	output reg [4:0] o_inst_rd,
	output reg [31:0] o_rd,
	output reg [31:0] o_pc_next,
	output reg o_mem_read,
	output reg o_mem_write,
	output reg [2:0] o_mem_width,
	output reg o_mem_signed,
	output reg [31:0] o_mem_address,

	output wire o_stall
);

	`include "Instructions_ops.v"

	// Alias symbols for generated code.
	`undef PC
	`undef RS1
	`undef RS2
	`undef PD
	`undef IMM
	`undef ZERO
	`define PC		i_pc
	`define RS1		i_rs1
	`define RS2		i_rs2
	`define RD		o_rd
	`define IMM		i_imm
	`define ZERO	0

	`define GOTO(ADDR) 				\
		o_pc_next <= ADDR;
	
	`define CYCLE					\
		cycle

	`define EXECUTE_OP				\
		i_op

	`define EXECUTE_DONE			\
		o_tag <= i_tag;				\
		cycle <= 0;

	reg [4:0] cycle;
	
	wire [31:0] alu_operand1 =
		(i_alu_operand1 == 3'd0) ? `ZERO :
		(i_alu_operand1 == 3'd1) ? `RS1 :
		(i_alu_operand1 == 3'd2) ? `RS2 :
		(i_alu_operand1 == 3'd3) ? `PC  :
		(i_alu_operand1 == 3'd4) ? `IMM :
		32'd0;

	wire [31:0] alu_operand2 =
		(i_alu_operand2 == 3'd0) ? `ZERO :
		(i_alu_operand2 == 3'd1) ? `RS1 :
		(i_alu_operand2 == 3'd2) ? `RS2 :
		(i_alu_operand2 == 3'd3) ? `PC  :
		(i_alu_operand2 == 3'd4) ? `IMM :
		32'd0;

	wire [31:0] alu_result;
	wire alu_compare_result;
	CPU_ALU alu(
		.i_op(i_alu_operation),
		.i_op1(alu_operand1),
		.i_op2(alu_operand2),
		.o_result(alu_result),
		.o_compare_result(alu_compare_result)
	);

	reg mul_signed;
	reg [31:0] mul_op1;
	reg [31:0] mul_op2;
	wire [63:0] mul_result;
	CPU_Multiply multiply(
		.i_clock(i_clock),
		.i_signed(mul_signed),
		.i_op1(mul_op1),
		.i_op2(mul_op2),
		.o_result(mul_result)
	);

	reg div_signed;
	reg [31:0] div_numerator;
	reg [31:0] div_denominator;
	wire [31:0] div_result;
	wire [31:0] div_remainder;
	CPU_Divide divide(
		.i_clock(i_clock),
		.i_signed(div_signed),
		.i_numerator(div_numerator),
		.i_denominator(div_denominator),
		.o_result(div_result),
		.o_remainder(div_remainder)
	);

	initial begin
		o_inst_rd = 0;
		o_pc_next = 0;
		o_mem_read = 0;
		o_mem_write = 0;
		o_mem_width = 0;
		o_mem_signed = 0;
		o_mem_address = 0;
		o_tag = 0;
		cycle = 0;
	end

	assign o_stall = (i_tag != o_tag) && (cycle != 0);
	
	wire [31:0] jump_conditional_target = $signed(`PC) + $signed(`IMM);

	always @(posedge i_clock) begin
		if (i_reset) begin
			o_inst_rd <= 0;
			o_pc_next <= 0;
			o_mem_read <= 0;
			o_mem_write <= 0;
			o_mem_width <= 0;
			o_mem_signed <= 0;
			o_mem_address <= 0;
			o_tag <= 0;
			cycle <= 0;
		end
		else begin
			if (!i_stall && i_tag != o_tag) begin
				o_pc_next <= i_pc + 4;
				o_inst_rd <= i_inst_rd;

				o_mem_address <= alu_result;
				o_mem_read <= i_memory_read;
				o_mem_write <= i_memory_write;
				o_mem_width <= i_memory_width;
				o_mem_signed <= i_memory_signed;

				cycle <= cycle + 1;

				if (i_arithmetic) begin
					`RD <= alu_result;
					`EXECUTE_DONE;				
				end
				else if (i_jump) begin
					`RD <= `PC + 4;
					`GOTO(alu_result);
					`EXECUTE_DONE;
				end
				else if (i_jump_conditional) begin
					if (alu_compare_result) begin
						`GOTO(jump_conditional_target);
					end
					`EXECUTE_DONE;
				end
				else if (i_memory_read) begin
					`EXECUTE_DONE;
				end
				else if (i_memory_write) begin
					`RD <=`RS2;
					`EXECUTE_DONE;
				end
				else begin
					// Note, input values are only valid in first cycle so
					// in case of multicycle operations the inputs must be
					// stored in temporary registers.
					`include "Instructions_execute_ops.v"
				end
			end
		end
	end

endmodule