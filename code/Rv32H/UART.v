
module UART #(
    parameter CLOCK_RATE = 50000000,
    parameter BAUD_RATE = 9600
)(
	input wire i_clock,
	input wire i_enable,
	input wire i_rw,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
    output reg o_ready,

    input wire UART_RX,
    output reg UART_TX
);

	localparam STATE_IDLE		= 3'b000;
	localparam STATE_START_BIT	= 3'b001;
	localparam STATE_DATA_BITS	= 3'b010;
	localparam STATE_STOP_BIT	= 3'b011;

	reg [2:0] rx_state = STATE_IDLE;
	reg [7:0] rx_data = 0;
	reg [4:0] rx_sample = 0;
	reg [4:0] rx_count = 0;

	reg [2:0] tx_state = STATE_IDLE;
	reg [7:0] tx_data = 0;
	reg [4:0] tx_count = 0;

	wire baud_rx_clock;
	ClockDivider #(
		CLOCK_RATE,
		BAUD_RATE * 16
	) rx_clock(
		.i_clock(i_clock),
		.o_clock(baud_rx_clock)
	);
	
	wire baud_tx_clock;
	ClockDivider #(
		CLOCK_RATE,
		BAUD_RATE
	) tx_clock(
		.i_clock(i_clock),
		.o_clock(baud_tx_clock)
	);
	
	initial begin
		o_ready <= 1;
	end
	
	always @ (posedge baud_rx_clock) begin
		case (rx_state)
			STATE_IDLE: begin
				if (UART_RX == 1'b0) begin
					rx_state <= STATE_START_BIT;
					rx_sample <= 16 + 7;
				end
			end
			
			STATE_START_BIT: begin
				if (rx_sample <= 0) begin
					rx_state <= STATE_DATA_BITS;
					rx_sample <= 16;
					rx_count <= 0;
				end else begin
					rx_sample = rx_sample - 1;
				end
			end
			
			STATE_DATA_BITS: begin
				if (rx_sample <= 0) begin
					rx_data[rx_count] <= UART_RX;
					if (rx_count >= 7) begin
						rx_state <= STATE_STOP_BIT;
						rx_sample <= 16;
					end else begin
						rx_count <= rx_count + 1;
					end
					rx_sample <= 16;
				end else begin
					rx_sample = rx_sample - 1;
				end
			end
			
			STATE_STOP_BIT: begin
				if (rx_sample <= 0) begin
					rx_state <= STATE_IDLE;
				end else begin
					rx_sample = rx_sample - 1;
				end
			end
		endcase
	end

	always @ (posedge baud_tx_clock) begin
		case (tx_state)
			STATE_IDLE: begin
				UART_TX <= 1;
				if (i_rw && i_enable) begin
					tx_data <= i_wdata[31:24] | i_wdata[23:16] | i_wdata[15:8] | i_wdata[7:0];
					tx_count <= 0;
					tx_state <= STATE_START_BIT;
					o_ready <= 0;
				end
			end

			STATE_START_BIT: begin
				UART_TX <= 0;
				tx_state <= STATE_DATA_BITS;
			end

			STATE_DATA_BITS: begin
				UART_TX <= tx_data[tx_count];
				if (tx_count >= 7) begin
					tx_state <= STATE_STOP_BIT;
				end else begin
					tx_count <= tx_count + 1;
				end
			end

			STATE_STOP_BIT: begin
				UART_TX <= 1;
				o_ready <= 1;
				if (!i_enable) begin
					tx_state <= STATE_IDLE;
				end
			end
		endcase
	end

endmodule
