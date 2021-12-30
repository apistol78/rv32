`include "CPU_Defines.v"

module CPU_ALU(
    input wire [3:0] i_op,

    input wire [31:0] i_op1,
    input wire [31:0] i_op2,

    output wire [31:0] o_result
);

    wire [31:0] signed_sum = $signed(i_op1) + $signed(i_op2);
    wire [31:0] unsigned_sum = i_op1 + i_op2;

    wire [31:0] and_result = i_op1 & i_op2;
    wire [31:0] or_result = i_op1 | i_op2;

    assign o_result =
        i_op == `OP_SIGNED_ADD ? signed_sum :
        i_op == `OP_UNSIGNED_ADD ? unsigned_sum :
        i_op == `OP_AND ? and_result :
        i_op == `OP_OR ? or_result :
        32'h0;

endmodule
