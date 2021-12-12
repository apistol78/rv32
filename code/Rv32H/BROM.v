module BROM(
	input wire i_enable,
	input wire [31:0] i_address,
	output reg [31:0] o_rdata
);

	reg [31:0] memory [0:32'hffff];

    // Read ROM.
	initial $readmemh("code/Mini/Mini.vmem", memory);

	always @ (posedge i_enable) begin
		o_rdata <= memory[i_address >> 2];
	end

endmodule