`include "CPU_Defines.v"

`timescale 1ns/1ns

module CPU_DCache(
	input wire i_reset,
	input wire i_clock,

	// Bus
	output reg o_bus_rw,
	output reg o_bus_request,
	input wire i_bus_ready,
	output reg [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,
	output reg [31:0] o_bus_wdata,

    // Input
    input wire i_rw,
    input wire i_request,
    output reg o_ready,
    input wire [31:0] i_address,
    output reg [31:0] o_rdata,
    input wire [31:0] i_wdata
);

	initial begin
		o_bus_rw = 0;
		o_bus_request = 0;
        o_bus_address = 0;
		o_bus_wdata = 0;

        o_ready = 0;
        o_rdata = 0;
	end

    always @(*) begin
        o_bus_rw = i_rw;
        o_bus_request = i_request;
        o_bus_address = i_address;
        o_bus_wdata = i_wdata;

        o_ready = i_bus_ready;
        o_rdata = i_bus_rdata;
    end

endmodule
