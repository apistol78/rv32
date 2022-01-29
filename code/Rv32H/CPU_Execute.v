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
	input wire i_jump,
	input wire i_jump_conditional,
	input wire i_csr,

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
	output wire [4:0] o_inst_rd,
	output wire [31:0] o_rd,
	output wire [31:0] o_pc_next,
	output wire o_mem_read,
	output wire o_mem_write,
	output wire [2:0] o_mem_width,
	output wire o_mem_signed,
	output wire [31:0] o_mem_address,
	output wire o_branch,
	output wire [31:0] o_branch_pc,

	output wire o_stall
);

	// Request process.
	initial o_tag = 0;
	always @(posedge csr_ready or posedge ib_ready) begin
		o_tag <= i_tag;
	end
	wire request = !i_stall && (i_tag != o_tag);
	

	assign o_inst_rd = csr_csr ? csr_inst_rd : ib_inst_rd;
	assign o_rd = csr_csr ? csr_rd : ib_rd;
	assign o_pc_next = ib_pc_next;
	assign o_mem_read = ib_mem_read;
	assign o_mem_write = ib_mem_write;
	assign o_mem_width = ib_mem_width;
	assign o_mem_signed = ib_mem_signed;
	assign o_mem_address = ib_mem_address;
	assign o_branch = csr_branch;
	assign o_branch_pc = csr_branch_pc;
	assign o_stall = ib_stall;

	// =============
	// CSR

	wire csr_ready;
	wire [4:0] csr_inst_rd;
	wire [31:0] csr_rd;
	wire csr_csr;
	wire csr_branch;
	wire [31:0] csr_branch_pc;

	CPU_CSR csr(
		.i_reset(i_reset),
		.i_clock(i_clock),

		// Control
		.i_interrupt(i_interrupt),

		// Input from decode.
		.i_request(request),
		.i_pc(i_pc),
		.i_rs1(i_rs1),
		.i_rs2(i_rs2),
		.i_inst_rd(i_inst_rd),
		.i_imm(i_imm),
		.i_csr(i_csr),
		.i_op(i_op),

		// Output from CSR.
		.o_ready(csr_ready),
		.o_inst_rd(csr_inst_rd),
		.o_rd(csr_rd),
		.o_csr(csr_csr),
		.o_branch(csr_branch),
		.o_branch_pc(csr_branch_pc)
	);

	// =============
	// INTEGER/BRANCH

	wire ib_ready;
	wire [4:0] ib_inst_rd;
	wire [31:0] ib_rd;
	wire [31:0] ib_pc_next;
	wire ib_mem_read;
	wire ib_mem_write;
	wire [2:0] ib_mem_width;
	wire ib_mem_signed;
	wire [31:0] ib_mem_address;
	wire [31:0] ib_mem_wdata;
	wire ib_stall;
	
	CPU_IB execute(
		.i_reset(i_reset),
		.i_clock(i_clock),

		// Input from decode.
		.i_request(request && !i_csr),
		.i_pc(i_pc),
		.i_instruction(i_instruction),
		.i_rs1(i_rs1),
		.i_rs2(i_rs2),
		.i_inst_rd(i_inst_rd),
		.i_imm(i_imm),

		.i_arithmetic(i_arithmetic),
		.i_compare(i_compare),
		.i_jump(i_jump),
		.i_jump_conditional(i_jump_conditional),

		.i_alu_operation(i_alu_operation),
		.i_alu_operand1(i_alu_operand1),
		.i_alu_operand2(i_alu_operand2),

		.i_memory_read(i_memory_read),
		.i_memory_write(i_memory_write),
		.i_memory_width(i_memory_width),
		.i_memory_signed(i_memory_signed),

		.i_op(i_op),

		// Output from execute.
		.o_ready(ib_ready),
		.o_inst_rd(ib_inst_rd),
		.o_rd(ib_rd),
		.o_pc_next(ib_pc_next),
		.o_mem_read(ib_mem_read),
		.o_mem_write(ib_mem_write),
		.o_mem_width(ib_mem_width),
		.o_mem_signed(ib_mem_signed),
		.o_mem_address(ib_mem_address),
		.o_stall(ib_stall)
	);

endmodule