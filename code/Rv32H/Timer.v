
// 52 ALUTS
// 81 DL Registers

module Timer#(
    parameter FREQUENCY = 50000000
)(
	input wire i_reset,
	input wire i_clock,
	input wire i_request,
	input wire [1:0] i_address,
	output reg [31:0] o_rdata,
	output wire o_ready,
	
	// Debug
	input wire [31:0] i_retire_count
);

	localparam PRESCALE = FREQUENCY / 1000;
	localparam PRESCALE_WIDTH = $clog2(PRESCALE);

	reg [PRESCALE_WIDTH - 1:0] prescale = 0;
	
	reg [31:0] cycles = 0;
	reg [31:0] ms = 0;

	assign o_ready = i_request;

	always @(posedge i_clock) begin
		if (i_reset)
			cycles <= 0;
		else
			cycles <= cycles + 1;
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			ms <= 0;
			prescale <= 0;
		end
		else begin
			prescale <= prescale + 1;
			if (prescale >= PRESCALE) begin
				ms <= ms + 1;
				prescale <= 0;
			end
		end
	end

	always @(posedge i_clock) begin
		if (i_request) begin
			if (i_address == 2'h0)
				o_rdata <= ms;
			else if (i_address == 2'h1)
				o_rdata <= cycles;
			else if (i_address == 2'h2)
				o_rdata <= i_retire_count;
			else
				o_rdata <= 0;
		end
	end

endmodule
