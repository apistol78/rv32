`include "ap_spi_master_cpu.v"

module ap_spi_master_tb();

	reg clk = 0;
	reg en = 0;
	reg rw = 0;
	reg [3:0] addr = 0;
	reg [7:0] i_data = 0;
	wire [7:0] o_data;

	wire sck;
	wire cs;
	wire dc;
	wire mosi;
	reg miso = 0;

	ap_spi_master_cpu dut(
		.iCLK(clk),
		.iEN(en),
		.iRW(rw),
		.iADDR(addr),
		.iDATA(i_data),
		.oDATA(o_data),

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
		$monitor($time, " : clk=%b, en=%b, rw=%b, sck=%b, cs=%b, dc=%b, mosi=%b, miso=%b", clk, en, rw, sck, cs, dc, mosi, miso);

		#1
		en <= 1;
		rw <= 1;
		addr <= 1;
		i_data <= 8'h55;
		#1
		en <= 0;
		#1
		en <= 1;
		rw <= 1;
		addr <= 0;
		i_data <= 8'h01;
		#1
		en <= 0;
		#100
		en <= 1;
		rw <= 0;
		addr <= 2;
		#2
		$display("%b", o_data);

		$finish;
	end

endmodule
