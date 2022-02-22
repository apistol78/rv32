
`timescale 1ns/1ns

module GPIO (
	input wire i_clock,
	input wire i_request,
	input wire i_rw,
	input wire [1:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
	output reg o_ready,

	inout [35:0] GPIO
);
	localparam DIR_IN = 1'b0;
	localparam DIR_OUT = 1'b1;

	reg [7:0] ctrl = 8'h00;		// all inputs initially.
	reg [7:0] outd = 8'h00;
	
	assign GPIO[0] = ctrl[0] ? outd[0] : 1'bz;
	assign GPIO[1] = ctrl[1] ? outd[1] : 1'bz;

	always @(posedge i_clock) begin
		if (i_request) begin
			if (!i_rw) begin
				if (i_address == 2'h0) begin	// ctrl
					o_rdata <= { 24'h0, ctrl };
				end
				else if (i_address == 2'h1) begin	// output
					o_rdata <= { 24'h0, outd };
				end			
			end
			else begin	// write
				if (i_address == 2'h0) begin	// ctrl
					ctrl <= i_wdata[7:0];
				end
				else if (i_address == 2'h1) begin	// output
					outd <= i_wdata[7:0];
				end
			end
		end
	end

	always @(posedge i_clock)
		o_ready <= i_request;

endmodule
