
`timescale 1ns/1ns

module I2C (
	input i_clock,
	input i_request,
	input i_rw,
	input [31:0] i_wdata,
	output bit [31:0] o_rdata,
	output bit o_ready,

	output I2C_SCL,
	inout I2C_SDA
);
	bit scl = 1;
	bit sda = 1;
	
	wire [3:0] mask = i_wdata[7:4];
	wire [3:0] value = i_wdata[3:0];

	assign I2C_SCL = scl;
	assign I2C_SDA = sda ? 1'bz : 1'b0;	// pulled up

	always_ff @(posedge i_clock) begin
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
	
	always_ff @(posedge i_clock)
		o_ready <= i_request;

endmodule
