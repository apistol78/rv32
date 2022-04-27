`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_DCache_None #(
	parameter SIZE = 14
)(
	input i_reset,
	input i_clock,

	// Bus
	output logic o_bus_rw,
	output logic o_bus_request,
	input i_bus_ready,
	output logic [31:0] o_bus_address,
	input [31:0] i_bus_rdata,
	output logic [31:0] o_bus_wdata,

	// Input
	input i_rw,
	input i_request,
	input i_flush,
	output logic o_ready,
	input [31:0] i_address,
	output logic [31:0] o_rdata,
	input [31:0] i_wdata,
	input i_cacheable
);

    initial begin
        o_bus_request = 0;
        o_ready = 0;
    end

    always_ff @(posedge i_clock) begin
        
        o_bus_request <= i_request;
        o_bus_rw <= i_rw;
        o_bus_address <= i_address;
        o_bus_wdata <= i_wdata;

        o_rdata <= i_bus_rdata;
        o_ready <= i_bus_ready;

    end

endmodule
