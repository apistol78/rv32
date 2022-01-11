
`timescale 1ns/1ns

// Fake SRAM module, for testing wait signal.
module SRAM_tb(
	input wire [17:0] SRAM_A,
	inout wire [15:0] SRAM_D,
	input wire SRAM_CE_n,
	input wire SRAM_OE_n,
	input wire SRAM_WE_n,
	input wire SRAM_LB_n,
	input wire SRAM_UB_n
);

	reg [15:0] data [0:32'h00040000 - 1];

	assign SRAM_D = !SRAM_OE_n ? data[SRAM_A] : 16'hz;

	always @(posedge SRAM_WE_n) begin
		$display("sram saved %x at %x (%0t)", SRAM_D, SRAM_A, $time);
		data[SRAM_A] = SRAM_D;
	end

endmodule