module CPU_Decode(
    input wire i_reset,
    input wire i_clock,

    input wire i_decode,
    input wire [31:0] i_instruction,

    output wire [31:0] o_instruction,
    output wire [4:0] o_inst_rs1,
    output wire [4:0] o_inst_rs2,
    output wire [4:0] o_inst_rd,
    output reg o_decoded
);

    reg [31:0] instruction;


    `include "Instructions_i.v"

    assign o_instruction = instruction;
	assign o_inst_rs1 = instruction[19:15];
	assign o_inst_rs2 = instruction[24:20];
	assign o_inst_rd  = instruction[11:7];

/*
	// Instruction formats
	wire [31:0] inst_B_imm = { { 20{ instruction[31] } }, instruction[7], instruction[30:25], instruction[11:8], 1'b0 };
	wire [31:0] inst_I_imm = { { 21{ instruction[31] } }, instruction[30:20] };
	wire [31:0] inst_J_imm = { { 12{ instruction[31] } }, instruction[19:12], instruction[20], instruction[30:21], 1'b0 };
	wire [31:0] inst_S_imm = { { 21{ instruction[31] } }, instruction[30:25], instruction[11:7] };
	wire [31:0] inst_U_imm = { instruction[31:12], 12'b0 };
*/

    initial begin
        o_decoded <= 0;
    end

    always @(posedge i_clock) begin
        
        if (i_decode) begin
            $display("decode instruction %x", i_instruction);

            instruction <= i_instruction;

            o_decoded <= 1;
        end
        else begin
            o_decoded <= 0;
        end


    end


endmodule
