
// 55 ALUTS
// 81 DL Registers

`timescale 1ns/1ns

module Timer#(
    parameter FREQUENCY = 50000000
)(
	input wire i_reset,
	input wire i_clock,
	input wire i_request,
	input wire [2:0] i_address,
	output reg [31:0] o_rdata,
	output reg o_ready,
	
	// Debug
	input wire [31:0] i_retire_count
);

	localparam PRESCALE = FREQUENCY / 1000;
	localparam PRESCALE_WIDTH = $clog2(PRESCALE);

	reg [PRESCALE_WIDTH - 1:0] prescale = 0;
	reg [31:0] cycles = 0;
	reg [31:0] ms = 0;
	reg request = 0;

	initial o_ready = 1'b0;

	always @(posedge i_clock) begin
		if (i_reset)
			cycles <= 32'h00;
		else
			cycles <= cycles + 32'h01;
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
		if (i_request && !request) begin

			if (i_address == 3'h0) begin
				o_rdata <= ms;
			end
			else if (i_address == 3'h1) begin
				o_rdata <= cycles;
			end
			else if (i_address == 3'h2) begin
				o_rdata <= i_retire_count;
			end
			else
				o_rdata <= i_address;

			o_ready <= 1;
		end
		else if (!i_request)
			o_ready <= 0;
			
		request <= i_request;
	end
	
endmodule
