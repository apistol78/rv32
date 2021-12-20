module BROM(
	input wire i_enable,
	input wire [31:0] i_address,
	output reg [31:0] o_rdata
);

	reg [31:0] data [0:300];

`ifdef __ICARUS__
	initial $readmemh("code/Firmware/Firmware.vmem", data);
`else
	initial $readmemh("../code/Firmware/Firmware.vmem", data);
`endif

	always @ (posedge i_enable) begin
		o_rdata <= data[i_address >> 2];
	end

endmodule