module ap_sram(

	// System interface.
	input wire iEN,
	input wire iRW,
	input wire [31:0] iADDR,
	output wire [31:0] oDATA,
	input wire [31:0] iDATA,

	// Hardware IO.
	output wire [17:0] iSRAM_A,
	inout wire [15:0] ioSRAM_D,
	output wire oSRAM_CE_n,
	output wire oSRAM_OE_n,
	output wire oSRAM_WE_n
);

	assign iSRAM_A = iADDR;
	assign oDATA = (iEN && iRW) ? ioSRAM_D : 32'hzzzzzzzz;
	assign ioSRAM_D = (iEN && !iRW) ? iDATA : 32'hzzzzzzzz;
	assign oSRAM_CE_n = !iEN;
	assign oSRAM_OE_n = iRW;
	assign oSRAM_WE_n = !iRW;

endmodule