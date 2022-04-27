
// Since we're using DSP to implement multiplication
// we need to pipeline result so we can meet timing.

`timescale 1ns/1ns

module CPU_Divide(
	input i_clock,
	input i_signed,
	input [31:0] i_numerator,
	input [31:0] i_denominator,
	output [31:0] o_result,
    output [31:0] o_remainder
);

`ifndef __TESTBENCH__

	wire [31:0] sdiv_result;
	wire [31:0] sdiv_remainder;
    IP_Divide ip_signed_divide(
        .clock(i_clock),
        .denom(i_denominator),
        .numer(i_numerator),
        .quotient(sdiv_result),
        .remain(sdiv_remainder)
    );

	wire [31:0] udiv_result;
	wire [31:0] udiv_remainder;
	IP_UnsignedDivide ip_unsigned_divide(
        .clock(i_clock),
        .denom(i_denominator),
        .numer(i_numerator),
        .quotient(udiv_result),
        .remain(udiv_remainder)
    );
	
	assign o_result = i_signed ? sdiv_result : udiv_result;
	assign o_remainder = i_signed ? sdiv_remainder : udiv_remainder;
	
`else

    logic [31:0] p1_result;
    logic [31:0] p1_remainder;
	logic [31:0] p2_result;
    logic [31:0] p2_remainder;
	
	assign o_result = p2_result;
    assign o_remainder = p2_remainder;

    always_ff @(posedge i_clock)
    begin
        if (i_signed) begin
		    p1_result <= $signed(i_numerator) / $signed(i_denominator);
		    p1_remainder <= $signed(i_numerator) % $signed(i_denominator);
        end
        else begin
		    p1_result <= i_numerator / i_denominator;
		    p1_remainder <= i_numerator % i_denominator;
        end

        p2_result <= p1_result;
        p2_remainder <= p1_remainder;
    end

`endif

endmodule
