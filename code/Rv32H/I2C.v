
module I2C (
	input wire i_enable,
	input wire i_rw,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,

	output wire I2C_SCL,
	inout wire I2C_SDA
);
	reg scl = 1;
	reg sda = 1;
	
	wire [3:0] wmask = i_wdata[7:4];
	wire [3:0] wvalue = i_wdata[3:0];

	assign I2C_SCL = scl;
	assign I2C_SDA = sda ? 1'bz : 1'b0;	// pulled up

	always @ (posedge i_enable) begin
		if (!i_rw) begin
			o_rdata <= { 30'b0, scl, I2C_SDA };
		end
		else begin	// write
			sda <= (sda & wmask[0]) | wvalue[0];
			scl <= (scl & wmask[1]) | wvalue[1];
		end
	end
endmodule
