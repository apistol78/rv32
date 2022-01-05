
module UART #(
    parameter CLOCK_RATE = 50000000,
    parameter BAUD_RATE = 9600
)(
	input wire i_reset,
	input wire i_clock,

	input wire i_request,
	input wire i_rw,
	input wire [31:0] i_wdata,
	output wire [31:0] o_rdata,
    output wire o_ready,
	
	output wire [1:0] o_state,
	output wire [7:0] o_sample,

    input wire UART_RX,
    output wire UART_TX
);

	wire rx_request;
	wire rx_ready;
	UART_RX #(
		.CLOCK_RATE(CLOCK_RATE),
		.BAUD_RATE(BAUD_RATE)
	) rx(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(rx_request),
		.o_rdata(o_rdata),
		.o_ready(rx_ready),
		
		.o_state(o_state),
		.o_sample(o_sample),
		
		.UART_RX(UART_RX)
	);

	wire tx_request;
	wire tx_ready;
	UART_TX #(
		.CLOCK_RATE(CLOCK_RATE),
		.BAUD_RATE(BAUD_RATE)
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
