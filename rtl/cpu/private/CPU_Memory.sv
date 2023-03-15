/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_Memory #(
	parameter DCACHE_SIZE,
	parameter DCACHE_REGISTERED
)(
	input i_reset,
	input i_clock,

	// Bus
	output o_bus_rw,
	output o_bus_request,
	input i_bus_ready,
	output [31:0] o_bus_address,
	input [31:0] i_bus_rdata,
	output [31:0] o_bus_wdata,

	// Input
	output bit o_busy,
	input execute_data_t i_data,

	// Output
	output memory_data_t o_data,

	// Debug
	output [31:0] o_dcache_hit,
	output [31:0] o_dcache_miss
);

	typedef enum bit [2:0]
	{
		IDLE,
		READ,
		WRITE_WORD,
		WRITE_RMW_0,
		WRITE_RMW_1,
		FLUSH
	} state_t;

	bit dcache_rw = 0;
	bit dcache_request = 0;
	bit dcache_flush = 0;
	wire dcache_ready;
	wire [31:0] dcache_address;
	wire [31:0] dcache_rdata;
	bit [31:0] dcache_wdata = 0;
	wire dcache_need_flush;

	// Only access SDRAM using DCACHE, since other are fast enough or periferials.
	wire dcache_cacheable = (i_data.mem_address[31:28] == 4'h2);

	generate if (DCACHE_SIZE > 0 && DCACHE_REGISTERED != 0) begin

		CPU_DCache_Reg #(
			.SIZE(DCACHE_SIZE)
		) dcache(
			.i_reset(i_reset),
			.i_clock(i_clock),

			.o_bus_rw(o_bus_rw),
			.o_bus_request(o_bus_request),
			.i_bus_ready(i_bus_ready),
			.o_bus_address(o_bus_address),
			.i_bus_rdata(i_bus_rdata),
			.o_bus_wdata(o_bus_wdata),

			.i_rw(dcache_rw),
			.i_request(dcache_request),
			.i_flush(dcache_flush),
			.o_ready(dcache_ready),
			.i_address(dcache_address),
			.o_rdata(dcache_rdata),
			.i_wdata(dcache_wdata),
			.i_cacheable(dcache_cacheable),

			.o_hit(o_dcache_hit),
			.o_miss(o_dcache_miss)
		);

		assign dcache_need_flush = 1'b1;

	end endgenerate

	generate if (DCACHE_SIZE > 0 && DCACHE_REGISTERED == 0) begin

		CPU_DCache_Comb #(
			.SIZE(DCACHE_SIZE)
		) dcache(
			.i_reset(i_reset),
			.i_clock(i_clock),

			.o_bus_rw(o_bus_rw),
			.o_bus_request(o_bus_request),
			.i_bus_ready(i_bus_ready),
			.o_bus_address(o_bus_address),
			.i_bus_rdata(i_bus_rdata),
			.o_bus_wdata(o_bus_wdata),

			.i_rw(dcache_rw),
			.i_request(dcache_request),
			.i_flush(dcache_flush),
			.o_ready(dcache_ready),
			.i_address(dcache_address),
			.o_rdata(dcache_rdata),
			.i_wdata(dcache_wdata),
			.i_cacheable(dcache_cacheable),

			.o_hit(o_dcache_hit),
			.o_miss(o_dcache_miss)
		);

		assign dcache_need_flush = 1'b1;

	end endgenerate	

	generate if (DCACHE_SIZE == 0) begin

		assign o_bus_rw = dcache_rw;
		assign o_bus_request = dcache_request;
		assign dcache_ready = i_bus_ready;
		assign o_bus_address = dcache_address;
		assign dcache_rdata = i_bus_rdata;
		assign o_bus_wdata = dcache_wdata;

		assign dcache_need_flush = 1'b0;

		assign o_dcache_hit = 0;
		assign o_dcache_miss = 0;

	end endgenerate

	assign o_data = data;
	assign dcache_address = { i_data.mem_address[31:2], 2'b00 };

	wire [1:0] address_byte_index = i_data.mem_address[1:0];
	wire [7:0] bus_rdata_byte = dcache_rdata >> (address_byte_index * 8);
	wire [15:0] bus_rdata_half = dcache_rdata >> (address_byte_index * 8);
	
	memory_data_t data = 0;
	state_t state = IDLE;
	bit [31:0] rmw_rdata = 0;
	bit last_strobe = 0;

	always_comb begin
		o_busy =
			(
				(i_data.strobe != last_strobe) &&
				(i_data.mem_read || i_data.mem_write || i_data.mem_flush)
			);
	end

	always_ff @(posedge i_clock) begin
		unique case (state)
			IDLE: begin
				dcache_request <= 0;
				dcache_rw <= 0;
				dcache_wdata <= 0;
				dcache_flush <= 0;

				if (i_data.strobe != last_strobe) begin
					if (i_data.mem_read) begin
						dcache_request <= 1;
						state <= READ;
					end
					else if (i_data.mem_write) begin
						dcache_request <= 1;
						if (i_data.mem_width == `MEMW_4) begin
							dcache_rw <= 1;
							dcache_wdata <= i_data.rd;
							state <= WRITE_WORD;
						end
						else begin
							// Byte or half write, need to perform read-modify-write.
							state <= WRITE_RMW_0;
						end
					end
					else if (i_data.mem_flush && dcache_need_flush) begin
						dcache_request <= 1;
						dcache_flush <= 1;
						state <= FLUSH;
					end
					else begin
						data.rd <= i_data.rd;
						data.inst_rd <= i_data.inst_rd;
						data.strobe <= ~data.strobe;
						last_strobe <= i_data.strobe;
					end
				end
			end

			FLUSH: begin
				if (dcache_ready) begin
					dcache_request <= 0;
					dcache_flush <= 0;
					data.rd <= i_data.rd;
					data.inst_rd <= i_data.inst_rd;				
					data.strobe <= ~data.strobe;
					last_strobe <= i_data.strobe;
					state <= IDLE;
				end
			end

			READ: begin
				if (dcache_ready) begin
					dcache_request <= 0;
					case (i_data.mem_width)
						`MEMW_4: data.rd <= dcache_rdata;
						`MEMW_2: data.rd <= { { 16{ i_data.mem_signed & bus_rdata_half[15] } }, bus_rdata_half[15:0] };
						`MEMW_1: data.rd <= { { 24{ i_data.mem_signed & bus_rdata_byte[ 7] } }, bus_rdata_byte[ 7:0] };
						default: data.rd <= 0;
					endcase
					data.inst_rd <= i_data.mem_inst_rd;
					data.strobe <= ~data.strobe;
					last_strobe <= i_data.strobe;
					state <= IDLE;
				end
			end

			WRITE_WORD: begin
				if (dcache_ready) begin
					dcache_request <= 0;
					dcache_rw <= 0;
					data.rd <= i_data.rd;
					data.inst_rd <= i_data.inst_rd;	
					data.strobe <= ~data.strobe;
					last_strobe <= i_data.strobe;
					state <= IDLE;
				end
			end

			WRITE_RMW_0: begin
				if (dcache_ready) begin
					dcache_request <= 0;
					rmw_rdata <= dcache_rdata;
					state <= WRITE_RMW_1;
				end
			end

			WRITE_RMW_1: begin
				dcache_request <= 1;
				dcache_rw <= 1;
				if (i_data.mem_width == `MEMW_1) begin
					unique case (address_byte_index)
						2'd0: dcache_wdata <= { rmw_rdata[31:24], rmw_rdata[23:16], rmw_rdata[15:8], i_data.rd[7:0] };
						2'd1: dcache_wdata <= { rmw_rdata[31:24], rmw_rdata[23:16],  i_data.rd[7:0], rmw_rdata[7:0] };
						2'd2: dcache_wdata <= { rmw_rdata[31:24],   i_data.rd[7:0], rmw_rdata[15:8], rmw_rdata[7:0] };
						2'd3: dcache_wdata <= {   i_data.rd[7:0], rmw_rdata[23:16], rmw_rdata[15:8], rmw_rdata[7:0] };
					endcase
				end
				else begin	// width must be 2
					unique case (address_byte_index)
						2'd0: dcache_wdata <= { rmw_rdata[31:16], i_data.rd[15:0] };
						2'd2: dcache_wdata <= {  i_data.rd[15:0], rmw_rdata[15:0] };
						default: dcache_wdata <= 0;
					endcase						
				end
				if (dcache_ready) begin
					dcache_request <= 0;
					dcache_rw <= 0;
					data.rd <= i_data.rd;
					data.inst_rd <= i_data.inst_rd;	
					data.strobe <= ~data.strobe;
					last_strobe <= i_data.strobe;
					state <= IDLE;
				end					
			end

			default:
				state <= IDLE;

		endcase
	end

endmodule
