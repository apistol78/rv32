
`timescale 1ns/1ns

module I2C (
	input wire i_clock,
	input wire i_request,
	input wire i_rw,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
	output reg o_ready,

	output wire I2C_SCL,
	inout wire I2C_SDA
);
	reg scl = 1;
	reg sda = 1;
	
	wire [3:0] mask = i_wdata[7:4];
	wire [3:0] value = i_wdata[3:0];

	assign I2C_SCL = scl;
	assign I2C_SDA = sda ? 1'bz : 1'b0;	// pulled up

	always @ (posedge i_clock) begin
		if (i_request) begin
			if (!i_rw) begin
				o_rdata <= { 30'b0, scl, I2C_SDA };
			end
			else begin	// write
				sda <= (sda & ~mask[0]) | value[0];
				scl <= (scl & ~mask[1]) | value[1];
			end
		end
	end
	
	always @(posedge i_clock)
		o_ready <= i_request;
endmodule
