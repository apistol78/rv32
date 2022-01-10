
`timescale 1ns/1ns

module GPIO (
	input wire i_clock,
	input wire i_request,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
	output reg o_ready,

	inout [35:0] GPIO
);
/*
	reg [35:0] gpio = 36'b0;

	wire [15:0] mask = i_wdata[31:16];
	wire [15:0] write = i_wdata[15:0];

	assign GPIO = gpio;
	
	initial o_rdata = 0;
	initial o_ready = 0;
*/

	reg [7:0] ctrl = 8'h00;		// all inputs initially.
	reg [7:0] outd = 8'h00;
	
	
	assign GPIO[0] = ctrl[0] ? outd[0] : 1'bz;
	assign GPIO[1] = ctrl[1] ? outd[1] : 1'bz;
	

	always @ (posedge i_clock) begin
		if (i_request) begin
			if (!i_rw) begin
			
				if (i_address[3:0] == 4'h0) begin	// ctrl
					o_rdata <= { 24'h0, ctrl };
				end
				else if (i_address[3:0] == 4'h4) begin	// output
					o_rdata <= { 24'h0, outd };
				end			
			
			end
			else begin	// write
			
				if (i_address[3:0] == 4'h0) begin	// ctrl
					ctrl <= i_wdata[7:0];
				end
				else if (i_address[3:0] == 4'h4) begin	// output
					outd <= i_wdata[7:0];
				end
				
			end
		end
	end

	always @(posedge i_clock)
		o_ready <= i_request;

endmodule
