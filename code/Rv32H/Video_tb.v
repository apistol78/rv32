
`timescale 1ns/1ns

module Video_tb(
	input wire i_clock,
	input wire i_request,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata
);

	always @ (posedge i_clock) begin
		if (i_request) begin
			if (!i_rw) begin
				o_rdata <= 32'h0;
			end
			else begin
				//$display("VIDEO %d (%s)", i_wdata[7:0], i_wdata[7:0]);
				$write("%s", i_wdata[7:0]);
			end
		end
	end

endmodule