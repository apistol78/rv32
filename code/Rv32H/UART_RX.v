
module UART_RX #(
    parameter CLOCK_RATE = 50000000,
    parameter BAUD_RATE = 9600
)(
	input wire i_reset,
	input wire i_clock,

	input wire i_request,
	output reg [31:0] o_rdata,
    output reg o_ready,
	output reg o_waiting,
	
	output wire [1:0] o_state,
	
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
	reg [1:0] rds;
	
	assign o_state = state;

	wire rx_fifo_empty;
	reg rx_fifo_write = 0;
	reg rx_fifo_read = 0;
	wire [7:0] rx_fifo_rdata;
	FIFO #(
		.DEPTH(4)
	) rx_fifo(
		.i_reset(i_reset),
		.i_clock(i_clock),
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
		state <= STATE_IDLE;
		data <= 0;
		sample <= 0;
		count <= 0;
		rds <= 0;

		o_rdata <= 32'h0;
		o_ready <= 0;
		o_waiting <= 0;
	end
	
	always @ (posedge i_clock, posedge i_reset) begin
		if (i_reset) begin
			rds <= 0;
			o_rdata <= 32'h0;
			o_ready <= 0;
			o_waiting <= 0;
		end
		else begin
			if (i_request) begin
				case (rds)
					2'b00: begin
						if (!rx_fifo_empty) begin
							o_waiting <= 0;
							rx_fifo_read <= 1;
							rds <= 2'b01;
						end
						else begin
							o_waiting <= 1;
						end
					end
					2'b01: begin
						rds <= 2'b10;
					end
					2'b10: begin
						o_rdata <= { 24'b0, rx_fifo_rdata };
						o_ready <= 1;
					end
				endcase
			end
			else begin
				rx_fifo_read <= 0;
				o_ready <= 0;
				rds <= 2'b00;
			end
		end
	end

	always @ (posedge baud_rx_clock, posedge i_reset) begin
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
