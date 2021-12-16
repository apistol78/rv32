module BRAM(
	input wire i_enable,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata
);

	reg [31:0] data [0:32'h1fff];

	// Clear entire memory.
	integer i;
	initial begin
		for (i = 0; i < 32'h1000; i = i + 1) begin
			data[i] = 0;
			data[i + 32'h1000] = 0;
		end
	end

	always @ (posedge i_enable) begin
		if (!i_rw) begin
			o_rdata <= data[i_address >> 2];
		end
		else begin
			data[i_address >> 2] <= i_wdata;
		end
	end

endmodule