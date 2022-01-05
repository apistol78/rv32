module ClockDivider #(
    parameter CLOCK_RATE = 50000000,
    parameter BAUD_RATE = 9600
)(
	input wire i_reset,
	input wire i_clock,
	output reg o_clock
);

	localparam MAX_RATE = CLOCK_RATE / (2 * BAUD_RATE);
	localparam CNT_WIDTH = $clog2(MAX_RATE);

	reg [CNT_WIDTH - 1:0] counter = 0;

	initial o_clock = 0;

	always @(posedge i_clock) begin
		if (i_reset) begin
			counter <= 0;
			o_clock <= 0;
		end
		else begin
			if (counter >= MAX_RATE[CNT_WIDTH - 1:0]) begin
				counter <= 0;
				o_clock <= ~o_clock;
			end else begin
				counter <= counter + 1'b1;
			end
		end
	end

endmodule
