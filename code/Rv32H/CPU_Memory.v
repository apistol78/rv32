`include "CPU_Defines.v"

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
	input wire [`TAG_SIZE] i_tag,
	input wire [4:0] i_inst_rd,
	input wire [31:0] i_rd,
	input wire i_mem_read,
	input wire i_mem_write,
	input wire i_mem_flush,
	input wire [2:0] i_mem_width,
	input wire i_mem_signed,
	input wire [31:0] i_mem_address,

	// Output
	output reg [`TAG_SIZE] o_tag,
	output reg [4:0] o_inst_rd,
	output reg [31:0] o_rd,
	output wire o_stall
);

	localparam STATE_IDLE			= 0;
	localparam STATE_READ			= 1;
	localparam STATE_WRITE_WORD		= 2;
	localparam STATE_WRITE_RMW_0	= 3;
	localparam STATE_WRITE_RMW_1	= 4;
	localparam STATE_FLUSH			= 5;

	// localparam STATE_RMW_READ			= 1;
	// localparam STATE_RMW_RST_REQUEST	= 2;
	// localparam STATE_RMW_WAIT_WRITE		= 3;

	reg [4:0] state;
	reg [4:0] next;
	
	reg dcache_rw;
	reg dcache_request;
	reg dcache_flush;
	wire dcache_ready;
	wire [31:0] dcache_address;
	wire [31:0] dcache_rdata;
	reg [31:0] dcache_wdata;

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

	initial begin
		state = STATE_IDLE;
		next = STATE_IDLE;
		dcache_rw = 0;
		dcache_request = 0;
		dcache_flush = 0;
		dcache_wdata = 0;
		o_tag = 0;
		o_inst_rd = 0;
		o_rd = 0;
	end

	// Stall pipeline if we perform a memory access.
	assign o_stall = (i_tag != o_tag) && (i_mem_read || i_mem_write || i_mem_flush);

	assign dcache_address = { i_mem_address[31:2], 2'b00 };

	wire [1:0] address_byte_index = i_mem_address[1:0];
	wire [7:0] bus_rdata_byte = dcache_rdata >> (address_byte_index * 8);
	wire [15:0] bus_rdata_half = dcache_rdata >> (address_byte_index * 8);

	reg [`TAG_SIZE] next_o_tag;
	reg [31:0] next_o_rd;
	reg [4:0] next_o_inst_rd;
	reg [31:0] rmw_rdata;
	reg [31:0] next_rmw_rdata;

	always @(posedge i_clock) begin
		if (i_reset) begin
			state <= STATE_IDLE;
			o_tag <= 0;
			o_inst_rd <= 0;
			o_rd <= 0;			
		end
		else begin
			o_tag <= next_o_tag;
			o_rd <= next_o_rd;
			o_inst_rd <= next_o_inst_rd;	

			rmw_rdata <= next_rmw_rdata;
			state <= next;
		end
	end

	always @(*) begin
		next = state;

		next_o_tag = o_tag;
		next_o_rd = o_rd;
		next_o_inst_rd = o_inst_rd;

		next_rmw_rdata = rmw_rdata;

		dcache_request = 0;
		dcache_rw = 0;
		dcache_wdata = 0;
		dcache_flush = 0;

		case (state)
			STATE_IDLE: begin
				if (i_tag != o_tag) begin
					if (i_mem_read) begin
						dcache_request = 1;
						next = STATE_READ;
					end
					else if (i_mem_write) begin
						dcache_request = 1;
						if (i_mem_width == 4) begin
							dcache_rw = 1;
							dcache_wdata = i_rd;
							next = STATE_WRITE_WORD;
						end
						else begin
							// Byte or half write, need to perform read-modify-write.
							next = STATE_WRITE_RMW_0;
						end
					end
					else if (i_mem_flush) begin
						dcache_request = 1;
						dcache_flush = 1;
						next = STATE_FLUSH;
					end
					else begin
						next_o_tag = i_tag;
						next_o_rd = i_rd;
						next_o_inst_rd = i_inst_rd;
					end
				end
			end

			STATE_FLUSH: begin
				dcache_request = 1;
				dcache_flush = 1;
				if (dcache_ready) begin
					//dcache_request = 0;
					//dcache_flush = 0;

					next_o_tag = i_tag;
					next_o_rd = i_rd;
					next_o_inst_rd = i_inst_rd;					

					next = STATE_IDLE;
				end
			end

			STATE_READ: begin
				dcache_request = 1;
				if (dcache_ready) begin
					next_o_tag = i_tag;

					case (i_mem_width)
						4: next_o_rd = dcache_rdata;
						2: next_o_rd = { { 16{ i_mem_signed & bus_rdata_half[15] } }, bus_rdata_half[15:0] };
						1: next_o_rd = { { 24{ i_mem_signed & bus_rdata_byte[ 7] } }, bus_rdata_byte[ 7:0] };
						default: next_o_rd = 0;
					endcase
					next_o_inst_rd = i_inst_rd;

					next = STATE_IDLE;
				end
			end

			STATE_WRITE_WORD: begin
				dcache_request = 1;
				dcache_rw = 1;
				dcache_wdata = i_rd;
				if (dcache_ready) begin
					next_o_tag = i_tag;
					next_o_rd = i_rd;
					next_o_inst_rd = i_inst_rd;
					next = STATE_IDLE;
				end
			end

			STATE_WRITE_RMW_0: begin
				dcache_request = 1;
				if (dcache_ready) begin
					next_rmw_rdata = dcache_rdata;
					next = STATE_WRITE_RMW_1;
				end
			end

			STATE_WRITE_RMW_1: begin
				dcache_request = 1;
				dcache_rw = 1;
				if (i_mem_width == 1) begin
					case ( address_byte_index  )
						2'd0: dcache_wdata = { rmw_rdata[31:24], rmw_rdata[23:16], rmw_rdata[15:8],      i_rd[7:0] };
						2'd1: dcache_wdata = { rmw_rdata[31:24], rmw_rdata[23:16],       i_rd[7:0], rmw_rdata[7:0] };
						2'd2: dcache_wdata = { rmw_rdata[31:24],        i_rd[7:0], rmw_rdata[15:8], rmw_rdata[7:0] };
						2'd3: dcache_wdata = {        i_rd[7:0], rmw_rdata[23:16], rmw_rdata[15:8], rmw_rdata[7:0] };
					endcase
				end
				else begin	// width must be 2
					case ( address_byte_index  )
						2'd0: dcache_wdata = { rmw_rdata[31:16],      i_rd[15:0] };
						2'd2: dcache_wdata = {       i_rd[15:0], rmw_rdata[15:0] };
						default:
							dcache_wdata = 0;
					endcase						
				end
				if (dcache_ready) begin
					next_o_tag = i_tag;
					next_o_rd = i_rd;
					next_o_inst_rd = i_inst_rd;
					next = STATE_IDLE;
				end					
			end

		endcase
	end

endmodule
