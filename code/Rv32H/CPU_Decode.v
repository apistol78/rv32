module CPU_Decode(
	input wire i_reset,
	input wire i_clock,
	input wire i_stall,

	// Input
	input wire [7:0] i_tag,
	input wire [31:0] i_instruction,
	input wire [31:0] i_pc,

	// Output
	output reg [7:0] o_tag,
	output reg [31:0] o_instruction,
	output reg [31:0] o_pc,
	output reg [4:0] o_inst_rs1,
	output reg [4:0] o_inst_rs2,
	output reg [4:0] o_inst_rd,
	output reg [31:0] o_imm,
	output reg o_branch
);

	`define INSTRUCTION i_instruction
	`include "Instructions_i.v"

	wire [31:0] inst_B_imm = { { 20{ `INSTRUCTION[31] } }, `INSTRUCTION[7], `INSTRUCTION[30:25], `INSTRUCTION[11:8], 1'b0 };
	wire [31:0] inst_I_imm = { { 21{ `INSTRUCTION[31] } }, `INSTRUCTION[30:20] };
	wire [31:0] inst_J_imm = { { 12{ `INSTRUCTION[31] } }, `INSTRUCTION[19:12], `INSTRUCTION[20], `INSTRUCTION[30:21], 1'b0 };
	wire [31:0] inst_S_imm = { { 21{ `INSTRUCTION[31] } }, `INSTRUCTION[30:25], `INSTRUCTION[11:7] };
	wire [31:0] inst_U_imm = { `INSTRUCTION[31:12], 12'b0 };

	wire have_RS1 = is_B | is_I | is_R | is_S;
	wire have_RS2 = is_B | is_R | is_S;
	wire have_RD  = is_I | is_J | is_R | is_U;

	initial begin
		o_tag <= 0;
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			o_tag <= 0;
		end
		else begin
			if (!i_stall && i_tag != o_tag) begin
				//$display("decode instruction %x", i_instruction);

				o_instruction <= i_instruction;
				o_pc <= i_pc;

				o_inst_rs1 <= have_RS1 ? `INSTRUCTION[19:15] : 5'h0;
				o_inst_rs2 <= have_RS2 ? `INSTRUCTION[24:20] : 5'h0;
				o_inst_rd  <= have_RD  ? `INSTRUCTION[11:7]  : 5'h0;            

				o_imm <=
					is_B ? inst_B_imm :
					is_I ? inst_I_imm :
					is_J ? inst_J_imm :
					is_S ? inst_S_imm :
					is_U ? inst_U_imm :
					32'h0;

				o_branch <= is_BRANCH;

				o_tag <= i_tag;
			end
		end
	end

endmodule
