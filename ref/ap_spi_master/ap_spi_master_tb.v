`include "ap_spi_master.v"

module ap_spi_master_tb();

	reg clk = 0;
	reg en = 0;
	wire rdy;
	reg [7:0] tx;
	wire [7:0] rx;

	wire sck;
	wire cs;
	wire dc;
	wire mosi;
	reg miso = 0;

	ap_spi_master dut(
		.iCLK(clk),
		.iEN(en),
		.oRDY(rdy),
		.iTX(tx),
		.oRX(rx),

		.oSCK(sck),
		.oCS_i(cs),
		.oDC(dc),
		.oMOSI(mosi),
		.iMISO(miso)
	);

	initial begin
		forever #1 clk <= !clk;
	end

	initial begin
		$monitor($time, " : clk=%b, en=%b, rdy=%b, sck=%b, cs=%b, dc=%b, mosi=%b, miso=%b", clk, en, rdy, sck, cs, dc, mosi, miso);

		#1
		en <= 1;
		tx <= 8'h55;
		#1
		@(posedge rdy);
		#1
		en <= 0;
		#4

		$display("RX=%h", rx);
		$finish;
	end

endmodule
