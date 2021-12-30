`include "CPU_Defines.v"

module CPU_Execute (
	input wire i_reset,
	input wire i_clock,
	input wire i_stall,

	// Input
	input wire [7:0] i_tag,
	input wire [31:0] i_pc,
	input wire [31:0] i_instruction,
	input wire [31:0] i_rs1,
	input wire [31:0] i_rs2,
	input wire [4:0] i_inst_rd,
	input wire [31:0] i_imm,
	input wire i_branch,

	// Output
	output reg [7:0] o_tag,

	output reg [4:0] o_inst_rd,
	output reg [31:0] o_rd,

	output reg o_branch,
	output reg [31:0] o_pc_next,

	output reg o_mem_read,
	output reg o_mem_write,
	output reg [2:0] o_mem_width,
	output reg o_mem_signed,
	output reg [31:0] o_mem_address
);
	`define INSTRUCTION i_instruction
	`include "Instructions_i.v"

	// Alias symbols for generated code.
	`define PC i_pc
	`define RS1 i_rs1
	`define RS2 i_rs2
	`define RD o_rd
	`define IMM i_imm
	`define ZERO 0

	`define GOTO(ADDR) o_pc_next <= ADDR;

	`define MEM_READ_W(ADDR)		\
		o_mem_address <= ADDR;		\
		o_mem_read <= 1;			\
		o_mem_width <= 4;			\
		o_mem_signed <= 0;

	`define MEM_READ_UH(ADDR)		\
		o_mem_address <= ADDR;		\
		o_mem_read <= 1;			\
		o_mem_width <= 2;			\
		o_mem_signed <= 0;

	`define MEM_READ_SH(ADDR)		\
		o_mem_address <= ADDR;		\
		o_mem_read <= 1;			\
		o_mem_width <= 2;			\
		o_mem_signed <= 1;

	`define MEM_READ_UB(ADDR)		\
		o_mem_address <= ADDR;		\
		o_mem_read <= 1;			\
		o_mem_width <= 1;			\
		o_mem_signed <= 0;

	`define MEM_READ_SB(ADDR)		\
		o_mem_address <= ADDR;		\
		o_mem_read <= 1;			\
		o_mem_width <= 1;			\
		o_mem_signed <= 1;

	`define MEM_WRITE_W(ADDR, DATA)	\
		o_mem_address <= ADDR;		\
		o_mem_write <= 1;			\
		o_mem_width <= 4;			\
		o_rd <= DATA;

	`define MEM_WRITE_H(ADDR, DATA)	\
		o_mem_address <= ADDR;		\
		o_mem_write <= 1;			\
		o_mem_width <= 2;			\
		o_rd <= DATA;

	`define MEM_WRITE_B(ADDR, DATA)	\
		o_mem_address <= ADDR;		\
		o_mem_write <= 1;			\
		o_mem_width <= 1;			\
		o_rd <= DATA;

	`define EXECUTE_DONE
	`define ERROR

	`include "Instructions_alu.v"

	wire [31:0] alu_result;
	CPU_ALU alu(
		.i_op(alu_operation),
		.i_op1(alu_operand1),
		.i_op2(alu_operand2),
		.o_result(alu_result)
	);

	initial begin
		o_inst_rd <= 0;
		o_branch <= 0;
		o_pc_next <= 0;
		o_mem_read <= 0;
		o_mem_write <= 0;
		o_mem_signed <= 0;
		o_mem_address <= 0;
		o_tag <= 0;
	end

	always @(posedge i_clock) begin
		if (!i_stall && i_tag != o_tag) begin
			$display("Execute %x (%d)", i_instruction, i_tag);
			$display("   PC %x", i_pc);
			$display("  RS1 %x", i_rs1);
			$display("  RS2 %x", i_rs2);
			$display("  IMM %d", i_imm);

			o_branch <= i_branch;
			o_pc_next <= i_pc + 4;
			o_mem_read <= 0;
			o_mem_write <= 0;
			o_mem_width <= 0;
			o_inst_rd <= i_inst_rd;

			if (is_ALU) begin
				if (!i_branch) begin
					o_rd <= alu_result;
				end
				else begin
					// If ALU result non-zero then branch is
					// taken and PC is updated.
					if (alu_result) begin
						`GOTO($signed(`PC) + $signed(`IMM));
					end
				end
			end
			else begin
				`include "Instructions_d.v"
			end

			o_tag <= i_tag;
		end
	end

endmodule