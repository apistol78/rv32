/*

Interface CPU (32-bit addr/data) with Memory (32-bit addr, 32-bit data).

*/
module ap_cpu_mem_32(
	input wire iCLK,
	input wire iEN,
	input wire iRW,				// Data read/write
	input reg oRDY,			// IO request ready.
	input wire [31:0] iADDR,	// Address

	input wire [31:0] iDATA,	// Read data
	output reg [31:0] oDATA,		// Write data
);

	reg ram_en;
	reg [31:0] ram_addr;
	reg [15:0] ram_idata;
	wire [15:0] ram_odata;

	ap_soft_ram ram(
		.iEN(ram_en),
		.iRW(iRW),
		.iADDR(ram_addr),
		.iDATA(ram_idata),
		.oDATA(ram_odata)
	);

	reg last_iEN = 1'b0;


	always @ (posedge iCLK) begin
		
		if (iEN && !last_iEN) begin
			
			// Become enabled.

			ram_en <= 1'b1;
			ram_rw <= iRW;
			ram_addr <= iADDR;

			oRDY <= 1'b0;

		end

		if (state == ST_)


		last_iEN <= iEN;

	end

end
