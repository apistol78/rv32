module ap_soft_rom(
	input wire iEN,
	input wire [31:0] iADDR,
	output reg [15:0] oDATA
);

	reg [15:0] rom [0:699];
	initial $readmemh("rom.hex", rom);

	always @ (*) begin
		oDATA = iEN ? rom[iADDR] : 16'hzzzz;
	end

endmodule