
`timescale 1ns/1ns

module SRAM_interface(
	input i_reset,
	input i_clock,
	input i_request,
	input i_rw,
	input [31:0] i_address,
	input [31:0] i_wdata,
	output reg [31:0] o_rdata,
	output o_ready,

	output reg [17:0] SRAM_A,
	inout [15:0] SRAM_D,
	output SRAM_CE_n,
	output SRAM_OE_n,
	output reg SRAM_WE_n,
	output SRAM_LB_n,
	output SRAM_UB_n
);
	// Number of cycles for entire transaction, must to be multiple of 2.
	// 50 MHz -> 8
	localparam CYCLES = 8;

	logic [15:0] count;
	logic [15:0] wdata;
	
	initial begin
		count = 0;
		SRAM_A = 18'h0;
		SRAM_WE_n = 1;
	end

	assign SRAM_CE_n = 1'b0;
	assign SRAM_LB_n = 1'b0;
	assign SRAM_UB_n = 1'b0;
	
	// Output 
	assign SRAM_D = (i_request && i_rw) ? wdata : 16'hz;

	// Input
	wire [15:0] sram_d = (i_request && !i_rw) ? SRAM_D : 16'h0;

	// Keep OE active for entire READ request.
	assign SRAM_OE_n = (i_request && !i_rw) ? 1'b0 : 1'b1;

	// Output ready signal.
	assign o_ready = i_request && count >= (CYCLES - 1);

	// Output 16-bit SRAM address, first low then high parts.
	always_comb begin
		SRAM_A = 32'h0;
		
		if (i_request) begin
			if (count < CYCLES / 2)
				SRAM_A = (i_address >> 1) & 32'hfffffffe;
			else
				SRAM_A = ((i_address >> 1) & 32'hfffffffe) + 1;
		end
	end

	// Output SRAM write enable, memory is stored on positive edge.
	always_comb begin
		SRAM_WE_n = 1;

		if (i_request && i_rw) begin
			if (count == 0 || count == CYCLES / 2)
				SRAM_WE_n = 0;
			else
				SRAM_WE_n = 1;
		end
	end

	// Output 16-bit SRAM data, first low the high part.
	always_comb begin
		wdata = 32'h0;

		if (i_request && i_rw) begin
			if (count < CYCLES / 2)
				wdata = i_wdata[15:0];
			else
				wdata = i_wdata[31:16];
		end
	end

	// Increment counter, store data read from SRAM.
	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			count <= 0;
		end
		else begin
			if (i_request) begin
				count <= count + 1;
				if (count == 2)
					o_rdata[15:0] <= sram_d;
				else if (count == CYCLES / 2 + 2)
					o_rdata[31:16] <= sram_d;
			end
			else
				count <= 0;
		end
	end

endmodule