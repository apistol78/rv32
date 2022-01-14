
`timescale 1ns/1ns

module UART #(
    parameter PRESCALE = 50000000 / (9600 * 8)
)(
	input wire i_reset,
	input wire i_clock,

	input wire i_request,
	input wire i_rw,
	input wire [1:0] i_address,
	input wire [31:0] i_wdata,
	output wire [31:0] o_rdata,
    output wire o_ready,
	
    input wire UART_RX,
    output wire UART_TX
);

	wire rx_request;
	wire rx_ready;
	UART_RX #(
		.PRESCALE(PRESCALE)
	) rx(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(rx_request),
		.i_address(i_address),
		.o_rdata(o_rdata),
		.o_ready(rx_ready),
		.UART_RX(UART_RX)
	);

	wire tx_request;
	wire tx_ready;
	UART_TX #(
		.PRESCALE(PRESCALE)
	) tx(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(tx_request),
		.i_wdata(i_wdata),
		.o_ready(tx_ready),
		.UART_TX(UART_TX)
	);
	
	assign rx_request = i_request && !i_rw;
	assign tx_request = i_request && i_rw;
	
	assign o_ready =
		rx_request ? rx_ready :
		tx_request ? tx_ready :
		1'b0;

endmodule
