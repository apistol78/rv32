
`timescale 1ns/1ns

// 
// RELOAD
// 1814		44.1 kHz @ 80 MHz
// 2268		44.1 kHz @ 100 MHz
//
module AUDIO_pwm_output #(
	parameter RELOAD = 2268
)(
	input i_clock,

	output logic o_busy,
	input [15:0] i_sample,

	output o_pwm
);

	logic [15:0] reload_counter =  0;
	logic [15:0] sample = 0;
	logic [15:0] pwm_counter = 0;
	wire [15:0] pwm_counter_rev;

	initial o_busy = 0;

	always_ff @(posedge i_clock) begin
		o_busy <= 1;
		if (reload_counter > 0) begin
			reload_counter <= reload_counter - 1;
		end
		else begin
			reload_counter <= RELOAD;
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