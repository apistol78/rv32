
`timescale 1ns/1ns

`ifdef __VERILATOR__
	`define INSTANT_CLEAR
`endif

module BRAM_clear #(
	parameter WIDTH = 32,
	parameter SIZE = 32'h400,
	parameter ADDR_LSH = 2,
	parameter CLEAR_VALUE = 32'h0
)(
	input wire i_reset,
	input wire i_clock,
	output wire o_initialized,
	input wire i_request,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [WIDTH - 1:0] i_wdata,
	output reg [WIDTH - 1:0] o_rdata,
	output reg o_ready
);

	reg [WIDTH - 1:0] data [0:SIZE - 1];
	reg [31:0] clear;

	integer i;
	initial begin
		o_ready = 0;
		o_rdata = 0;
`ifdef INSTANT_CLEAR
		clear = SIZE;
		for (i = 0; i < SIZE; i = i + 1)
			data[i] = CLEAR_VALUE;
`else
		clear = 0;
`endif
	end

	assign o_initialized = clear >= SIZE;

	always @(posedge i_clock) begin
		if (i_reset) begin
			clear <= 0;
		end
		else if (clear < SIZE) begin
			clear <= clear + 1;
		end
	end

	always @(posedge i_clock) begin
		if (clear < SIZE) begin
			data[clear] <= CLEAR_VALUE;
		end 
		else begin
			if (i_request) begin
				if (!i_rw) begin
					o_rdata <= data[i_address >> ADDR_LSH];
				end
				else begin
					data[i_address >> ADDR_LSH] <= i_wdata;
				end
			end
		end
	end

	always @(posedge i_clock)
		o_ready <= i_request;

endmodule