
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

	reg [63:0] record;
	reg pending = 0;
	reg ready = 0;

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

		if (!pending && i_request) begin
			if (i_rw) begin
				record <= { i_address, i_wdata };
				pending <= 1;
				ready <= 1;
			end
			else if (!i_rw && !ready) begin
				o_bus_request <= 1;
				o_bus_address <= i_address;
				o_rdata <= i_bus_rdata;
				if (i_bus_ready) begin
					o_bus_request <= 0;
					ready <= 1;
				end
			end
		end

		if (pending) begin
			o_bus_request <= 1;
			o_bus_rw <= 1;
			o_bus_address <= record[63:32];
			o_bus_wdata <= record[31:0];
			if (i_bus_ready) begin
				o_bus_request <= 0;
				o_bus_rw <= 0;
				pending <= 0;
			end
		end

	end

endmodule
