`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_Decode(
	input wire i_reset,
	input wire i_clock,
	output reg o_fault,

	// Input
	input i_execute_busy,
	input fetch_data_t i_data,
	
	// Output
	output decode_data_t o_data
);

	`include "private/generated/Instructions_ops.sv"

	`undef INSTRUCTION
	`define INSTRUCTION i_data.instruction
	`include "private/generated/Instructions_decode.sv"
	
	// Alias symbols for generated code.
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
	`include "private/generated/Instructions_alu.sv"
//	`include "private/generated/Instructions_fpu.sv"
	`include "private/generated/Instructions_memory.sv"

	wire [31:0] inst_B_imm = { { 20{ `INSTRUCTION[31] } }, `INSTRUCTION[7], `INSTRUCTION[30:25], `INSTRUCTION[11:8], 1'b0 };
	wire [31:0] inst_I_imm = { { 21{ `INSTRUCTION[31] } }, `INSTRUCTION[30:20] };
	wire [31:0] inst_J_imm = { { 12{ `INSTRUCTION[31] } }, `INSTRUCTION[19:12], `INSTRUCTION[20], `INSTRUCTION[30:21], 1'b0 };
	wire [31:0] inst_S_imm = { { 21{ `INSTRUCTION[31] } }, `INSTRUCTION[30:25], `INSTRUCTION[11:7] };
	wire [31:0] inst_U_imm = { `INSTRUCTION[31:12], 12'b0 };
	wire [31:0] inst_R_imm = { 26'b0, `INSTRUCTION[25:20] };
	wire [31:0] inst_CSR_imm = { 20'b0, `INSTRUCTION[31:20] };

	assign o_data = data;

	decode_data_t data = 0;

	initial begin
		o_fault = 0;
	end

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			data <= 0;
			o_fault <= 0;
		end
		else begin
			if (i_data.tag != data.tag) begin

`ifdef __VERILATOR__
				// If decode has received new transaction
				// but execute is still busy we fault.
				if (i_execute_busy)
					o_fault <= 1;
`endif
				data.pc <= i_data.pc;

				data.have_rs <= {
					i_data.inst_rs3 != 0 ? 1'b1 : 1'b0,
					i_data.inst_rs2 != 0 ? 1'b1 : 1'b0,
					i_data.inst_rs1 != 0 ? 1'b1 : 1'b0
				};

				data.inst_rs1 <= i_data.inst_rs1;
				data.inst_rs2 <= i_data.inst_rs2;
				data.inst_rs3 <= i_data.inst_rs3;
				data.inst_rd <= i_data.inst_rd;
			
				data.imm <=
					is_B ? inst_B_imm :
					is_I ? inst_I_imm :
					is_J ? inst_J_imm :
					is_S ? inst_S_imm :
					is_U ? inst_U_imm :
					is_R ? inst_R_imm :
					is_CSR ? inst_CSR_imm :
					32'h0;
				
				data.arithmetic <= is_ARITHMETIC;
				data.shift <= is_SHIFT;
				data.compare <= is_COMPARE;
				data.complx <= is_COMPLEX;
				data.jump <= is_JUMP;
				data.jump_conditional <= is_JUMP_CONDITIONAL;

				data.alu_operation <= alu_operation;
				data.alu_operand1 <= alu_operand1;
				data.alu_operand2 <= alu_operand2;

				data.memory_read <= memory_read;
				data.memory_write <= memory_write;
				data.memory_width <= memory_width;
				data.memory_signed <= memory_signed;
				
//				data.fpu <= is_FPU;
//				data.fpu_operation <= fpu_operation;

				`define OP data.op
				`include "private/generated/Instructions_decode_ops.sv"

				if (
					is_ARITHMETIC ||
					is_SHIFT ||
					is_COMPARE ||
					is_COMPLEX ||
					is_JUMP ||
					is_JUMP_CONDITIONAL ||
					is_MEMORY/* ||
					is_FPU*/
				) begin
					data.tag <= i_data.tag;
				end
				else begin
					// Invalid or unsupported instructions end here.
					o_fault <= 1;
				end
			end
		end
	end

endmodule
