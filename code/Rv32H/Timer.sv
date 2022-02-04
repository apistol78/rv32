
// 55 ALUTS
// 81 DL Registers

`timescale 1ns/1ns

module Timer#(
    parameter FREQUENCY = 50000000
)(
	input wire i_reset,
	input wire i_clock,
	input wire i_request,
	input wire i_rw,
	input wire [2:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
	output reg o_ready,

	output reg o_interrupt
);

	localparam PRESCALE = FREQUENCY / 1000;
	localparam PRESCALE_WIDTH = $clog2(PRESCALE);

	reg [PRESCALE_WIDTH - 1:0] prescale = 0;
	reg [63:0] cycles = 0;
	reg [63:0] compare = { 64{1'b1} };
	reg [31:0] ms = 0;
	reg request = 0;

	initial begin
		o_ready = 1'b0;
		o_interrupt = 1'b0;
	end

	always @(posedge i_clock) begin
		if (i_reset)
			cycles <= 64'h0;
		else begin
			cycles <= cycles + 64'h1;
			o_interrupt <= (cycles == compare) ? 1'b1 : 1'b0;
		end
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
			if (!i_rw) begin
				if (i_address == 3'h0) begin
					o_rdata <= ms;
				end
				else if (i_address == 3'h1) begin
					o_rdata <= cycles[31:0];
				end
				else if (i_address == 3'h2) begin
					o_rdata <= cycles[63:32];
				end
			end
			else begin
				if (i_address == 3'h3) begin
					compare[31:0] <= i_wdata;
				end
				else if (i_address == 3'h4) begin
					compare[63:32] <= i_wdata;
				end
			end
			o_ready <= 1;
		end
		else if (!i_request)
			o_ready <= 0;
			
		request <= i_request;
	end
	
endmodule
