
// Since we're using DSP to implement multiplication
// we need to pipeline result so we can meet timing.

module CPU_Divide(
	input wire i_clock,
	input wire [31:0] i_numerator,
	input wire [31:0] i_denominator,
	output wire [31:0] o_result,
    output wire [31:0] o_remainder
);

`ifndef __ICARUS__

    IP_Divide ip_divide(
        .clock(i_clock),
        .denom(i_denominator),
        .numer(i_numerator),
        .quotient(o_result),
        .remain(o_remainder)
    );

`else

    reg [31:0] p1_result;
    reg [31:0] p1_remainder;
	reg [31:0] p2_result;
    reg [31:0] p2_remainder;
	
	assign o_result = p2_result;
    assign o_remainder = p2_remainder;

    always @(posedge i_clock)
    begin
        p1_result <= i_numerator / i_denominator;
        p1_remainder <= i_numerator % i_denominator;
        p2_result <= p1_result;
        p2_remainder <= p1_remainder;
    end

`endif

endmodule
