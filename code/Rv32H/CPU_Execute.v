module CPU_Execute (
    input wire i_reset,
    input wire i_clock,
    input wire i_stall,

    // Input
    input wire [7:0] i_tag,
    input wire [31:0] i_pc,
    input wire [31:0] i_instruction,
    input wire [31:0] i_rs1,
    input wire [31:0] i_rs2,
    input wire [4:0] i_inst_rd,
    input wire [31:0] i_imm,
    input wire i_branch,

    // Output
    output reg [7:0] o_tag,

    output reg [4:0] o_inst_rd,
    output reg [31:0] o_rd,

    output reg o_branch,
    output reg [31:0] o_pc_next,

    output reg o_mem_read,
    output reg o_mem_write,
    output reg [31:0] o_mem_address
);
    `define INSTRUCTION i_instruction
    `include "Instructions_i.v"

    `define PC i_pc
    `define RS1 i_rs1
    `define RS2 i_rs2
    `define RD o_rd
    `define IMM i_imm

    `define GOTO(ADDR) o_pc_next <= ADDR;
    `define MEM_READ(ADDR) o_mem_address <= ADDR; o_mem_read <= 1;
    `define MEM_WRITE(ADDR, DATA) o_mem_address <= ADDR; o_rd <= DATA; o_mem_write <= 1;
    `define EXECUTE_DONE
    `define ERROR

    initial begin
        o_inst_rd <= 0;
        o_branch <= 0;
        o_pc_next <= 0;
        o_mem_read <= 0;
        o_mem_write <= 0;
        o_mem_address <= 0;
        o_tag <= 0;
    end

    always @(posedge i_clock) begin
        if (!i_stall && i_tag != o_tag) begin
            $display("execute %x", i_instruction);
            $display("   PC %x", i_pc);
            $display("  rs1 %x", i_rs1);
            $display("  rs2 %x", i_rs2);
            $display("  imm %d", i_imm);

            o_branch <= i_branch;
            o_pc_next <= i_pc;
            o_mem_read <= 0;
            o_mem_write <= 0;

            `include "Instructions_d.v"

            o_tag <= i_tag;
            o_inst_rd <= i_inst_rd;
        end
        // else begin
        //     o_inst_rd <= 0;
        //     o_mem_read <= 0;
        //     o_mem_write <= 0;
        // end
    end

endmodule