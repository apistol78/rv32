`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_Execute (
	input wire i_reset,
	input wire i_clock,
	output reg o_fault,

	// CSR
	output wire [11:0] o_csr_index,
	input wire [31:0] i_csr_rdata,
	output reg o_csr_wdata_wr,
	output reg [31:0] o_csr_wdata,
	input wire [31:0] i_epc,

	// Control
	output reg o_jump,
	output reg [31:0] o_jump_pc,

	// Input
	output o_busy,
	input decode_data_t i_data,
	input wire [31:0] i_rs1,
	input wire [31:0] i_rs2,
	
	// Output
	input i_memory_busy,
	output execute_data_t o_data
);

	`include "Instructions_ops.sv"

	// Alias symbols for generated code.
	`undef PC
	`undef RS1
	`undef RS2
	`undef IMM
	`undef ZERO
	`define PC			i_data.pc
	`define RS1			i_rs1
	`define RS2			i_rs2
	`define IMM			i_data.imm
	`define ZERO		0

	`undef RD
	`undef MEM_FLUSH
	`define RD			dataC.rd
	`define MEM_FLUSH	dataC.mem_flush

	`undef GOTO
	`define GOTO(ADDR) 		\
		o_jump <= 1'b1;		\
		o_jump_pc <= ADDR;

	`define MEPC 			\
		i_epc

	`define CYCLE			\
		cycle

	`define EXECUTE_OP		\
		i_data.op

	`define EXECUTE_DONE			\
		dataC.tag <= i_data.tag;	\
		busy <= 0;					\
		cycle <= 0;

	// ====================
	// CSR

	assign o_csr_index = i_data.imm;

	// ====================
	// ALU
	
	wire [31:0] alu_operand1 =
		(i_data.alu_operand1 == 3'd0) ? `ZERO :
		(i_data.alu_operand1 == 3'd1) ? `RS1 :
		(i_data.alu_operand1 == 3'd2) ? `RS2 :
		(i_data.alu_operand1 == 3'd3) ? `PC  :
		(i_data.alu_operand1 == 3'd4) ? `IMM :
		32'd0;

	wire [31:0] alu_operand2 =
		(i_data.alu_operand2 == 3'd0) ? `ZERO :
		(i_data.alu_operand2 == 3'd1) ? `RS1 :
		(i_data.alu_operand2 == 3'd2) ? `RS2 :
		(i_data.alu_operand2 == 3'd3) ? `PC  :
		(i_data.alu_operand2 == 3'd4) ? `IMM :
		32'd0;

	wire [31:0] alu_result;
	wire alu_compare_result;
	CPU_ALU alu(
		.i_op(i_data.alu_operation),
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

	logic busy = 0;
	logic [4:0] cycle = 0;
	execute_data_t dataC = 0;
	execute_data_t dataN = 0;

	assign o_busy = busy || i_memory_busy;
	assign o_data = !i_memory_busy ? dataC : dataN;
	
	initial begin
		o_csr_wdata_wr = 0;
		o_csr_wdata = 0;
		o_jump = 0;
		o_jump_pc = 0;
		o_fault = 0;
	end
	
	always_ff @(posedge i_clock) begin
		if (i_reset)
			dataN <= 0;
		else if (!i_memory_busy)
			dataN <= dataC;
	end

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			busy <= 0;
			cycle <= 0;
			dataC <= 0;
		end
		else begin
			busy <= 0;
			if (i_data.tag != dataC.tag) begin
	
				o_jump <= 0;

				dataC.inst_rd <= i_data.inst_rd;

				dataC.mem_address <= alu_result;
				dataC.mem_read <= i_data.memory_read;
				dataC.mem_write <= i_data.memory_write;
				dataC.mem_flush <= 0;
				dataC.mem_width <= i_data.memory_width;
				dataC.mem_signed <= i_data.memory_signed;

				if (i_data.arithmetic) begin
					`RD <= alu_result;
					`EXECUTE_DONE;
				end
				else if (i_data.compare) begin
					`RD <= { 31'b0, alu_compare_result };
					`EXECUTE_DONE;
				end
				else if (i_data.jump) begin
					`RD <= `PC + 4;
					`GOTO(alu_result);
					`EXECUTE_DONE;
				end
				else if (i_data.jump_conditional) begin
					if (alu_compare_result) begin
						`GOTO($signed(`PC) + $signed(`IMM));
					end
					else begin
						`GOTO(`PC + 4);
					end
					`EXECUTE_DONE;
				end
				else if (i_data.memory_read) begin
					`EXECUTE_DONE;
				end
				else if (i_data.memory_write) begin
					`RD <=`RS2;
					`EXECUTE_DONE;
				end
				else if (i_data.complx) begin
					busy <= 1;
					cycle <= cycle + 1;

					// Note, input values are only valid in first cycle so
					// in case of multicycle operations the inputs must be
					// stored in temporary registers.
					`include "Instructions_execute_ops.sv"
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