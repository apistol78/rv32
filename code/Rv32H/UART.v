
module UART #(
    parameter CLOCK_RATE = 50000000,
    parameter BAUD_RATE = 9600
)(
	input wire i_clock,
	input wire i_enable,
	input wire i_rw,
	input wire [31:0] i_wdata,
	output wire [31:0] o_rdata,
    output wire o_ready,
	output wire o_waiting,

    input wire UART_RX,
    output wire UART_TX
);

	wire rx_enable;
	wire rx_ready;
	UART_RX #(
		.CLOCK_RATE(CLOCK_RATE),
		.BAUD_RATE(BAUD_RATE)
	) rx(
		.i_clock(i_clock),
		.i_enable(rx_enable),
		.o_rdata(o_rdata),
		.o_ready(rx_ready),
		.o_waiting(o_waiting),
		.UART_RX(UART_RX)
	);

	wire tx_enable;
	wire tx_ready;
	UART_TX #(
		.CLOCK_RATE(CLOCK_RATE),
		.BAUD_RATE(BAUD_RATE)
	) tx(
		.i_clock(i_clock),
		.i_enable(tx_enable),
		.i_wdata(i_wdata),
		.o_ready(tx_ready),
		.UART_TX(UART_TX)
	);
	
	assign rx_enable = i_enable && !i_rw;
	assign tx_enable = i_enable && i_rw;
	
	assign o_ready =
		tx_enable ? tx_ready :
		rx_enable ? rx_ready :
		1'b0;

endmodule
