
module UART_RX #(
    parameter CLOCK_RATE = 50000000,
    parameter BAUD_RATE = 9600
)(
	input wire i_reset,
	input wire i_clock,

	input wire i_request,
	output reg [31:0] o_rdata,
    output wire o_ready,
	
	output wire [1:0] o_state,
	output wire [7:0] o_sample,
	
    input wire UART_RX
);

	localparam STATE_IDLE		= 0;
	localparam STATE_START_BIT	= 1;
	localparam STATE_DATA_BITS	= 2;
	localparam STATE_STOP_BIT	= 3;

	reg [2:0] state;
	reg [7:0] data;
	reg [7:0] sample;
	reg [7:0] count;
	reg [3:0] rds;
	
	assign o_state = state;
	assign o_sample = sample;

	wire rx_fifo_empty;
	reg rx_fifo_write = 0;
	reg rx_fifo_read = 0;
	wire [7:0] rx_fifo_rdata;
	FIFO #(
		.DEPTH(4)
	) rx_fifo(
		.i_clock_rd(i_clock),
		.i_clock_wr(baud_rx_clock),
		.o_empty(rx_fifo_empty),
		.i_write(rx_fifo_write),
		.i_wdata(data),
		.i_read(rx_fifo_read),
		.o_rdata(rx_fifo_rdata)
	);
	
	wire baud_rx_clock;
	ClockDivider #(
		CLOCK_RATE,
		BAUD_RATE * 16
	) rx_clock(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.o_clock(baud_rx_clock)
	);
	
	initial begin
		state = STATE_IDLE;
		data = 0;
		sample = 0;
		count = 0;
		rds = 0;

		o_rdata = 32'h0;
	end
	
	assign o_ready = (rds == 5) && i_request;
	
	always @ (posedge i_clock) begin
		if (i_reset) begin
			rds <= 0;
			o_rdata <= 32'h0;
		end
		else begin
			if (i_request) begin
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
			else begin
				rx_fifo_read <= 0;
				rds <= 0;
			end
		end
	end

	always @ (posedge baud_rx_clock) begin
		if (i_reset) begin
			state <= STATE_IDLE;
			data <= 0;
			sample <= 0;
			count <= 0;
		end
		else begin	
			case (state)
				STATE_IDLE: begin
					rx_fifo_write <= 0;
					if (UART_RX == 1'b0) begin
						state <= STATE_START_BIT;
						sample <= 0;
						count <= 0;
					end
				end
				
				STATE_START_BIT: begin
					if (sample >= 7) begin
						state <= STATE_DATA_BITS;
						sample <= 0;
						count <= 0;
					end
					else begin
						sample <= sample + 8'd1;
					end
				end
				
				STATE_DATA_BITS: begin
					if (sample >= 16 - 1) begin
						data[count] <= UART_RX;
						if (count >= 7) begin
							state <= STATE_STOP_BIT;
						end else begin
							count <= count + 8'd1;
						end
						sample <= 0;
					end
					else begin
						sample <= sample + 8'd1;
					end
				end
				
				STATE_STOP_BIT: begin
					if (sample >= 16 - 1) begin
						state <= STATE_IDLE;
						rx_fifo_write <= 1;
					end
					else begin
						sample <= sample + 8'd1;
					end
				end
			endcase
		end
	end

endmodule
