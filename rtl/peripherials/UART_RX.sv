
`timescale 1ns/1ns

module UART_RX #(
    parameter PRESCALE = 50000000 / (9600 * 8)
)(
	input wire i_reset,
	input wire i_clock,

	input wire i_request,
	input wire [1:0] i_address,
	output reg [31:0] o_rdata,
    output wire o_ready,
	
    input wire UART_RX
);
	reg frame_error;
	reg [18:0] prescale;
	reg [7:0] data;
	reg [3:0] bidx;
	reg [3:0] rds;
	reg rx;
	
	// FIFO
	wire rx_fifo_empty;
	//wire rx_fifo_full;
	reg rx_fifo_write = 0;
	reg rx_fifo_read = 0;
	wire [7:0] rx_fifo_rdata;
	FIFO #(
		.DEPTH(4)
	) rx_fifo(
		.i_clock(i_clock),
		.o_empty(rx_fifo_empty),
		.o_full(/*rx_fifo_full*/),
		.i_write(rx_fifo_write),
		.i_wdata(data),
		.i_read(rx_fifo_read),
		.o_rdata(rx_fifo_rdata)
	);
	
	initial begin
		frame_error = 0;
		prescale = 0;
		data = 0;
		bidx = 0;
		rds = 0;
		rx = 0;
		o_rdata = 32'h0;
	end
	
	assign o_ready = (rds == 5) && i_request;
	
	// Read from FIFO.
	always @(posedge i_clock) begin
		if (i_reset) begin
			rds <= 0;
			o_rdata <= 32'h0;
		end
		else begin
			if (i_request) begin
				if (i_address == 2'h0) begin	// Read byte from fifo.
					case (rds)
						0: begin
							if (!rx_fifo_empty) begin
								rx_fifo_read <= 1;
								rds <= 1;
							end
						end
						1, 2, 3: begin
							// Need to wait a couple of cycles to ensure valid data out from fifo
							// because empty is signaled before values has been latched into queue.
							o_rdata <= { 24'b0, rx_fifo_rdata };
							rds <= rds + 1;
						end
						4: begin
							rds <= 5;
						end
					endcase
				end
				else if (i_address == 2'h1) begin	// Read status.
					o_rdata <= { 30'b0, rx_fifo_empty, 1'b0 };
					rds <= 5;
				end
			end
			else begin
				rx_fifo_read <= 0;
				rds <= 0;
			end
		end
	end	
	
	// Receive and put into FIFO.
	always @(posedge i_clock) begin
		frame_error <= 0;

		rx_fifo_write <= 0;
		rx <= UART_RX;
	
		if (prescale > 0) begin
			prescale <= prescale - 1;
		end
		else if (bidx > 0) begin
			if (bidx > 8 + 1) begin
				if (!rx) begin
					// Assume mid point of start bit,
					// continue with data bits.
					bidx <= bidx - 1;
					prescale <= (PRESCALE << 3) - 1;
				end
				else begin
					// Unexpected end of start bit.
					bidx <= 0;
					prescale <= 0;
				end
			end
			else if (bidx > 1) begin
				// Shift in data bits.
				bidx <= bidx - 1;
				prescale <= (PRESCALE << 3) - 1;
				data <= { rx, data[8 - 1:1] };
			end
			else if (bidx == 1) begin
				bidx <= bidx - 1;
				if (rx) begin
					// Stop bit found, save data into fifo.
					rx_fifo_write <= 1;
				end
				else begin
					// Stop bit expected.
					frame_error <= 1;
				end
			end
		end
		else begin
			// Wait for start bit.
			if (!rx) begin
				prescale <= (PRESCALE << 2) - 2;
				bidx <= 8 + 2;
				data <= 0;
			end
		end
	end

endmodule