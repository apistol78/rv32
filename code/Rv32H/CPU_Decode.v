`include "CPU_Defines.v"

`timescale 1ns/1ns

module CPU_Decode(
	input wire i_reset,
	input wire i_clock,
	input wire i_stall,

	// Input
	input wire [`TAG_SIZE] i_tag,
	input wire [31:0] i_instruction,
	input wire [31:0] i_pc,

	// Output
	output reg [`TAG_SIZE] o_tag,
	output reg [31:0] o_instruction,
	output reg [31:0] o_pc,
	output reg [4:0] o_inst_rs1,
	output reg [4:0] o_inst_rs2,
	output reg [4:0] o_inst_rd,
	output reg [31:0] o_imm,

	output reg o_arithmetic,
	output reg o_compare,
	output reg o_complex,
	output reg o_jump,
	output reg o_jump_conditional,

	output reg [3:0] o_alu_operation,
	output reg [2:0] o_alu_operand1,
	output reg [2:0] o_alu_operand2,

	output reg o_memory_read,
	output reg o_memory_write,
	output reg [2:0] o_memory_width,
	output reg o_memory_signed,

	output reg [4:0] o_op,
	
	output reg o_fault
);

	`include "Instructions_ops.v"

	`undef INSTRUCTION
	`define INSTRUCTION i_instruction
	`include "Instructions_decode.v"
	
	`undef ZERO
	`undef RS1
	`undef RS2
	`undef PC
	`undef IMM
	`define ZERO 3'd0
	`define RS1 3'd1
	`define RS2 3'd2
	`define PC	3'd3
	`define IMM 3'd4
	`include "Instructions_alu.v"

	`include "Instructions_memory.v"

	wire [31:0] inst_B_imm = { { 20{ `INSTRUCTION[31] } }, `INSTRUCTION[7], `INSTRUCTION[30:25], `INSTRUCTION[11:8], 1'b0 };
	wire [31:0] inst_I_imm = { { 21{ `INSTRUCTION[31] } }, `INSTRUCTION[30:20] };
	wire [31:0] inst_J_imm = { { 12{ `INSTRUCTION[31] } }, `INSTRUCTION[19:12], `INSTRUCTION[20], `INSTRUCTION[30:21], 1'b0 };
	wire [31:0] inst_S_imm = { { 21{ `INSTRUCTION[31] } }, `INSTRUCTION[30:25], `INSTRUCTION[11:7] };
	wire [31:0] inst_U_imm = { `INSTRUCTION[31:12], 12'b0 };
	wire [31:0] inst_R_imm = { 26'b0, `INSTRUCTION[25:20] };
	wire [31:0] inst_CSR_imm = { 20'b0, `INSTRUCTION[31:20] };
	
	wire have_RS1 = is_B | is_I | is_R | is_S | is_CSR;
	wire have_RS2 = is_B | is_R | is_S;
	wire have_RD  = is_I | is_J | is_R | is_U | is_CSR;

	initial begin
		o_tag = 0;
		o_instruction = 0;
		o_pc = 0;
		o_inst_rs1 = 0;
		o_inst_rs2 = 0;
		o_inst_rd = 0;
		o_imm = 0;
		o_arithmetic = 0;
		o_compare = 0;
		o_complex = 0;
		o_jump = 0;
		o_jump_conditional = 0;
		o_alu_operation = 0;
		o_alu_operand1 = 0;
		o_alu_operand2 = 0;
		o_memory_read = 0;
		o_memory_write = 0;
		o_memory_width = 0;
		o_memory_signed = 0;
		o_op = 0;
		o_fault = 0;
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			o_tag <= 0;
			o_instruction <= 0;
			o_pc <= 0;
			o_inst_rs1 <= 0;
			o_inst_rs2 <= 0;
			o_inst_rd <= 0;
			o_imm <= 0;
			o_arithmetic <= 0;
			o_compare <= 0;
			o_complex <= 0;
			o_jump <= 0;
			o_jump_conditional <= 0;
			o_alu_operation <= 0;
			o_alu_operand1 <= 0;
			o_alu_operand2 <= 0;
			o_memory_read <= 0;
			o_memory_write <= 0;
			o_memory_width <= 0;
			o_memory_signed <= 0;
			o_op <= 0;
			o_fault <= 0;
		end
		else begin
			if (!i_stall && i_tag != o_tag) begin
				o_instruction <= i_instruction;
				o_pc <= i_pc;

				o_inst_rs1 <= have_RS1 ? `INSTRUCTION[19:15] : 5'h0;
				o_inst_rs2 <= have_RS2 ? `INSTRUCTION[24:20] : 5'h0;
				o_inst_rd  <= have_RD  ? `INSTRUCTION[ 11:7] : 5'h0;
				
				o_imm <=
					is_B ? inst_B_imm :
					is_I ? inst_I_imm :
					is_J ? inst_J_imm :
					is_S ? inst_S_imm :
					is_U ? inst_U_imm :
					is_R ? inst_R_imm :
					is_CSR ? inst_CSR_imm :
					32'h0;
				
				o_arithmetic <= is_ARITHMETIC;
				o_compare <= is_COMPARE;
				o_complex <= is_COMPLEX;
				o_jump <= is_JUMP;
				o_jump_conditional <= is_JUMP_CONDITIONAL;

				o_alu_operation <= alu_operation;
				o_alu_operand1 <= alu_operand1;
				o_alu_operand2 <= alu_operand2;

				o_memory_read <= memory_read;
				o_memory_write <= memory_write;
				o_memory_width <= memory_width;
				o_memory_signed <= memory_signed;
				
				`define OP o_op
				`include "Instructions_decode_ops.v"

				if (is_ARITHMETIC || is_COMPARE || is_COMPLEX || is_JUMP || is_JUMP_CONDITIONAL || is_MEMORY) begin
					o_tag <= i_tag;
				end
				else begin
					// Invalid or unsupported instructions end here.
					o_fault <= 1;
				end
			end
		end
	end

endmodule
