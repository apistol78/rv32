
`timescale 1ns/1ns

module UART_TX #(
    parameter PRESCALE = 50000000 / (9600 * 8)
)(
	input wire i_reset,
	input wire i_clock,
	input wire i_request,
	input wire [31:0] i_wdata,
	output reg o_ready,

	output reg UART_TX
);

	reg [18:0] prescale;
	reg [8:0] data;
	reg [3:0] bit;

	initial begin
		o_ready = 0;
		prescale = 0;
		data = 0;
		bit = 0;
		UART_TX = 1;
	end

	always @(posedge i_clock) begin
		if (prescale > 0) begin
			prescale <= prescale - 1;
		end
		else if (i_request) begin
			if (bit == 0) begin
				prescale <= (PRESCALE << 3) - 1;
				bit <= 8 + 1;
				data <= { 1'b1, i_wdata };
				UART_TX <= 0;
			end
			else if (bit > 1) begin
				bit <= bit - 1;
				prescale <= (PRESCALE << 3) - 1;
				{ data, UART_TX } <= { 1'b0, data };
			end
			else if (bit == 1) begin
				bit <= bit - 1;
				prescale <= (PRESCALE << 3);
				UART_TX <= 1;
				o_ready <= 1;
			end
		end
		
		if (!i_request) begin
			bit <= 0;
			o_ready <= 0;
		end
	end

endmodule
