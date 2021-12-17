
module UART_RX #(
    parameter CLOCK_RATE = 50000000,
    parameter BAUD_RATE = 9600
)(
	input wire i_clock,
	input wire i_enable,
	output reg [31:0] o_rdata,
    output reg o_received,

    input wire UART_RX
);

	localparam STATE_IDLE		= 3'b000;
	//localparam STATE_START_BIT	= 3'b001;
	localparam STATE_DATA_BITS	= 3'b010;
	localparam STATE_STOP_BIT	= 3'b011;

	reg [2:0] state = STATE_IDLE;
	reg [7:0] data = 0;
	reg [6:0] sample = 0;
	reg [6:0] count = 0;

	wire baud_rx_clock;
	ClockDivider #(
		CLOCK_RATE,
		BAUD_RATE * 16
	) rx_clock(
		.i_clock(i_clock),
		.o_clock(baud_rx_clock)
	);
	
	initial begin
		o_rdata <= 32'h0;
		o_received <= 0;
	end

	always @ (posedge baud_rx_clock) begin

		if (!i_enable) begin
			o_received <= 0;
		end

		case (state)
			STATE_IDLE: begin
				if (UART_RX == 1'b0) begin
					state <= STATE_DATA_BITS;
					sample <= 16-1 + 4;
					count <= 0;
					o_received <= 0;
				end
			end
			
			/*
			STATE_START_BIT: begin
				if (sample == 0) begin
					if (UART_RX == 1'b1) begin
						state <= STATE_DATA_BITS;
						sample <= 16-1;
						count <= 0;
					end
					else begin
						state <= STATE_IDLE;	// was expecting start bit (1), cannot continue.
					end
				end else begin
					sample = sample - 1;
				end
			end
			*/
			
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
						o_rdata <= { data, data, data, data };
						o_received <= 1;
					end
				end else begin
					sample = sample - 1;
				end
			end
		endcase
	end

endmodule
