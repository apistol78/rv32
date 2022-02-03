`include "CPU_Defines.v"

`timescale 1ns/1ns

module CPU_Execute (
	input wire i_reset,
	input wire i_clock,

	// CSR
	output wire [11:0] o_csr_index,
	input wire [31:0] i_csr_rdata,
	output reg o_csr_wdata_wr,
	output reg [31:0] o_csr_wdata,
	input wire [31:0] i_epc,

	// Control
	input wire i_memory_busy,
	input wire i_decode_valid,
	output reg o_busy,
	output reg o_valid,
	output reg o_jump,
	output reg [31:0] o_jump_pc,
	output reg o_fault,

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
	input wire i_complex,
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
	output reg o_mem_read,
	output reg o_mem_write,
	output reg o_mem_flush,
	output reg [2:0] o_mem_width,
	output reg o_mem_signed,
	output reg [31:0] o_mem_address
);

	`include "Instructions_ops.v"

	// Alias symbols for generated code.
	`undef PC
	`undef RS1
	`undef RS2
	`undef RD
	`undef IMM
	`undef ZERO
	`define PC		i_pc
	`define RS1		i_rs1
	`define RS2		i_rs2
	`define RD		o_rd
	`define IMM		i_imm
	`define ZERO	0

	`undef GOTO
	`define GOTO(ADDR) 		\
		o_jump <= 1'b1;		\
		o_jump_pc <= ADDR;

	`define MEPC 			\
		i_epc

	`define CYCLE			\
		cycle

	`define EXECUTE_OP		\
		i_op

	`define EXECUTE_DONE	\
		busy <= 1;			\
		o_valid <= 1;		\
		cycle <= 0;

	// ====================
	// CSR

	assign o_csr_index = i_imm;

	// ====================
	// ALU
	
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

	// ====================
	// MUL/DIV

	wire mul_signed = (`EXECUTE_OP == OP_MUL) || (`EXECUTE_OP == OP_MULH);
	wire [63:0] mul_result;
	CPU_Multiply multiply(
		.i_clock(i_clock),
		.i_signed(mul_signed),
		.i_op1(`RS1),
		.i_op2(`RS2),
		.o_result(mul_result)
	);

	wire div_signed = (`EXECUTE_OP == OP_DIV) || (`EXECUTE_OP == OP_REM);
	wire [31:0] div_result;
	wire [31:0] div_remainder;
	CPU_Divide divide(
		.i_clock(i_clock),
		.i_signed(div_signed),
		.i_numerator(`RS1),
		.i_denominator(`RS2),
		.o_result(div_result),
		.o_remainder(div_remainder)
	);

	// ====================

	reg busy = 0;
	reg [4:0] cycle = 0;

	initial begin
		o_valid = 0;
		o_tag = 0;
		o_inst_rd = 0;
		o_mem_read = 0;
		o_mem_write = 0;
		o_mem_flush = 0;
		o_mem_width = 0;
		o_mem_signed = 0;
		o_mem_address = 0;
		o_jump = 0;
		o_jump_pc = 0;
		o_fault = 0;
	end
	
	always @(*) begin
		o_busy = busy || i_memory_busy;
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			o_valid <= 0;
			o_tag <= 0;
			o_inst_rd <= 0;
			o_mem_read <= 0;
			o_mem_write <= 0;
			o_mem_flush <= 0;
			o_mem_width <= 0;
			o_mem_signed <= 0;
			o_mem_address <= 0;
			o_jump <= 0;
			o_jump_pc <= 0;
			o_fault <= 0;

			busy <= 0;
			cycle <= 0;
		end
		else begin

			busy <= 0;
			o_valid <= 0;

			if ((i_decode_valid && !i_memory_busy) || cycle > 0) begin
				
				o_valid <= 1;
				o_jump <= 0;
				o_tag <= i_tag;

				o_inst_rd <= i_inst_rd;

				o_mem_address <= alu_result;
				o_mem_read <= i_memory_read;
				o_mem_write <= i_memory_write;
				o_mem_flush <= 0;
				o_mem_width <= i_memory_width;
				o_mem_signed <= i_memory_signed;

				if (i_arithmetic) begin
					`RD <= alu_result;
				end
				else if (i_compare) begin
					`RD <= { 31'b0, alu_compare_result };
				end
				else if (i_jump) begin
					`RD <= `PC + 4;
					`GOTO(alu_result);
				end
				else if (i_jump_conditional) begin
					if (alu_compare_result) begin
						`GOTO($signed(`PC) + $signed(`IMM));
					end
					else begin
						`GOTO(`PC + 4);
					end
				end
				else if (i_memory_read) begin
				end
				else if (i_memory_write) begin
					`RD <=`RS2;
				end
				else if (i_complex) begin
					busy <= 1;
					o_valid <= 0;
					cycle <= cycle + 1;

					// Note, input values are only valid in first cycle so
					// in case of multicycle operations the inputs must be
					// stored in temporary registers.
					`include "Instructions_execute_ops.v"
				end
				else begin
					// Invalid condition, should fault since it's
					// most probably a bug.
					o_fault <= 1;
				end
			end
		end
	end

endmodule