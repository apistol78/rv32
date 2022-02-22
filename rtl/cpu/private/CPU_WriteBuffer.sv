
`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_WriteBuffer(
	input i_reset,
	input i_clock,

	// Bus
	output reg o_bus_rw,
	output reg o_bus_request,
	input i_bus_ready,
	output reg [31:0] o_bus_address,
	input [31:0] i_bus_rdata,
	output reg [31:0] o_bus_wdata,

	// Input
	input i_rw,
	input i_request,
	output reg o_ready,
	input [31:0] i_address,
	output reg [31:0] o_rdata,
	input [31:0] i_wdata
);
	localparam DEPTH = 6;

	logic [63:0] records [DEPTH - 1:0];
	logic [$clog2(DEPTH) - 1:0] in = 0;
	logic [$clog2(DEPTH) - 1:0] out = 0;
	logic ready = 0;

	wire empty = (in == out) ? 1'b1 : 1'b0;
	wire full = (((in + 1) & (DEPTH - 1)) == out) ? 1'b1 : 1'b0;

	initial begin
		o_bus_rw = 0;
		o_bus_request = 0;
		o_bus_address = 0;
		o_bus_wdata = 0;
		o_ready = 0;
		o_rdata = 0;
	end

	always_comb begin
		o_ready = i_request && ready;
	end

	always_ff @(posedge i_clock) begin

		if (!i_request)
			ready <= 0;

		o_bus_request <= 0;
		o_bus_rw <= 0;

		//  Process requests from CPU.
		if (i_request) begin
			if (i_rw && !full) begin
				records[in] <= { i_address, i_wdata };
				in <= (in + 1) & (DEPTH - 1);
				ready <= 1;
			end
			else if (!i_rw && !ready && empty) begin
				o_bus_request <= 1;
				o_bus_address <= i_address;
				o_rdata <= i_bus_rdata;
				if (i_bus_ready) begin
					o_bus_request <= 0;
					ready <= 1;
				end
			end
		end

		// Process pending BUS write requests.
		if (!empty) begin
			o_bus_request <= 1;
			o_bus_rw <= 1;
			o_bus_address <= records[out][63:32];
			o_bus_wdata <= records[out][31:0];
			if (i_bus_ready) begin
				o_bus_request <= 0;
				o_bus_rw <= 0;
				out <= (out + 1) & (DEPTH - 1);
			end
		end

	end

endmodule
