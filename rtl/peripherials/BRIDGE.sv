
`timescale 1ns/1ns

module BRIDGE (
	input i_clock,
	input i_reset,

	// Near
	input i_request,
	input i_rw,
	input [27:0] i_address,
	input [31:0] i_wdata,
	output logic [31:0] o_rdata,
	output logic o_ready,

	// Far
	output o_far_request,
	output o_far_rw,
	output [27:0] o_far_address,
	output [31:0] o_far_wdata,
	input [31:0] i_far_rdata,
	input i_far_ready
);

	logic request = 0;
	logic rw;
	logic [27:0] address;
	logic [31:0] wdata;

	assign o_far_request = request;
	assign o_far_rw = rw;
	assign o_far_address = address;
	assign o_far_wdata = wdata;

	initial begin
		o_ready = 0;
	end

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			request <= 0;
			o_ready <= 0;
		end
		else begin
			request <= i_request;
			rw <= i_rw;
			address <= i_address;
			wdata <= i_wdata;

			o_rdata <= i_far_rdata;
			o_ready <= i_request && i_far_ready;
		end
	end

endmodule