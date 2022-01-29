`include "CPU_Defines.v"

`timescale 1ns/1ns

module CPU_CSR(
	input wire i_reset,
	input wire i_clock,

	// Control
	input wire i_interrupt,

	// Input
	input wire i_request,
	input wire [31:0] i_pc,
	input wire [31:0] i_rs1,
	input wire [31:0] i_rs2,
	input wire [4:0] i_inst_rd,
	input wire [31:0] i_imm,
	input wire i_csr,
	input wire [4:0] i_op,

	// Output
	output reg o_ready,
	output reg [4:0] o_inst_rd,
	output reg [31:0] o_rd,
	output reg o_csr,
	output reg o_branch,
	output reg [31:0] o_branch_pc
);

	`include "Instructions_ops.v"

	// Alias symbols for generated code.
	`undef RS1
	`undef RD
	`undef IMM
	`define RS1		i_rs1
	`define RD		o_rd
	`define IMM		i_imm

	// CSR registers
	`define MTVEC	mtvec
	`define MEPC	mepc
	`define MCAUSE	mcause

	`define EXECUTE_OP			\
		i_op

	`undef GOTO
	`define GOTO(ADDR) 			\
		o_branch <= 1; 			\
		o_branch_pc <= ADDR;

	`undef EXECUTE_DONE
	`define EXECUTE_DONE		\
		o_ready <= 1;

	reg mie_mtie;
	reg [31:0] mtvec;
	reg [31:0] mepc;
	reg [31:0] mcause;
	reg mip_mtip;

	wire [31:0] mie =
	{
		16'b0,
		4'b0,	// Reserved
		1'b0,	// MEIE
		1'b0,	// Reserved
		1'b0,	// SEIE
		1'b0,	// Reserved
		mie_mtie,	// MTIE
		1'b0,	// Reserved
		1'b0,	// STIE
		1'b0,	// Reserved
		1'b0,	// MSIE
		1'b0,	// Reserved
		1'b0,	// SSIE
		1'b0	// Reserved
	};	

	wire [31:0] mip =
	{
		16'b0,
		4'b0,	// Reserved
		1'b0,	// MEIP
		1'b0,	// Reserved
		1'b0,	// SEIP
		1'b0,	// Reserved
		mip_mtip,	// MTIP
		1'b0,	// Reserved
		1'b0,	// STIP
		1'b0,	// Reserved
		1'b0,	// MSIP
		1'b0,	// Reserved
		1'b0,	// SSIP
		1'b0	// Reserved
	};

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
	reg csr_wr_en = 0;
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

	initial begin
		o_inst_rd = 0;
		o_ready = 0;
		o_branch = 0;
		o_branch_pc = 0;
		
		csr_wr_en = 0;

		mie_mtie = 0;
		mtvec = 0;
		mepc = 0;
		mcause = 0;
		mip_mtip = 0;
	end

	always @(posedge i_clock) begin
		o_inst_rd <= i_inst_rd;
		o_csr <= i_csr;
		o_ready <= 0;
		o_branch <= 0;

		csr_wr_en <= 0;

		if (i_interrupt) begin
			if (mie_mtie)
				mip_mtip <= 1;
		end

		if (i_request) begin
			if (mip_mtip) begin
				mepc <= i_pc;
				mcause <= 32'h80000000 | (1 << 7);
				o_branch <= 1;
				o_branch_pc <= mtvec;
				mip_mtip <= 0;
			end
			if (i_csr) begin
				`include "Instructions_execute_csr_ops.v"
			end
		end
	end

endmodule
