
// Since we're using DSP to implement multiplication
// we need to pipeline result so we can meet timing.

`timescale 1ns/1ns

module CPU_Multiply(
	input wire i_clock,
	input wire i_signed,
	input wire [31:0] i_op1,
	input wire [31:0] i_op2,
	output wire [63:0] o_result
);

`ifndef __TESTBENCH__

	wire [63:0] smul_result;
    IP_Multiply ip_signed_multiply(
        .clock(i_clock),
        .dataa(i_op1),
        .datab(i_op2),
        .result(smul_result)
    );
	
	wire [63:0] umul_result;
    IP_UnsignedMultiply ip_unsigned_multiply(
        .clock(i_clock),
        .dataa(i_op1),
        .datab(i_op2),
        .result(umul_result)
    );
	
	assign o_result = i_signed ? smul_result : umul_result;

`else

    reg [63:0] p1;
	reg [63:0] p2;
	
	assign o_result = p2;

    always @(posedge i_clock)
    begin
        if (i_signed)
            p1 <= $signed(i_op1) * $signed(i_op2);
        else
            p1 <= { 32'b0, i_op1 } * { 32'b0, i_op2 };

        p2 <= p1;
    end

`endif

endmodule
