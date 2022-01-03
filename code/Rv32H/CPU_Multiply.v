
// Since we're using DSP to implement multiplication
// we need to pipeline result so we can meet timing.

module CPU_Multiply(
	input wire i_clock,
	input wire [31:0] i_op1,
	input wire [31:0] i_op2,
	output wire [63:0] o_result
);

`ifndef __ICARUS__

    IP_Multiply ip_multiply(
        .clock(i_clock),
        .dataa(i_op1),
        .datab(i_op2),
        .result(o_result)
    );

`else

    reg [63:0] p1;
	reg [63:0] p2;
	
	assign o_result = p2;

    always @(posedge i_clock)
    begin
        p1 <= i_op1 * i_op2;
        p2 <= p1;
    end

`endif

endmodule
