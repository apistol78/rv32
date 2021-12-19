module ALU (
    input wire [4:0] inst_I_rd,
    input wire [4:0] inst_I_rs1,
    output wire [31:0] inst_I_imm,
);

	always @ (posedge i_clock)
	begin
        r[inst_I_rd] <= $signed(r[inst_I_rs1]) + $signed(inst_I_imm);
    end

endmodule