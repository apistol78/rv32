module CPU_Execute (
    input wire i_reset,
    input wire i_clock,

    input wire i_execute,

    input wire [31:0] i_instruction,
    input wire [31:0] i_rs1,
    input wire [31:0] i_rs2,

    output reg o_ready
);

    always @(posedge i_clock) begin
        if (i_execute) begin
            $display("execute %x", i_instruction);
            $display("  rs1 %x", i_rs1);
            $display("  rs2 %x", i_rs2);
        end
    end

endmodule