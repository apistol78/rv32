
module GPIO (
	input wire i_enable,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,

	inout [35:0] GPIO
);
	reg [35:0] gpio = 0;

	wire [15:0] mask = i_wdata[31:16];
	wire [15:0] write = i_wdata[15:0];

	assign GPIO = gpio;
	
	initial o_rdata = 0;

	always @ (posedge i_enable) begin
		if (!i_rw) begin
			if (i_address == 32'h0000_0000) begin
				o_rdata <= { 16'h0000, GPIO[15:0] };
			end
			else if (i_address == 32'h0000_0004) begin
				o_rdata <= { 16'h0000, GPIO[31:16] };
			end
		end
		else begin	// write
			if (i_address == 32'h0000_0004) begin
				gpio[15:0] <= (gpio[15:0] & ~mask) | write;
			end
			else if (i_address == 32'h0000_0008) begin
				gpio[31:16] <= (gpio[31:16] & ~mask) | write;
			end
			else begin
				gpio[31:0] <= i_wdata;
			end
		end
	end
endmodule
