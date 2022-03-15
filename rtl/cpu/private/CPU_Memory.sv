`include "CPU_Defines.sv"

`timescale 1ns/1ns

//`define ENABLE_WBUFFER

module CPU_Memory #(
	parameter DCACHE_SIZE = 14
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
	output o_busy,
	input execute_data_t i_data,

	// Output
	output memory_data_t o_data
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

`ifdef ENABLE_WBUFFER
	wire wbuffer_rw;
	wire wbuffer_request;
	wire wbuffer_ready;
	wire [31:0] wbuffer_address;
	wire [31:0] wbuffer_rdata;
	wire [31:0] wbuffer_wdata;

	CPU_WriteBuffer wbuffer(
		.i_reset(i_reset),
		.i_clock(i_clock),
	
		.o_bus_rw(o_bus_rw),
		.o_bus_request(o_bus_request),
		.i_bus_ready(i_bus_ready),
		.o_bus_address(o_bus_address),
		.i_bus_rdata(i_bus_rdata),
		.o_bus_wdata(o_bus_wdata),
		
		.i_rw(wbuffer_rw),
		.i_request(wbuffer_request),
		.o_ready(wbuffer_ready),
		.i_address(wbuffer_address),
		.o_rdata(wbuffer_rdata),
		.i_wdata(wbuffer_wdata)		
	);
`endif

	reg dcache_rw = 0;
	reg dcache_request = 0;
	reg dcache_flush = 0;
	wire dcache_ready;
	wire [31:0] dcache_address;
	wire [31:0] dcache_rdata;
	reg [31:0] dcache_wdata = 0;
	wire dcache_need_flush;

	// Only access SDRAM using DCACHE, since other are fast enough or periferials.
	wire dcache_cacheable = (i_data.mem_address[31:28] == 4'h2);

	generate
		if (DCACHE_SIZE > 0) begin

			CPU_DCache #(
				.SIZE(DCACHE_SIZE)
			) dcache(
				.i_reset(i_reset),
				.i_clock(i_clock),
			
`ifdef ENABLE_WBUFFER
				.o_bus_rw(wbuffer_rw),
				.o_bus_request(wbuffer_request),
				.i_bus_ready(wbuffer_ready),
				.o_bus_address(wbuffer_address),
				.i_bus_rdata(wbuffer_rdata),
				.o_bus_wdata(wbuffer_wdata),
`else
				.o_bus_rw(o_bus_rw),
				.o_bus_request(o_bus_request),
				.i_bus_ready(i_bus_ready),
				.o_bus_address(o_bus_address),
				.i_bus_rdata(i_bus_rdata),
				.o_bus_wdata(o_bus_wdata),
`endif
				.i_rw(dcache_rw),
				.i_request(dcache_request),
				.i_flush(dcache_flush),
				.o_ready(dcache_ready),
				.i_address(dcache_address),
				.o_rdata(dcache_rdata),
				.i_wdata(dcache_wdata),
				.i_cacheable(dcache_cacheable)
			);

			assign dcache_need_flush = 1'b1;

		end
		else begin

			assign o_bus_rw = dcache_rw;
			assign o_bus_request = dcache_request;
			assign dcache_ready = i_bus_ready;
			assign o_bus_address = dcache_address;
			assign dcache_rdata = i_bus_rdata;
			assign o_bus_wdata = dcache_wdata;

			assign dcache_need_flush = 1'b0;

		end
	endgenerate

	assign o_busy = busy;
	assign o_data = data;
	assign dcache_address = { i_data.mem_address[31:2], 2'b00 };

	wire [1:0] address_byte_index = i_data.mem_address[1:0];
	wire [7:0] bus_rdata_byte = dcache_rdata >> (address_byte_index * 8);
	wire [15:0] bus_rdata_half = dcache_rdata >> (address_byte_index * 8);

	logic busy;
	memory_data_t data = 0;
	state_t state = IDLE;
	logic [31:0] rmw_rdata = 0;

	always_comb begin
		busy = (i_data.tag != data.tag) && (i_data.mem_read || i_data.mem_write || i_data.mem_flush);
	end

	always_ff @(posedge i_clock) begin
		case (state)
			IDLE: begin
				dcache_request <= 0;
				dcache_rw <= 0;
				dcache_wdata <= 0;
				dcache_flush <= 0;

				if (i_data.tag != data.tag) begin
					if (i_data.mem_read) begin
						dcache_request <= 1;
						state <= READ;
					end
					else if (i_data.mem_write) begin
						dcache_request <= 1;
						if (i_data.mem_width == 4) begin
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
						data.tag <= i_data.tag;
						data.rd <= i_data.rd;
						data.inst_rd <= i_data.inst_rd;
					end
				end
			end

			FLUSH: begin
				if (dcache_ready) begin
					dcache_request <= 0;
					dcache_flush <= 0;
					data.tag <= i_data.tag;
					data.rd <= i_data.rd;
					data.inst_rd <= i_data.inst_rd;				
					state <= IDLE;
				end
			end

			READ: begin
				if (dcache_ready) begin
					dcache_request <= 0;
					data.tag <= i_data.tag;
					case (i_data.mem_width)
						4: data.rd <= dcache_rdata;
						2: data.rd <= { { 16{ i_data.mem_signed & bus_rdata_half[15] } }, bus_rdata_half[15:0] };
						1: data.rd <= { { 24{ i_data.mem_signed & bus_rdata_byte[ 7] } }, bus_rdata_byte[ 7:0] };
						default: data.rd <= 0;
					endcase
					data.inst_rd <= i_data.mem_inst_rd; //inst_rd;
					state <= IDLE;
				end
			end

			WRITE_WORD: begin
				if (dcache_ready) begin
					dcache_request <= 0;
					dcache_rw <= 0;
					data.tag <= i_data.tag;
					data.rd <= i_data.rd;
					data.inst_rd <= i_data.inst_rd;	
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
				if (i_data.mem_width == 1) begin
					case (address_byte_index)
						2'd0: dcache_wdata <= { rmw_rdata[31:24], rmw_rdata[23:16], rmw_rdata[15:8], i_data.rd[7:0] };
						2'd1: dcache_wdata <= { rmw_rdata[31:24], rmw_rdata[23:16],  i_data.rd[7:0], rmw_rdata[7:0] };
						2'd2: dcache_wdata <= { rmw_rdata[31:24],   i_data.rd[7:0], rmw_rdata[15:8], rmw_rdata[7:0] };
						2'd3: dcache_wdata <= {   i_data.rd[7:0], rmw_rdata[23:16], rmw_rdata[15:8], rmw_rdata[7:0] };
					endcase
				end
				else begin	// width must be 2
					case (address_byte_index)
						2'd0: dcache_wdata <= { rmw_rdata[31:16], i_data.rd[15:0] };
						2'd2: dcache_wdata <= {  i_data.rd[15:0], rmw_rdata[15:0] };
						default: dcache_wdata <= 0;
					endcase						
				end
				if (dcache_ready) begin
					dcache_request <= 0;
					dcache_rw <= 0;
					data.tag <= i_data.tag;
					data.rd <= i_data.rd;
					data.inst_rd <= i_data.inst_rd;	
					state <= IDLE;
				end					
			end

			default:
				state <= IDLE;

		endcase
	end

endmodule
