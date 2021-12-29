module BROM(
	input wire i_clock,
	input wire i_request,
	input wire [31:0] i_address,
	output reg [31:0] o_rdata,
	output reg o_ready
);

	reg [31:0] data [0:400];

	initial o_ready = 0;

`ifdef __ICARUS__
	initial $readmemh("code/Firmware/Firmware.vmem", data);
`else
	initial $readmemh("../code/Firmware/Firmware.vmem", data);
`endif

	always @(posedge i_clock)
		if (i_request) begin
			o_rdata <= data[i_address >> 2];
		end
		else begin
			o_rdata <= 32'hx;
		end

	always @(posedge i_clock)
		o_ready <= i_request;

endmodule