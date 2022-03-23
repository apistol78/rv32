
`timescale 1ns/1ns

module BRAM_dual #(
	parameter WIDTH = 32,
	parameter SIZE = 32'h400,
	parameter ADDR_LSH = 2
)(
	input wire i_clock,

    // Port A
	input wire i_pa_request,
	input wire i_pa_rw,
	input wire [31:0] i_pa_address,
	input wire [WIDTH - 1:0] i_pa_wdata,
	output reg [WIDTH - 1:0] o_pa_rdata,
	output reg o_pa_ready,

    // Port B
	input wire i_pb_request,
	input wire i_pb_rw,
	input wire [31:0] i_pb_address,
	input wire [WIDTH - 1:0] i_pb_wdata,
	output reg [WIDTH - 1:0] o_pb_rdata,
	output reg o_pb_ready
);

	logic [WIDTH - 1:0] data [0:SIZE - 1];

	initial begin
        o_pa_ready = 0;
        o_pb_ready = 0;
    end

	always_ff @(posedge i_clock) begin
		if (i_pa_request) begin
			if (!i_pa_rw) begin
				o_pa_rdata <= data[i_pa_address >> ADDR_LSH];
			end
			else begin
				data[i_pa_address >> ADDR_LSH] <= i_pa_wdata;
			end
		end
		o_pa_ready <= i_pa_request;
	end

	always_ff @(posedge i_clock) begin
		if (i_pb_request) begin
			if (!i_pb_rw) begin
				o_pb_rdata <= data[i_pb_address >> ADDR_LSH];
			end
			else begin
				data[i_pb_address >> ADDR_LSH] <= i_pb_wdata;
			end
		end
		o_pb_ready <= i_pb_request;
	end

endmodule