
`timescale 1ns/1ns

module Timer#(
    parameter FREQUENCY,
	parameter DEVICEID
)(
	input i_reset,
	input i_clock,
	input i_request,
	input i_rw,
	input [2:0] i_address,
	input [31:0] i_wdata,
	output logic [31:0] o_rdata,
	output logic o_ready,

	output logic o_interrupt
);

	localparam PRESCALE = FREQUENCY / 1000;
	localparam PRESCALE_WIDTH = $clog2(PRESCALE);

	logic [PRESCALE_WIDTH - 1:0] prescale = 0;
	logic [63:0] cycles = 0;
	logic [63:0] compare = { 64{1'b1} };
	logic [31:0] ms = 0;
	logic request = 0;

	initial begin
		o_ready = 1'b0;
		o_interrupt = 1'b0;
	end

	always_ff @(posedge i_clock) begin
		if (i_reset)
			cycles <= 64'h0;
		else begin
			cycles <= cycles + 64'h1;
			o_interrupt <= (cycles == compare) ? 1'b1 : 1'b0;
		end
	end

	always_ff @(posedge i_clock) begin
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

	always_ff @(posedge i_clock) begin
		if (i_request && !request) begin
			if (!i_rw) begin
				case (i_address)
					3'h0: o_rdata <= ms;
					3'h1: o_rdata <= cycles[31:0];
					3'h2: o_rdata <= cycles[63:32];
					3'h3: o_rdata <= compare[31:0];
					3'h4: o_rdata <= compare[63:32];
					3'h5: o_rdata <= FREQUENCY;
					3'h6: o_rdata <= DEVICEID;
					default: o_rdata <= 0;
				endcase
			end
			else begin
				case (i_address)
					3'h3: compare[31:0] <= i_wdata;
					3'h4: compare[63:32] <= i_wdata;
					default:;
				endcase
			end
			o_ready <= 1;
		end
		else if (!i_request)
			o_ready <= 0;
			
		request <= i_request;
	end
	
endmodule
