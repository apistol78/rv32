
`timescale 1ns/1ns

// 
// RELOAD
// 1814		44.1 kHz @ 80 MHz
// 2268		44.1 kHz @ 100 MHz
// 4536		22050 kHz @ 100 MHz
// 5443		22050 kHz @ 120 MHz
// 5670		22050 kHz @ 125 MHz
// 9072		11025 kHz @ 100 Mhz
//
module AUDIO_pwm_output(
	input i_clock,

	output bit o_busy,
	input [15:0] i_sample,
	input [31:0] i_reload,

	output o_pwm
);

	bit [15:0] reload_counter =  0;
	bit [15:0] sample = 0;
	bit [15:0] pwm_counter = 0;
	wire [15:0] pwm_counter_rev;

	initial o_busy = 0;

	always_ff @(posedge i_clock) begin
		o_busy <= 1;
		if (reload_counter > 0) begin
			reload_counter <= reload_counter - 1;
		end
		else begin
			reload_counter <= i_reload[15:0];
			sample <= { !i_sample[15], i_sample[14:0] };
			o_busy <= 0;
		end
	end

	always_ff @(posedge i_clock) begin
		pwm_counter <= pwm_counter + 1;
	end

	genvar i;
	generate for (i = 0; i < 16; i = i + 1) begin : reverse_bits
		assign pwm_counter_rev[i] = pwm_counter[15 - i];
	end endgenerate

	always_ff @(posedge i_clock) begin
		o_pwm <= (sample >= pwm_counter_rev);
	end

endmodule
