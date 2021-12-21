
module UART_RX #(
    parameter CLOCK_RATE = 50000000,
    parameter BAUD_RATE = 9600
)(
	input wire i_clock,
	input wire i_enable,
	output reg [31:0] o_rdata,
    output reg o_ready,
	output reg o_waiting,
	
    input wire UART_RX
);

	localparam STATE_IDLE		= 3'b000;
	localparam STATE_DATA_BITS	= 3'b010;
	localparam STATE_STOP_BIT	= 3'b011;

	reg [2:0] state = STATE_IDLE;
	reg [7:0] data = 0;
	reg [6:0] sample = 0;
	reg [6:0] count = 0;
	reg [2:0] rds = 0;

	wire baud_rx_clock;
	ClockDivider #(
		CLOCK_RATE,
		BAUD_RATE * 16
	) rx_clock(
		.i_clock(i_clock),
		.o_clock(baud_rx_clock)
	);
	
	wire rx_fifo_empty;
	reg rx_fifo_write = 0;
	reg rx_fifo_read = 0;
	wire [7:0] rx_fifo_rdata;
	FIFO #(
		.DEPTH(64)
	) rx_fifo(
		.i_clock(i_clock),
		.o_empty(rx_fifo_empty),
		.i_write(rx_fifo_write),
		.i_wdata(data),
		.i_read(rx_fifo_read),
		.o_rdata(rx_fifo_rdata)
	);
	
	initial begin
		o_rdata <= 32'h0;
		o_ready <= 0;
		o_waiting <= 0;
	end
	
	always @ (posedge i_clock) begin
		if (i_enable) begin
			case (rds)
				0: begin
					if (!rx_fifo_empty) begin
						o_waiting <= 0;
						rx_fifo_read <= 1;
						rds <= 1;
					end
					else begin
						o_waiting <= 1;
					end
				end
				1: begin
					rds <= 2;
				end
				2: begin
					o_rdata <= { rx_fifo_rdata, rx_fifo_rdata, rx_fifo_rdata, rx_fifo_rdata };
					o_ready <= 1;
				end
			endcase
		end
		else begin
			rx_fifo_read <= 0;
			o_ready <= 0;
			rds <= 0;
		end
	end

	always @ (posedge baud_rx_clock) begin
		case (state)
			STATE_IDLE: begin
				rx_fifo_write <= 0;
				if (UART_RX == 1'b0) begin
					state <= STATE_DATA_BITS;
					sample <= 16-1 + 4;
					count <= 0;
				end
			end
			
			STATE_DATA_BITS: begin
				if (sample == 0) begin
					data[count] <= UART_RX;
					if (count >= 7) begin
						state <= STATE_STOP_BIT;
						sample <= 16-1;
					end else begin
						count <= count + 1;
					end
					sample <= 16-1;
				end else begin
					sample = sample - 1;
				end
			end
			
			STATE_STOP_BIT: begin
				if (sample == 0) begin
					if (UART_RX == 1'b1) begin
						state <= STATE_IDLE;
						rx_fifo_write <= 1;
					end
				end else begin
					sample = sample - 1;
				end
			end
		endcase
	end

endmodule
