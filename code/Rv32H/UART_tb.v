
`timescale 1ns/1ns

module UART_tb(
	input wire i_reset,
	input wire i_clock,

	input wire i_request,
	input wire i_rw,
	input wire [1:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
    output reg o_ready
);

	always @(*) begin
		o_ready = i_request;
	end

	always @(posedge i_clock) begin
		if (i_request) begin
			if (!i_rw) begin
				o_rdata <= 32'h0;
			end
			else begin
				$write("%s", i_wdata[7:0]);
			end
		end
	end

endmodule