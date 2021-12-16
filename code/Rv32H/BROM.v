module BROM(
	input wire i_enable,
	input wire [31:0] i_address,
	output reg [31:0] o_rdata
);

	reg [31:0] data [0:165];

   // Read ROM.
	// initial $readmemh("code/Bare/Bare.vmem", data);
	initial $readmemh("../code/Bare/Bare.vmem", data);
	
	// initial $readmemh("code/Mini/Mini.vmem", data);
	// initial $readmemh("../code/Mini/Mini.vmem", data);

	always @ (posedge i_enable) begin
		o_rdata <= data[i_address >> 2];
	end

endmodule