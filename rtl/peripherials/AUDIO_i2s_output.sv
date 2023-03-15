/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

`timescale 1ns/1ns

module AUDIO_i2s_output #(
	parameter FREQUENCY
)(
	input i_clock,

	output bit o_busy,
	input [15:0] i_sample,

	output bit o_i2s_sdout,
	output bit o_i2s_sclk,
	output bit o_i2s_lrck,
	output bit o_i2s_mclk
);

	wire mclk;

	ClockDivider #(
		.CLOCK_RATE(FREQUENCY),
		.BAUD_RATE(12_288_000)
	) mclk_div(
		.i_reset(1'b0),
		.i_clock(i_clock),
		.o_clock(mclk)
	);

	wire sclk;

	ClockDivider #(
		.CLOCK_RATE(18), 
		.BAUD_RATE(3)
	) sclk_div(
		.i_reset(1'b0),
		.i_clock(mclk),
		.o_clock(sclk)
	);

	assign o_i2s_mclk = mclk;
	assign o_i2s_sclk = sclk;
	assign o_i2s_lrck = lrck;
	assign o_i2s_sdout = sdout;

	bit [1:0] sclk_pp = 2'b0;
	bit [4:0] bitcnt = 0;
	bit lrck = 0;
	bit [15:0] sample = 0;
	bit sdout = 0;

	always_ff @(posedge i_clock) begin
		sclk_pp <= { sclk_pp[0], sclk };
		o_busy <= 1;
		if (sclk_pp == 2'b10) begin
			sdout <= sample[15];
			sample <= { sample[14:0], 1'b0 };
			bitcnt <= bitcnt + 1;
			if (bitcnt >= 16-1) begin
				lrck <= ~lrck;
				bitcnt <= 0;
				sample <= i_sample;
				o_busy <= 0;
			end
		end
	end

endmodule
