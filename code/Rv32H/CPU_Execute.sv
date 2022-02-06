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
	`define RD			data.rd
	`define MEM_FLUSH	data.mem_flush

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

	`define EXECUTE_DONE		\
		data.tag <= i_data.tag;	\
		cycle <= 0;

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

	assign o_busy = busy || i_memory_busy;
	assign o_csr_index = i_data.imm;
	assign o_data = data;

	logic busy;
	logic [4:0] cycle = 0;
	execute_data_t data = 0;

	initial begin
		o_csr_wdata_wr = 0;
		o_csr_wdata = 0;
		o_jump = 0;
		o_jump_pc = 0;
		o_fault = 0;
	end
	
	always_comb begin
		busy = (i_data.tag != data.tag) && i_data.complx;
	end

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			cycle <= 0;
			data <= 0;
			o_csr_wdata_wr <= 0;
			o_csr_wdata <= 0;
			o_jump <= 0;
			o_jump_pc <= 0;
			o_fault <= 0;			
		end
		else begin
			if (i_data.tag != data.tag) begin
	
				o_jump <= 0;

				data.inst_rd <= i_data.inst_rd;
				data.mem_address <= alu_result;
				data.mem_read <= i_data.memory_read;
				data.mem_write <= i_data.memory_write;
				data.mem_flush <= 0;
				data.mem_width <= i_data.memory_width;
				data.mem_signed <= i_data.memory_signed;

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