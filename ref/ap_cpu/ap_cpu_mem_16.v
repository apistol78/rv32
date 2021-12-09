/*

Interface CPU (32-bit addr/data) with Memory (32-bit addr, 16-bit data).

*/
module ap_cpu_mem_16(
	input wire iCLK,

	input wire iCPU_EN,
	input wire iCPU_RW,
	output reg oCPU_RDY,
	input wire [31:0] iCPU_ADDR,
	input wire [31:0] iCPU_WDATA,
	output reg [31:0] oCPU_RDATA,

	output reg oMEM_EN,
	output reg oMEM_RW,
	output reg [31:0] oMEM_ADDR,
	output reg [15:0] oMEM_WDATA,
	input wire [15:0] iMEM_RDATA
);

	reg hl = 0;

	initial begin
		oMEM_EN <= 1'b0;
		oMEM_RW <= 1'b0;
		oMEM_ADDR <= 32'b0;
		oMEM_WDATA <= 16'b0;
		oCPU_RDY <= 1'b0;
	end

	always @ (*) begin
		oMEM_EN = iCPU_EN;
		oMEM_RW = iCPU_RW;
		oMEM_ADDR = iCPU_ADDR * 32'd2 + hl;
	end

	always @ (negedge iCLK) begin
		if (iCPU_EN) begin
			if (!hl) begin
				oCPU_RDATA[31:16] <= iMEM_RDATA;
				oCPU_RDY <= 1'b0;
			end
			else begin
				oCPU_RDATA[15:0] <= iMEM_RDATA;
				oCPU_RDY <= 1'b1;
			end
			hl <= 1'b1;
		end
		else begin
			hl <= 1'b0;
		end
	end

endmodule
