module BROM(
	input wire i_enable,
	input wire [31:0] i_address,
	output reg [31:0] o_rdata
);

	reg [31:0] memory [32'hffff:0];

    // Read ROM.
	// initial $readmemh("code/Bare/Bare.vmem", memory);
	initial $readmemh("code/Mini/Mini.vmem", memory);

	always @ (posedge i_enable) begin
		o_rdata <= memory[i_address >> 2];
	end

endmodule