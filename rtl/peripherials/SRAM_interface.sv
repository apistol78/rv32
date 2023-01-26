
`timescale 1ns/1ns

module SRAM_interface(
	input i_reset,
	input i_clock,
	input i_request,
	input i_rw,
	input [31:0] i_address,
	input [31:0] i_wdata,
	output bit [31:0] o_rdata,
	output bit o_ready,

	output bit [17:0] SRAM_A,
	inout [15:0] SRAM_D,
	output SRAM_CE_n,
	output bit SRAM_OE_n,
	output bit SRAM_WE_n,
	output SRAM_LB_n,
	output SRAM_UB_n
);

	// Number of cycles for entire transaction.
	localparam CYCLES = 4;
	localparam READ_OFFSET = 1;
	localparam WRITE_OFFSET = 1;

	bit [7:0] count;
	bit [15:0] wdata;
	
	initial begin
		count = 0;
	end

	assign SRAM_CE_n = 1'b0;
	assign SRAM_LB_n = 1'b0;
	assign SRAM_UB_n = 1'b0;
	
	// Output 
	assign SRAM_D = i_rw ? wdata : 16'hz;

	always_comb begin
		SRAM_OE_n = ~(i_request && !i_rw);

		SRAM_WE_n = 1'b1;
		if (i_request && i_rw) begin
			if (count == WRITE_OFFSET)
				SRAM_WE_n = 1'b0;
			else if (count == CYCLES / 2 + WRITE_OFFSET)
				SRAM_WE_n = 1'b0;		
		end

		if (count < CYCLES / 2) begin
			SRAM_A = { i_address[18:2], 1'b0 };
		end
		else begin
			SRAM_A = { i_address[18:2], 1'b1 };
		end

		if (count < CYCLES / 2) begin
			wdata = i_wdata[15:0];
		end
		else begin
			wdata = i_wdata[31:16];
		end

		o_ready = (i_request && count >= CYCLES) ? 1'b1 : 1'b0;
	end

	// Increment counter, store data read from SRAM.
	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			count <= 0;
		end
		else begin
			if (i_request) begin
				if (!i_rw) begin
					if (count == READ_OFFSET)
						o_rdata[15:0] <= SRAM_D;
					else if (count == CYCLES / 2 + READ_OFFSET)
						o_rdata[31:16] <= SRAM_D;
				end
			 	count <= count + 1;
			end
			else begin
				count <= 0;
			end
		end
	end

endmodule