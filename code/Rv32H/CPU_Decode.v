module CPU_Decode(
    input wire i_reset,
    input wire i_clock,
    input wire i_execute,

    // Input
    input wire [31:0] i_instruction,
    input wire [31:0] i_pc,

    // Output
    output wire [31:0] o_instruction,
    output wire [31:0] o_pc,
    output wire [4:0] o_inst_rs1,
    output wire [4:0] o_inst_rs2,
    output wire [4:0] o_inst_rd,
    output wire [31:0] o_imm,
    output wire o_branch,
    output reg o_ready
);

    reg [31:0] instruction = 0;
    reg [31:0] pc = 0;

    `define INSTRUCTION instruction
    `include "Instructions_i.v"

	wire [31:0] inst_B_imm = { { 20{ instruction[31] } }, instruction[7], instruction[30:25], instruction[11:8], 1'b0 };
	wire [31:0] inst_I_imm = { { 21{ instruction[31] } }, instruction[30:20] };
	wire [31:0] inst_J_imm = { { 12{ instruction[31] } }, instruction[19:12], instruction[20], instruction[30:21], 1'b0 };
	wire [31:0] inst_S_imm = { { 21{ instruction[31] } }, instruction[30:25], instruction[11:7] };
	wire [31:0] inst_U_imm = { instruction[31:12], 12'b0 };

    assign o_instruction = instruction;
    assign o_pc = pc;

    wire have_RS1 = is_B | is_I | is_R | is_S;
    wire have_RS2 = is_B | is_R | is_S;
    wire have_RD  = is_I | is_J | is_R | is_U;

	assign o_inst_rs1 = have_RS1 ? instruction[19:15] : 5'h0;
	assign o_inst_rs2 = have_RS2 ? instruction[24:20] : 5'h0;
	assign o_inst_rd  = have_RD  ? instruction[11:7]  : 5'h0;

    assign o_imm =
        is_B ? inst_B_imm :
        is_I ? inst_I_imm :
        is_J ? inst_J_imm :
        is_S ? inst_S_imm :
        is_U ? inst_U_imm :
        32'h0;

    assign o_branch = is_BRANCH;

    initial begin
        o_ready <= 0;
    end

    always @(posedge i_clock) begin
        if (i_execute) begin
            $display("decode instruction %x", i_instruction);

            instruction <= i_instruction;
            pc <= i_pc;

            o_ready <= 1;
        end
        else begin
            o_ready <= 0;
        end
    end

endmodule
