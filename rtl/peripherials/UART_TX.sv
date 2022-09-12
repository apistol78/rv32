
`timescale 1ns/1ns

module UART_TX #(
    parameter PRESCALE = 50000000 / (9600 * 8)
)(
	input i_reset,
	input i_clock,
	input i_request,
	input [31:0] i_wdata,
	output bit o_ready,

	output bit UART_TX
);
	localparam MAX_PRESCALE_VALUE = (PRESCALE << 3);

	bit [1:0] state = 0;
	bit [$clog2(MAX_PRESCALE_VALUE)-1:0] prescale = 0;
	bit [8:0] data = 0;
	bit [3:0] bidx = 0;

	// FIFO
	wire tx_fifo_empty;
	wire tx_fifo_full;
	bit tx_fifo_write = 0;
	bit tx_fifo_read = 0;
	wire [7:0] tx_fifo_rdata;
	FIFO #(
		.DEPTH(64),
		.WIDTH(8)
	) tx_fifo(
		.i_clock(i_clock),
		.o_empty(tx_fifo_empty),
		.o_full(tx_fifo_full),
		.i_write(tx_fifo_write),
		.i_wdata(i_wdata[7:0]),
		.i_read(tx_fifo_read),
		.o_rdata(tx_fifo_rdata),
		.o_queued()
	);

	initial begin
		UART_TX = 1'b1;
		o_ready = 1'b0;
	end

	// Write to FIFO.
	always_ff @(posedge i_clock) begin
		tx_fifo_write <= 0;
		if (!i_request) begin
			o_ready <= 0;
		end
		if (i_request) begin
			if (!tx_fifo_full) begin
				if (!o_ready)
					tx_fifo_write <= 1;
				o_ready <= 1;
			end
		end
	end

	// Read from FIFO and transmit each byte.
	always_ff @(posedge i_clock) begin
		case (state)
			0: begin
				if (!tx_fifo_empty) begin
					if (!tx_fifo_read)
						tx_fifo_read <= 1;
					else begin
						tx_fifo_read <= 0;
						bidx <= 0;
						state <= 1;
					end
				end
			end

			1: begin
				prescale <= (PRESCALE << 3) - 1;
				bidx <= 8+1;
				data <= { 1'b1, tx_fifo_rdata };
				UART_TX <= 0;
				state <= 2;
			end

			2: begin
				if (prescale > 0) begin
					prescale <= prescale - 1;
				end
				else begin				
					if (bidx > 0) begin
						bidx <= bidx - 1;
						prescale <= (PRESCALE << 3) - 1;
						{ data, UART_TX } <= { 1'b0, data };
					end
					else begin
						prescale <= (PRESCALE << 3);
						UART_TX <= 1;
						state <= 0;
					end
				end
			end
		endcase
	end

endmodule
