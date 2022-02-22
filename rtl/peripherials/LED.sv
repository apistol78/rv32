
`timescale 1ns/1ns

module LED(
	input wire i_reset,
	input wire i_clock,
	input wire i_request,
	input wire [31:0] i_wdata,
	output reg o_ready,
	output wire [9:0] LEDR
);

	reg [9:0] leds;

	initial o_ready = 0;
	initial leds = 10'b1010101010;
	
	assign LEDR = leds;

	always @(posedge i_clock, posedge i_reset) begin
		if (i_reset) begin
			leds <= 10'b0101010101;
		end
		else if (i_request) begin
			leds <= i_wdata[9:0];
		end
	end

	always @(posedge i_clock)
		o_ready <= i_request;

endmodule