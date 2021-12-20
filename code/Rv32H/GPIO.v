
module GPIO (
	input wire i_enable,
	input wire i_rw,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,

	inout [35:0] io_GPIO
);
	reg [35:0] gpio = 0;

	assign io_GPIO = gpio;

	always @ (posedge i_enable) begin
		if (!i_rw) begin
			o_rdata <= GPIO[31:0];
		end
		else begin	// write
			gpio[31:0] <= i_wdata;
		end
	end
endmodule
