`include "CPU_Defines.v"

`timescale 1ns/1ns

module CPU_Hazard(
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
	input wire i_compare,
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
	output reg [31:0] o_instruction,
	output reg [31:0] o_pc,
	output reg [31:0] o_rs1,
	output reg [31:0] o_rs2,
	output reg [4:0] o_inst_rd,
	output reg [31:0] o_imm,

	output reg o_arithmetic,
	output reg o_compare,
	output reg o_jump,
	output reg o_jump_conditional,

	output reg [3:0] o_alu_operation,
	output reg [2:0] o_alu_operand1,
	output reg [2:0] o_alu_operand2,

	output reg o_memory_read,
	output reg o_memory_write,
	output reg [2:0] o_memory_width,
	output reg o_memory_signed,

	output reg [4:0] o_op
);

	initial begin
		o_tag = 0;
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			o_tag <= 0;
		end
		else begin
			if (!i_stall && i_tag != o_tag) begin
				o_instruction <= i_instruction;
				o_pc <= i_pc;
				o_rs1 <= i_rs1;
				o_rs2 <= i_rs2;
				o_inst_rd <= i_inst_rd;
				o_imm <= i_imm;
				o_arithmetic <= i_arithmetic;
				o_compare <= i_compare;
				o_jump <= i_jump;
				o_jump_conditional <= i_jump_conditional;
				o_alu_operation <= i_alu_operation;
				o_alu_operand1 <= i_alu_operand1;
				o_alu_operand2 <= i_alu_operand2;
				o_memory_read <= i_memory_read;
				o_memory_write <= i_memory_write;
				o_memory_width <= i_memory_width;
				o_memory_signed <= i_memory_signed;
				o_op <= i_op;
				o_tag <= i_tag;
			end
		end		
	end

endmodule
