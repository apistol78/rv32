`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_FPU_Compare(
	input [31:0] i_op1,
	input [31:0] i_op2,
	output o_less,
	output o_equal
);

	wire [22:0] a_m = { i_op1[22:0] };
	wire [22:0] b_m = { i_op2[22:0] };

	wire [9:0] a_e = { i_op1[30:23] };
	wire [9:0] b_e = { i_op2[30:23] };

	wire a_s = i_op1[31];
	wire b_s = i_op2[31];

	reg less;
	reg equal;

	assign o_less = less;
	assign o_equal = equal;

	always_comb begin
		less = 0;
		equal = 0;
		if (a_s != b_s) begin
			if (!a_s) begin
				less = 0;
			end
			else begin
				less = 1;
			end
		end
		else if (a_e != b_e) begin
			if (a_e > b_e) begin
				less = a_s;
			end
			else begin
				less = !a_s;
			end
		end
		else if (a_m != b_m) begin
			if (a_m > b_m) begin
				less = a_s;
			end
			else begin
				less = !a_s;
			end
		end
		else begin
			equal = 1;
		end
	end

endmodule
