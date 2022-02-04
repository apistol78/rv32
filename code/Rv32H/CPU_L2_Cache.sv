`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_L2_Cache(
	input wire i_reset,
	input wire i_clock,

	// Bus
	output wire o_bus_rw,
	output wire o_bus_request,
	input wire i_bus_ready,
	output wire [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,
	output wire [31:0] o_bus_wdata,

	// Input
	input wire i_rw,
	input wire i_request,
	output wire o_ready,
	input wire [31:0] i_address,
	output wire [31:0] o_rdata,
	input wire [31:0] i_wdata
);

	assign o_bus_rw = i_rw;
	assign o_bus_request = i_request;
	assign o_ready = i_bus_ready;
	assign o_bus_address = i_address;
	assign o_rdata = i_bus_rdata;
	assign o_bus_wdata = i_wdata;

endmodule
