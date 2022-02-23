
`timescale 1ns/1ns

module BRAM_latency #(
	parameter WIDTH = 32,
	parameter SIZE = 32'h400,
	parameter ADDR_LSH = 2,
	parameter LATENCY = 10
)(
	input wire i_clock,
	input wire i_request,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [WIDTH - 1:0] i_wdata,
	output reg [WIDTH - 1:0] o_rdata,
	output reg o_ready
);

	reg [WIDTH - 1:0] data [0:SIZE - 1];
	reg [15:0] counter;

	initial o_ready = 0;

	always_comb begin
		o_ready = i_request && (counter >= LATENCY);
	end

	always_ff @(posedge i_clock) begin
		if (i_request) begin
			if (!i_rw) begin
				o_rdata <= data[i_address >> ADDR_LSH];
			end
			else begin
				data[i_address >> ADDR_LSH] <= i_wdata;
			end

			counter <= counter + 1;
		end
		else begin
			counter <= 0;
		end
	end

endmodule