
module Timer#(
    parameter FREQUENCY = 50000000
)(
	input wire i_clock,
	input wire i_request,
	output reg [31:0] o_rdata,
	output reg o_ready
);

	localparam PRESCALE = FREQUENCY / 1000;

	reg [31:0] prescale = 0;
	reg [31:0] counter = 0;

	always @(posedge i_clock) begin
		prescale <= prescale + 1;
		if (prescale >= PRESCALE) begin
			counter <= counter + 1;
			prescale <= 0;
		end
	end

	always @(posedge i_clock) begin
		if (i_request) begin
			o_rdata <= counter;
			o_ready <= 1;
		end
		else begin
			o_ready <= 0;
		end
	end

endmodule
