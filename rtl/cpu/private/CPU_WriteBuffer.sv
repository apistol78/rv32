
`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_WriteBuffer(
	input i_reset,
	input i_clock,

	// Bus
	output bit o_bus_rw,
	output bit o_bus_request,
	input i_bus_ready,
	output bit [31:0] o_bus_address,
	input [31:0] i_bus_rdata,
	output bit [31:0] o_bus_wdata,

	// Input
	input i_rw,
	input i_request,
	output bit o_ready,
	input [31:0] i_address,
	output bit [31:0] o_rdata,
	input [31:0] i_wdata
);


	initial begin
		o_bus_rw = 0;
		o_bus_request = 0;
		o_bus_address = 0;
		o_bus_wdata = 0;
		o_ready = 0;
		o_rdata = 0;
	end

	bit pending = 0;

	always_ff @(posedge i_clock) begin

		if (!i_request)
			o_ready <= 0;

		if (i_request && !o_ready && !pending) begin
			if (i_rw) begin
				
				o_bus_request <= 1;
				o_bus_rw <= 1;
				o_bus_address <= i_address;
				o_bus_wdata <= i_wdata;
				pending <= 1;

				o_ready <= 1;

			end
			else begin

				o_bus_request <= 1;
				o_bus_rw <= 0;
				o_bus_address <= i_address;

				if (i_bus_ready) begin
					o_bus_request <= 0;
					o_rdata <= i_bus_rdata;
					o_ready <= 1;
				end

			end
		end

		if (pending) begin
			// pending write, need to wait until it's finished.
			if (i_bus_ready) begin
				o_bus_request <= 0;
				pending <= 0;
			end
		end


	end

endmodule
