
// SRAM interface
module SRAM_interface(
	input wire i_reset,
	input wire i_clock,
	input wire i_clock125,
	input wire i_request,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
	output wire o_ready,

	output reg [17:0] SRAM_A,
	inout wire [15:0] SRAM_D,
	output reg SRAM_CE_n,
	output wire SRAM_OE_n,
	output reg SRAM_WE_n,
	output reg SRAM_LB_n,
	output reg SRAM_UB_n
);
	// Number of cycles for entire transaction, must to be multiple of 2.
	// 50 MHz -> 6
	localparam CYCLES = 8;

	reg [4:0] count;
	reg [15:0] wdata;
	
	initial begin
		count <= 0;
		SRAM_A <= 18'h0;
		SRAM_CE_n <= 0;
		SRAM_WE_n <= 1;
		SRAM_LB_n <= 0;
		SRAM_UB_n <= 0;
	end
	
	// Output 
	assign SRAM_D = (i_request && i_rw) ? wdata : 16'hz;

	// Input
	wire [15:0] sram_d = (i_request && !i_rw) ? SRAM_D : 16'h0;

	// Keep OE active for entire READ request.
	assign SRAM_OE_n = (i_request && !i_rw) ? 1'b0 : 1'b1;

	// Output ready signal.
	assign o_ready = i_request && count >= (CYCLES - 1);

	// Output 16-bit SRAM address, first low then high parts.
	always @(*) begin
		if (i_request) begin
			if (count < CYCLES / 2)
				SRAM_A = ((i_address >> 1) & 32'hfffffffe);
			else
				SRAM_A = ((i_address >> 1) & 32'hfffffffe) + 1;
		end
	end

	// Output SRAM write enable, memory is stored on positive edge.
	always @(*) begin
		if (i_request && i_rw) begin
			if (count == 0 || count == CYCLES / 2)
				SRAM_WE_n = 0;
			else
				SRAM_WE_n = 1;
		end
	end

	// Output 16-bit SRAM data, first low the high part.
	always @(*) begin
		if (i_request && i_rw) begin
			if (count < CYCLES / 2)
				wdata = i_wdata[15:0];
			else
				wdata = i_wdata[31:16];
		end
	end

	// Increment counter, store data read from SRAM.
	always @(posedge i_clock) begin
		if (i_reset) begin
			count <= 0;
		end
		else begin
			if (i_request) begin
				count <= count + 1;
				if (count == 1)
					o_rdata[15:0] <= sram_d;
				else if (count == CYCLES / 2 + 1)
					o_rdata[31:16] <= sram_d;
			end
			else
				count <= 0;
		end
	end
endmodule