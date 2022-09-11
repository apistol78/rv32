
`timescale 1ns/1ns

module SRAM_interface(
	input i_reset,
	input i_clock,
	input i_request,
	input i_rw,
	input [31:0] i_address,
	input [31:0] i_wdata,
	output bit [31:0] o_rdata,
	output o_ready,

	output bit [17:0] SRAM_A,
	inout [15:0] SRAM_D,
	output SRAM_CE_n,
	output bit SRAM_OE_n,
	output bit SRAM_WE_n,
	output SRAM_LB_n,
	output SRAM_UB_n
);

	// Number of cycles for entire transaction.
	localparam CYCLES = 10;	// Work with 10 but not 14?....

	bit [5:0] count;
	bit [15:0] wdata;
	
	initial begin
		count = 0;
	end

	assign SRAM_CE_n = 1'b0;
	assign SRAM_LB_n = 1'b0;
	assign SRAM_UB_n = 1'b0;
	
	// Output 
	assign SRAM_D = i_rw ? wdata : 16'hz;

	// Input
	wire [15:0] sram_d = !i_rw ? SRAM_D : 16'h0;

	// Output ready signal.
	assign o_ready = i_request && count >= (CYCLES - 1);
	
	always_comb begin
		SRAM_OE_n = i_rw;
		
		SRAM_WE_n = 1'b1;
		if (i_rw) begin
			if (count == 2 || count == CYCLES / 2 + 2)
				SRAM_WE_n = 1'b0;
			else
				SRAM_WE_n = 1'b1;
		end
		
		if (count < CYCLES / 2) begin
			SRAM_A = { i_address[18:1], 1'b0 };
		end
		else begin
			SRAM_A = { i_address[18:1], 1'b1 };
		end
		
		if (count < CYCLES / 2) begin
			wdata = i_wdata[15:0];
		end
		else begin
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
				if (!i_rw) begin
					if (count == 2)
						o_rdata[15:0] <= sram_d;
					else if (count == CYCLES / 2 + 2)
						o_rdata[31:16] <= sram_d;
				end
			end
			else begin
				count <= 0;
			end
		end
	end

endmodule