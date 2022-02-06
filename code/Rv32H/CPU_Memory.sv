`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_Memory(
	input wire i_reset,
	input wire i_clock,

	// Bus
	output wire o_bus_rw,
	output wire o_bus_request,
	input wire i_bus_ready,
	output wire [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,
	output wire [31:0] o_bus_wdata,

	// Input
	output o_busy,
	input execute_data_t i_data,

	// Output
	output memory_data_t o_data
);

	localparam STATE_IDLE			= 0;
	localparam STATE_READ			= 1;
	localparam STATE_WRITE_WORD		= 2;
	localparam STATE_WRITE_RMW_0	= 3;
	localparam STATE_WRITE_RMW_1	= 4;
	localparam STATE_FLUSH			= 5;

	reg dcache_rw = 0;
	reg dcache_request = 0;
	reg dcache_flush = 0;
	wire dcache_ready;
	wire [31:0] dcache_address;
	wire [31:0] dcache_rdata;
	reg [31:0] dcache_wdata = 0;

	CPU_DCache dcache(
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
		.i_wdata(dcache_wdata)
	);
/*
	assign o_bus_rw = dcache_rw;
	assign o_bus_request = dcache_request;
	assign dcache_ready = i_bus_ready;
	assign o_bus_address = dcache_address;
	assign dcache_rdata = i_bus_rdata;
	assign o_bus_wdata = dcache_wdata;
*/

	assign o_busy = busy;
	assign o_data = data;
	assign dcache_address = { i_data.mem_address[31:2], 2'b00 };

	wire [1:0] address_byte_index = i_data.mem_address[1:0];
	wire [7:0] bus_rdata_byte = dcache_rdata >> (address_byte_index * 8);
	wire [15:0] bus_rdata_half = dcache_rdata >> (address_byte_index * 8);

	logic busy;
	memory_data_t data = 0;
	memory_data_t next_data = 0;
	logic [4:0] state = STATE_IDLE;
	logic [4:0] next_state = STATE_IDLE;
	logic [31:0] rmw_rdata = 0;
	logic [31:0] next_rmw_rdata = 0;

	always_comb begin
		busy = (i_data.tag != data.tag) && (i_data.mem_read || i_data.mem_write || i_data.mem_flush);
	end

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			data <= 0;
			state <= STATE_IDLE;
			rmw_rdata <= 0;
		end
		else begin
			data <= next_data;
			state <= next_state;
			rmw_rdata <= next_rmw_rdata;
		end
	end

	always_comb begin
		next_state = state;
		next_data = data;
		next_rmw_rdata = rmw_rdata;

		dcache_request = 0;
		dcache_rw = 0;
		dcache_wdata = 0;
		dcache_flush = 0;

		case (state)
			STATE_IDLE: begin
				if (i_data.tag != data.tag) begin
					if (i_data.mem_read) begin
						dcache_request = 1;
						next_state = STATE_READ;
					end
					else if (i_data.mem_write) begin
						dcache_request = 1;
						if (i_data.mem_width == 4) begin
							dcache_rw = 1;
							dcache_wdata = i_data.rd;
							next_state = STATE_WRITE_WORD;
						end
						else begin
							// Byte or half write, need to perform read-modify-write.
							next_state = STATE_WRITE_RMW_0;
						end
					end
					else if (i_data.mem_flush) begin
						dcache_request = 1;
						dcache_flush = 1;
						next_state = STATE_FLUSH;
					end
					else begin
						next_data.tag = i_data.tag;
						next_data.rd = i_data.rd;
						next_data.inst_rd = i_data.inst_rd;
					end
				end
			end

			STATE_FLUSH: begin
				dcache_request = 1;
				dcache_flush = 1;
				if (dcache_ready) begin
					next_data.tag = i_data.tag;
					next_data.rd = i_data.rd;
					next_data.inst_rd = i_data.inst_rd;				
					next_state = STATE_IDLE;
				end
			end

			STATE_READ: begin
				dcache_request = 1;
				if (dcache_ready) begin
					next_data.tag = i_data.tag;
					case (i_data.mem_width)
						4: next_data.rd  = dcache_rdata;
						2: next_data.rd  = { { 16{ i_data.mem_signed & bus_rdata_half[15] } }, bus_rdata_half[15:0] };
						1: next_data.rd  = { { 24{ i_data.mem_signed & bus_rdata_byte[ 7] } }, bus_rdata_byte[ 7:0] };
						default: next_data.rd  = 0;
					endcase
					next_data.inst_rd = i_data.inst_rd;
					next_state = STATE_IDLE;
				end
			end

			STATE_WRITE_WORD: begin
				dcache_request = 1;
				dcache_rw = 1;
				dcache_wdata = i_data.rd;
				if (dcache_ready) begin
					next_data.tag = i_data.tag;
					next_data.rd = i_data.rd;
					next_data.inst_rd = i_data.inst_rd;	
					next_state = STATE_IDLE;
				end
			end

			STATE_WRITE_RMW_0: begin
				dcache_request = 1;
				if (dcache_ready) begin
					next_rmw_rdata = dcache_rdata;
					next_state = STATE_WRITE_RMW_1;
				end
			end

			STATE_WRITE_RMW_1: begin
				dcache_request = 1;
				dcache_rw = 1;
				if (i_data.mem_width == 1) begin
					case ( address_byte_index  )
						2'd0: dcache_wdata = { rmw_rdata[31:24], rmw_rdata[23:16], rmw_rdata[15:8], i_data.rd[7:0] };
						2'd1: dcache_wdata = { rmw_rdata[31:24], rmw_rdata[23:16],  i_data.rd[7:0], rmw_rdata[7:0] };
						2'd2: dcache_wdata = { rmw_rdata[31:24],   i_data.rd[7:0], rmw_rdata[15:8], rmw_rdata[7:0] };
						2'd3: dcache_wdata = {   i_data.rd[7:0], rmw_rdata[23:16], rmw_rdata[15:8], rmw_rdata[7:0] };
					endcase
				end
				else begin	// width must be 2
					case ( address_byte_index  )
						2'd0: dcache_wdata = { rmw_rdata[31:16], i_data.rd[15:0] };
						2'd2: dcache_wdata = {  i_data.rd[15:0], rmw_rdata[15:0] };
						default:
							dcache_wdata = 0;
					endcase						
				end
				if (dcache_ready) begin
					next_data.tag = i_data.tag;
					next_data.rd = i_data.rd;
					next_data.inst_rd = i_data.inst_rd;	
					next_state = STATE_IDLE;
				end					
			end

		endcase
	end

endmodule
