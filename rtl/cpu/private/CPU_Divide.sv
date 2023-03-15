/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

// Since we're using DSP to implement multiplication
// we need to pipeline result so we can meet timing.

`timescale 1ns/1ns

module CPU_Divide(
	input i_clock,
	input i_latch,
	input i_signed,
	input [31:0] i_numerator,
	input [31:0] i_denominator,
	output [31:0] o_result,
	output [31:0] o_remainder
);

	wire snumerator = i_numerator[31];
	wire sdenominator = i_denominator[31];

	wire [31:0] unumerator = (i_signed && snumerator) ? -$signed(i_numerator) : i_numerator;
	wire [31:0] udenominator = (i_signed && sdenominator) ? -$signed(i_denominator) : i_denominator;

	bit [1:0] s;
	always_ff @(posedge i_clock) begin
		if (i_latch)
			s <= { snumerator, sdenominator };
	end

`ifndef __TESTBENCH__

	wire [31:0] udiv_result;
	wire [31:0] udiv_remainder;
	IP_UnsignedDivide ip_unsigned_divide(
		.clock(i_clock),
		.denom(udenominator),
		.numer(unumerator),
		.quotient(udiv_result),
		.remain(udiv_remainder)
	);

	always_comb begin
		if (i_signed) begin
			if (s[0] != s[1])
				o_result = -$signed(udiv_result);
			else
				o_result = udiv_result;

			if (s[0])
				o_remainder = -$signed(udiv_remainder);
			else
				o_remainder = udiv_remainder;
		end
		else begin
			o_result = udiv_result;
			o_remainder = udiv_remainder;
		end
	end

`else

	bit [31:0] p1_result;
	bit [31:0] p1_remainder;
	bit [31:0] p2_result;
	bit [31:0] p2_remainder;
	
	assign o_result = p2_result;
	assign o_remainder = p2_remainder;

	always_ff @(posedge i_clock)
	begin
		p1_result <= unumerator / udenominator;
		p1_remainder <= unumerator % udenominator;

		if (i_signed) begin
			if (s[0] != s[1])
				p2_result <= -$signed(p1_result);
			else
				p2_result <= p1_result;

			if (s[0])
				p2_remainder <= -$signed(p1_remainder);
			else
				p2_remainder <= p1_remainder;
		end
		else begin
			p2_result <= p1_result;
			p2_remainder <= p1_remainder;
		end
	end

`endif

endmodule
