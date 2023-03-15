/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
`timescale 1ns / 1ps

module LRU_cache(
	input i_clock,

	// CPU
	input i_request,
	input i_rw,
	input [31:0] i_address,     // Byte address
	input [31:0] i_wdata,
	output bit [31:0] o_rdata,
	output bit o_ready,
	input i_oddeven,			// Cache line channel

	// SDRAM
	output bit o_sdram_request,
	output bit o_sdram_rw,
	output bit [31:0] o_sdram_address,
	output bit [127:0] o_sdram_wdata,
	input [127:0] i_sdram_rdata,
	input i_sdram_ready
);
	typedef enum bit [3:0]
	{
		IDLE,
		WRITE_BACK_0,
		WRITE_BACK_1,
		READ_LINE_0,
		READ_LINE_1,
		PROCESS_0,
		PROCESS_1,
		WAIT_END
	}
	state_t;

	typedef struct packed
	{
		bit [27:0] page;
		bit [127:0] data;
		bit valid;
		bit dirty;		
	} cache_line_t;

	state_t state = IDLE;
	cache_line_t line[1:0];

	bit ready = 0;
	bit [31:0] hit = 0;
	bit [31:0] miss = 0;

	wire [27:0] input_address_page = i_address[31:4];
	wire [1:0] input_address_dword = i_address[3:2];

	initial begin
		o_ready = 0;
		o_sdram_request = 0;

		line[0] = 0;
		line[1] = 0;
	end
	
	always_comb begin
	    o_ready = ready && i_request;
	end

	always_ff @(posedge i_clock) begin
		case (state)
			IDLE: begin
				if (i_request) begin
					if (line[0].valid && input_address_page == line[0].page) begin
						state <= PROCESS_0;
						hit <= hit + 1;
					end
					else if (line[1].valid && input_address_page == line[1].page) begin
						state <= PROCESS_1;
						hit <= hit + 1;
					end
					else begin
						if (i_oddeven == 0) begin
							if (line[0].valid && line[0].dirty)
								state <= WRITE_BACK_0;
							else
								state <= READ_LINE_0;
						end
						else begin
							if (line[1].valid && line[1].dirty)
								state <= WRITE_BACK_1;
							else
								state <= READ_LINE_1;
						end
						miss <= miss + 1;
					end
				end
			end

			WRITE_BACK_0: begin
				o_sdram_request <= 1;
				o_sdram_rw <= 1;
				o_sdram_address <= { line[0].page, 4'b0 };
				o_sdram_wdata <= line[0].data;
				if (o_sdram_request && i_sdram_ready) begin
					o_sdram_request <= 0;
					state <= READ_LINE_0;
				end
			end

			WRITE_BACK_1: begin
				o_sdram_request <= 1;
				o_sdram_rw <= 1;
				o_sdram_address <= { line[1].page, 4'b0 };
				o_sdram_wdata <= line[1].data;
				if (o_sdram_request && i_sdram_ready) begin
					o_sdram_request <= 0;
					state <= READ_LINE_1;
				end
			end

			READ_LINE_0: begin
				o_sdram_request <= 1;
				o_sdram_rw <= 0;
				o_sdram_address <= { input_address_page, 4'b0 };
				if (o_sdram_request && i_sdram_ready) begin
					o_sdram_request <= 0;
					line[0].page <= input_address_page;
					line[0].data <= i_sdram_rdata;
					line[0].valid <= 1;
					line[0].dirty <= 0;
					state <= PROCESS_0;
				end
			end

			READ_LINE_1: begin
				o_sdram_request <= 1;
				o_sdram_rw <= 0;
				o_sdram_address <= { input_address_page, 4'b0 };
				if (o_sdram_request && i_sdram_ready) begin
					o_sdram_request <= 0;
					line[1].page <= input_address_page;
					line[1].data <= i_sdram_rdata;
					line[1].valid <= 1;
					line[1].dirty <= 0;
					state <= PROCESS_1;
				end
			end

			PROCESS_0: begin
				if (!i_rw) begin
					case (input_address_dword)
						2'd0: o_rdata <= line[0].data[31:0];
						2'd1: o_rdata <= line[0].data[63:32];
						2'd2: o_rdata <= line[0].data[95:64];
						2'd3: o_rdata <= line[0].data[127:96];
					endcase
				end
				else begin
					case (input_address_dword)
						2'd0: line[0].data[31:0] <= i_wdata;
						2'd1: line[0].data[63:32] <= i_wdata;
						2'd2: line[0].data[95:64] <= i_wdata;
						2'd3: line[0].data[127:96] <= i_wdata;
					endcase
					line[0].dirty <= 1;
				end						
				ready <= 1;
				state <= WAIT_END;			
			end

			PROCESS_1: begin
				if (!i_rw) begin
					case (input_address_dword)
						2'd0: o_rdata <= line[1].data[31:0];
						2'd1: o_rdata <= line[1].data[63:32];
						2'd2: o_rdata <= line[1].data[95:64];
						2'd3: o_rdata <= line[1].data[127:96];
					endcase
				end
				else begin
					case (input_address_dword)
						2'd0: line[1].data[31:0] <= i_wdata;
						2'd1: line[1].data[63:32] <= i_wdata;
						2'd2: line[1].data[95:64] <= i_wdata;
						2'd3: line[1].data[127:96] <= i_wdata;
					endcase
					line[1].dirty <= 1;
				end						
				ready <= 1;
				state <= WAIT_END;			
			end

			WAIT_END: begin
				ready <= 1;
				if (!i_request) begin
					ready <= 0;
					state <= IDLE;
				end
			end

			default:
				state <= IDLE;
		endcase
	end

endmodule
