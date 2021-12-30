module CPU_ALU(
    input wire [31:0] i_rs1,
    input wire [31:0] i_rs2,

    output wire [31:0] o_rs1_add_rs2
);

    assign o_rs1_add_rs2 = $signed(i_rs1) + $signed(i_rs2);

endmodule