`include "CPU_Defines.v"

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
	input wire i_alu,
	input wire [3:0] i_alu_operation,
	input wire [2:0] i_alu_operand1,
	input wire [2:0] i_alu_operand2,
	input wire i_branch,
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
	`define PC		i_pc
	`define RS1		i_rs1
	`define RS2		i_rs2
	`define RD		o_rd
	`define IMM		i_imm
	`define ZERO	0

	`define GOTO(ADDR) 				\
		o_pc_next <= ADDR;

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
	
	`define CYCLE					\
		cycle

	`define EXECUTE_OP				\
		i_op

	`define EXECUTE_DONE			\
		o_tag <= i_tag;				\
		cycle <= 0;

	reg [7:0] cycle;
	
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
	wire [63:0] mul_result;
	CPU_Multiply multiply(
		.i_clock(i_clock),
		.i_signed(mul_signed),
		.i_op1(i_rs1),
		.i_op2(i_rs2),
		.o_result(mul_result)
	);

	reg div_signed;
	wire [31:0] div_result;
	wire [31:0] div_remainder;
	CPU_Divide divide(
		.i_clock(i_clock),
		.i_signed(div_signed),
		.i_numerator(i_rs1),
		.i_denominator(i_rs2),
		.o_result(div_result),
		.o_remainder(div_remainder)
	);

	initial begin
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

	assign o_stall = (i_tag != o_tag) && (cycle != 0);

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
				$display("Execute %x (%d)", i_instruction, i_tag);
				$display("  cycle %d", cycle);
				$display("     PC %x", i_pc);
				$display("    RS1 %x", i_rs1);
				$display("    RS2 %x", i_rs2);
				$display("    IMM %d", i_imm);

				o_pc_next <= i_pc + 4;
				o_mem_read <= 0;
				o_mem_write <= 0;
				o_mem_width <= 0;
				o_mem_signed <= 0;
				o_inst_rd <= i_inst_rd;

				cycle <= cycle + 1;

				if (i_alu) begin
					if (!i_branch) begin
						o_rd <= alu_result;
					end
					else begin
						// If ALU result non-zero then branch is
						// taken and PC is updated.
						if (alu_compare_result) begin
							`GOTO($signed(`PC) + $signed(`IMM));
						end
					end
					`EXECUTE_DONE;
				end
				else begin
					`include "Instructions_execute_ops.v"
				end
			end
		end
	end

endmodule