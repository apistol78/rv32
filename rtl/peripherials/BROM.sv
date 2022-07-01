
`timescale 1ns/1ns

module BROM(
	input i_clock,
	input i_request,
	input [31:0] i_address,
	output bit [31:0] o_rdata,
	output bit o_ready
);

	bit [31:0] data [
`ifdef __TESTBENCH__
	`include "Firmware.vmem-range"		
`else
	`include "../../Firmware.vmem-range"		
`endif
	];

	initial o_ready = 0;

`ifdef __TESTBENCH__
	initial $readmemh("Firmware.vmem", data);
`else
	initial $readmemh("../../Firmware.vmem", data);
`endif

	always_ff @(posedge i_clock)
		if (i_request) begin
			o_rdata <= data[i_address >> 2];
		end
		else begin
			o_rdata <= 32'hx;
		end

	always_ff @(posedge i_clock)
		o_ready <= i_request;

endmodule