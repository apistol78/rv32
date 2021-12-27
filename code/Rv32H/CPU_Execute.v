module CPU_Execute (
    input wire i_reset,
    input wire i_clock,
    input wire i_execute,

    // Input
    input wire [31:0] i_pc,
    input wire [31:0] i_instruction,
    input wire [31:0] i_rs1,
    input wire [31:0] i_rs2,
    input wire [4:0] i_inst_rd,
    input wire [31:0] i_imm,
    input wire i_branch,

    // Output
    output reg [4:0] o_inst_rd,
    output reg [31:0] o_rd,
    output reg o_branch,
    output reg [31:0] o_pc_next,
    output reg o_ready
);
    `define INSTRUCTION i_instruction
    `include "Instructions_i.v"

    `define PC i_pc
    `define RS1 i_rs1
    `define RS2 i_rs2
    `define RD o_rd
    `define IMM i_imm

    `define GOTO(ADDR) o_pc_next <= ADDR;
    `define EXECUTE_DONE
    `define ERROR

    initial begin
        o_inst_rd <= 0;
        o_branch <= 0;
        o_pc_next <= 0;
        o_ready <= 0;
    end

    always @(posedge i_clock) begin
        if (i_execute) begin
            $display("execute %x", i_instruction);
            $display("   PC %x", i_pc);
            $display("  rs1 %x", i_rs1);
            $display("  rs2 %x", i_rs2);
            $display("  imm %d", i_imm);

            o_branch <= i_branch;
            o_pc_next <= i_pc;

            `include "Instructions_d.v"

            o_inst_rd <= i_inst_rd;
            o_ready <= 1;
        end
        else begin
            o_inst_rd <= 0;
            o_ready <= 0;
        end
    end

endmodule