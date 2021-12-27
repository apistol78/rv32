module CPU_Memory(
    input wire i_reset,
    input wire i_clock,
    input wire i_execute,

    // Input
    input wire [4:0] i_inst_rd,
    input wire [31:0] i_rd,
    input wire i_branch,
    input wire [31:0] i_pc_next,

    // Output
    output reg [4:0] o_inst_rd,
    output reg [31:0] o_rd,
    output reg o_branch,
    output reg [31:0] o_pc_next,
    output reg o_ready
);

    initial begin
        o_inst_rd <= 0;
        o_rd <= 0;
        o_branch <= 0;
        o_pc_next <= 0;
        o_ready <= 0;
    end

    always @(posedge i_clock) begin
        if (i_execute) begin
            o_inst_rd <= i_inst_rd;
            o_rd <= i_rd;
            o_branch <= i_branch;
            o_pc_next <= i_pc_next;
            o_ready <= 1;
        end
        else begin
            o_ready <= 0;
        end
    end

endmodule
