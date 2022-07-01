
`timescale 1ns/1ns

module BRAM #(
	parameter WIDTH = 32,
	parameter SIZE = 32'h400,
	parameter ADDR_LSH = 2
)(
	input i_clock,
	input i_request,
	input i_rw,
	input [31:0] i_address,
	input [WIDTH - 1:0] i_wdata,
	output bit [WIDTH - 1:0] o_rdata,
	output bit o_ready,
	output bit o_valid
);
    (* ram_style = "block" *)
	bit [WIDTH - 1:0] data [0:SIZE - 1];

	initial begin
		o_ready = 0;
		o_valid = 1;
	end

	always_ff @(posedge i_clock) begin
		if (i_request) begin
			o_valid <= (i_address >> ADDR_LSH) < SIZE;
			if (!i_rw) begin
				o_rdata <= data[i_address >> ADDR_LSH];
			end
			else begin
				data[i_address >> ADDR_LSH] <= i_wdata;
			end
		end
	end

	always_ff @(posedge i_clock)
		o_ready <= i_request;

endmodule