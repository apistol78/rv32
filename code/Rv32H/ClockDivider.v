module ClockDivider #(
    parameter CLOCK_RATE = 50000000,
    parameter BAUD_RATE = 9600
)(
	input wire i_clock,
	output reg o_clock
);

	parameter MAX_RATE = CLOCK_RATE / (2 * BAUD_RATE);
	parameter CNT_WIDTH = $clog2(MAX_RATE);

	reg [CNT_WIDTH - 1:0] counter = 0;

	initial begin
	    o_clock = 1'b0;
	end

	always @(posedge i_clock) begin
	    if (counter == MAX_RATE[CNT_WIDTH - 1:0]) begin
	        counter <= 0;
	        o_clock <= ~o_clock;
	    end else begin
	        counter <= counter + 1'b1;
	    end
	end

endmodule
