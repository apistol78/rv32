module CPU_Writeback(
    input wire i_reset,
    input wire i_clock,
    input wire i_stall,

    // Input
    input wire [7:0] i_tag,
    input wire [4:0] i_inst_rd,
    input wire [31:0] i_rd,
    input wire i_branch,
    input wire [31:0] i_pc_next,

    // Output
    output reg [7:0] o_tag,
    output reg [4:0] o_inst_rd,
    output reg [31:0] o_rd,
    output reg o_branch,
    output reg [31:0] o_pc_next
);

    initial begin
        o_tag <= 0;
        o_branch <= 0;
        o_pc_next <= 0;
    end

    always @(posedge i_clock) begin
        if (i_tag != o_tag) begin
            //$display("writeback, store %x in R[%d]", i_rd, i_inst_rd);
            
            o_inst_rd <= i_inst_rd;
            o_rd <= i_rd;
            o_branch <= i_branch;
            o_pc_next <= i_pc_next;
            o_tag <= i_tag;
        end
    end

endmodule