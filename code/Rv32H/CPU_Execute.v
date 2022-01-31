`include "CPU_Defines.v"

`timescale 1ns/1ns

module CPU_Execute (
	input wire i_reset,
	input wire i_clock,

	// Control
	input wire i_stall,
	input wire i_interrupt,

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
	output reg [31:0] o_mem_address,

	output reg o_jump,
	output reg [31:0] o_jump_pc,
	output reg o_irq,
	output reg [31:0] o_irq_pc,
	output wire o_stall,
	output reg o_fault
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

	// CSR registers
	`define MTVEC	mtvec
	`define MEPC	mepc
	`define MCAUSE	mcause

	`undef GOTO
	`define GOTO(ADDR) 				\
		o_jump <= 1'b1;				\
		o_jump_pc <= ADDR;
	
	`define CYCLE					\
		cycle

	`define EXECUTE_OP				\
		i_op

	`undef EXECUTE_DONE
	`define EXECUTE_DONE			\
		o_tag <= i_tag;				\
		cycle <= 0;

	// ====================
	// CSR

	reg mie_mtie;
	reg [31:0] mtvec;
	reg [31:0] mepc;
	reg [31:0] mcause;
	reg mip_mtip;

	wire [31:0] mie = { 24'b0, mie_mtie, 7'b0 };	
	wire [31:0] mip = { 24'b0, mip_mtip, 7'b0 };

	// Read CSR value referenced by instruction.
	reg [31:0] csr_rd;
	always @(*) begin
		csr_rd = 0;
		if (`IMM == `CSR_MIE)
			csr_rd = mie;
		else if (`IMM == `CSR_MTVEC)
			csr_rd = mtvec;
		else if (`IMM == `CSR_MEPC)
			csr_rd = mepc;
		else if (`IMM == `CSR_MCAUSE)
			csr_rd = mcause;
		else if (`IMM == `CSR_MIP)
			csr_rd = mip;
		else if (`IMM == `CSR_MVENDORID)
			csr_rd = 0;
		else if (`IMM == `CSR_MARCHID)
			csr_rd = 0;
		else if (`IMM == `CSR_MIMPID)
			csr_rd = 0;
		else if (`IMM == `CSR_MHARTID)
			csr_rd = 0;
	end

	// Write CSR.
	reg csr_wr_en;
	reg [31:0] csr_wr;
	always @(posedge i_clock) begin
		if (csr_wr_en) begin
			if (`IMM == `CSR_MIE) begin
				mie_mtie <= csr_wr[7];
			end
			else if (`IMM == `CSR_MTVEC)
				mtvec <= csr_wr;
		end
	end	

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

	initial begin
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
		o_irq = 0;
		o_irq_pc = 0;
		o_fault = 0;

		mie_mtie = 0;
		mtvec = 0;
		mepc = 0;
		mcause = 0;
		mip_mtip = 0;		
		csr_wr_en = 0;

		cycle = 0;
	end

	assign o_stall = (i_tag != o_tag) && i_complex;
	
	reg [4:0] cycle;

	always @(posedge i_clock) begin
		if (i_reset) begin
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
			o_irq <= 0;
			o_irq_pc <= 0;
			o_fault <= 0;

			mepc <= 0;
			mcause <= 0;
			mip_mtip <= 0;			
			csr_wr_en <= 0;

			cycle <= 0;
		end
		else begin
			o_jump <= 0;
		
			// Set interrupt pending if interrupts are enabled.
			if (i_interrupt) begin
				if (mie_mtie)
					mip_mtip <= 1;
			end

			if (!i_stall && i_tag != o_tag) begin
				o_inst_rd <= i_inst_rd;

				o_mem_address <= alu_result;
				o_mem_read <= i_memory_read;
				o_mem_write <= i_memory_write;
				o_mem_flush <= 0;
				o_mem_width <= i_memory_width;
				o_mem_signed <= i_memory_signed;

				// if (mip_mtip) begin
				// 	mepc <= i_pc;
				// 	mcause <= 32'h80000000 | (1 << 7);
				// 	o_irq <= 1;
				// 	o_irq_pc <= mtvec;
				// 	mip_mtip <= 0;
				// end

				cycle <= cycle + 1;

				if (i_arithmetic) begin
					`RD <= alu_result;
					`EXECUTE_DONE;
				end
				else if (i_compare) begin
					`RD <= { 31'b0, alu_compare_result };
					`EXECUTE_DONE;
				end
				else if (i_jump) begin
					`RD <= `PC + 4;
					`GOTO(alu_result);
					`EXECUTE_DONE;
				end
				else if (i_jump_conditional) begin
					if (alu_compare_result) begin
						`GOTO($signed(`PC) + $signed(`IMM));
					end
					else begin
						`GOTO(`PC + 4);
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
				else if (i_complex) begin
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