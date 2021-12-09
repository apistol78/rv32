module ap_soft_ram(
	input wire iEN,
	input wire iRW,
	input wire [31:0] iADDR,
	input wire [15:0] iWDATA,
	output reg [15:0] oRDATA
);

	reg [15:0] memory [0:64];

	always @ (*) begin
		oRDATA = (iEN && !iRW) ? memory[iADDR] : 16'hzzzz;
		if (iEN &&iRW) begin
			memory[iADDR] = iWDATA;
		end
	end

endmodule