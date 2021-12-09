module ap_spi_master_cpu(
	input wire iCLK,

	// Register interface.
	input wire iEN,
	input wire iRW,
	input wire iREQ,
	input wire [3:0] iADDR,
	input wire [7:0] iDATA,
	output wire [7:0] oDATA,

	// Hardware pins.
	output wire oSCK,
	output reg oCS,
	output reg oDC,
	output wire oMOSI,
	input wire iMISO
);

	/*
	|7|6|5|4|3|2|1|0|
	-----------------
	|0| 0 = Idle, 1 = Execute transaction
	|1| 0 = Data / 1 = Command
	|2| Chip select, 0 = Deselect, 1 = Select
	*/

	//reg spi_iCLK = 1'b0;
	assign spi_iCLK = iCLK;
	
	reg spi_iEN = 1'b0;
	wire spi_oRDY;
	reg [7:0] spi_iTX;
	wire [7:0] spi_oRX;
	ap_spi_master spi(
		.iCLK(spi_iCLK),
		.iEN(spi_iEN),
		.oRDY(spi_oRDY),
		.iTX(spi_iTX),
		.oRX(spi_oRX),
		.oSCK(oSCK),
		.oMOSI(oMOSI),
		.iMISO(iMISO)
	);

   reg [7:0] regs [0:2];

	initial begin
        regs[0] = 8'h00;
        regs[1] = 8'h00;
        regs[2] = 8'h00;
		  
		  oCS = 1'b0;
		  oDC = 1'b0;
	end
	
	assign oDATA = (iEN && !iRW) ? regs[iADDR] : 8'hzz;
	
	/*
	always @ (negedge iCLK) begin
		spi_iCLK <= !spi_iCLK;
	end
	*/

	always @ (posedge iCLK) begin
		if (iEN && iRW) begin
         regs[iADDR] <= iDATA;
		end

      spi_iEN <= regs[0][0];
      spi_iTX <= regs[1];
		  
		oCS <= regs[0][2];
		oDC <= regs[0][1];

      if (spi_oRDY) begin
			regs[0][0] <= 1'b0;
			regs[2] <= spi_oRX;
      end
	end

endmodule
