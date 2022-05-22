
`timescale 1ns/1ns

module LED(
	input i_reset,
	input i_clock,
	input i_request,
	input [31:0] i_wdata,
	output bit o_ready,
	output [9:0] LEDR
);

	bit [9:0] leds;

	initial o_ready = 0;
	initial leds = 10'b0;
	
	assign LEDR = leds;

	always_ff @(posedge i_clock, posedge i_reset) begin
		if (i_reset) begin
			leds <= 10'b0;
		end
		else if (i_request) begin
			leds <= i_wdata[9:0];
		end
	end

	always @(posedge i_clock)
		o_ready <= i_request;

endmodule