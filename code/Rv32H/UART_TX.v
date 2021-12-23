
module UART_TX #(
    parameter CLOCK_RATE = 50000000,
    parameter BAUD_RATE = 9600
)(
	input wire i_clock,
	input wire i_request,
	input wire [31:0] i_wdata,
	output wire o_ready,

	output reg UART_TX
);

	localparam STATE_IDLE		= 3'b000;
	localparam STATE_START_BIT	= 3'b001;
	localparam STATE_DATA_BITS	= 3'b010;
	localparam STATE_STOP_BIT	= 3'b011;

	reg [2:0] state = STATE_IDLE;
	reg [7:0] data = 0;
	reg [4:0] count = 0;
	reg ready = 0;

	wire baud_tx_clock;
	ClockDivider #(
		CLOCK_RATE,
		BAUD_RATE
	) tx_clock(
		.i_clock(i_clock),
		.o_clock(baud_tx_clock)
	);
	
	assign o_ready = i_request && ready;

	always @ (posedge baud_tx_clock or negedge i_request) begin

		if (~i_request) begin
			state <= STATE_IDLE;
			ready <= 0;
		end
		else begin

			case (state)
				STATE_IDLE: begin
					UART_TX <= 1;
					data <= i_wdata[7:0];
					count <= 0;
					state <= STATE_START_BIT;
				end

				STATE_START_BIT: begin
					UART_TX <= 0;
					state <= STATE_DATA_BITS;
				end

				STATE_DATA_BITS: begin
					UART_TX <= data[count];
					if (count >= 7) begin
						state <= STATE_STOP_BIT;
					end else begin
						count <= count + 5'd1;
					end
				end

				STATE_STOP_BIT: begin
					UART_TX <= 1;
					ready <= 1;
				end
			endcase

		end
	end

endmodule
