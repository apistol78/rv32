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
	input wire [31:0] i_pc_next,
	input wire i_mem_read,
	input wire i_mem_write,
	input wire [2:0] i_mem_width,
	input wire i_mem_signed,
	input wire [31:0] i_mem_address,

	// Output
	output reg [`TAG_SIZE] o_tag,
	output reg [4:0] o_inst_rd,
	output reg [31:0] o_rd,
	output reg [31:0] o_pc_next,
	output wire o_stall,

	// Debug
	output wire [31:0] o_dcache_hit_count,
	output wire [31:0] o_dcache_miss_count
);

	localparam STATE_RMW_READ			= 1;
	localparam STATE_RMW_RST_REQUEST	= 2;
	localparam STATE_RMW_WAIT_WRITE		= 3;

	reg [4:0] state;
	
	reg dcache_rw;
	reg dcache_request;
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
		.o_ready(dcache_ready),
		.i_address(dcache_address),
		.o_rdata(dcache_rdata),
		.i_wdata(dcache_wdata),

		.o_dcache_hit_count(o_dcache_hit_count),
		.o_dcache_miss_count(o_dcache_miss_count)
	);

	initial begin
		state = 0;
		dcache_rw = 0;
		dcache_request = 0;
		dcache_wdata = 0;
		o_tag = 0;
		o_inst_rd = 0;
		o_rd = 0;
		o_pc_next = 0;
	end

	// Stall pipeline if we perform a memory access.
	assign o_stall = (i_tag != o_tag) && (i_mem_read || i_mem_write);

	assign dcache_address = { i_mem_address[31:2], 2'b00 };

	wire [1:0] address_byte_index = i_mem_address[1:0];
	wire [7:0] bus_rdata_byte = dcache_rdata >> (address_byte_index * 8);
	wire [15:0] bus_rdata_half = dcache_rdata >> (address_byte_index * 8);

	always @(posedge i_clock) begin
		if (i_reset) begin
			state <= 0;
			dcache_rw <= 0;
			dcache_request <= 0;
			dcache_wdata <= 0;
			o_tag <= 0;
			o_inst_rd <= 0;
			o_rd <= 0;
			o_pc_next <= 0;
		end
		else begin
			case(state)
				0: begin
					if (i_tag != o_tag) begin
						if (i_mem_read) begin
							// Single read cycle.
							dcache_request <= 1;
							if (dcache_ready) begin
								case (i_mem_width)
									4: o_rd <= dcache_rdata;
									2: o_rd <= { { 16{ i_mem_signed & bus_rdata_half[15] } }, bus_rdata_half[15:0] };
									1: o_rd <= { { 24{ i_mem_signed & bus_rdata_byte[ 7] } }, bus_rdata_byte[ 7:0] };
								endcase
								
								// Finish
								dcache_request <= 0;
								o_pc_next <= i_pc_next; 
								o_inst_rd <= i_inst_rd;
								o_tag <= i_tag;
							end
						end
						else if (i_mem_write) begin
							dcache_request <= 1;
							
							if (i_mem_width == 4) begin
								// Single write cycle.
								dcache_rw <= 1;
								dcache_wdata <= i_rd;

								if (dcache_ready) begin
								
									// Finish
									dcache_rw <= 0;
									dcache_request <= 0;
									o_pc_next <= i_pc_next; 
									o_inst_rd <= i_inst_rd;
									o_tag <= i_tag;
								end
							end
							else begin
								// Read-modify-write cycle.
								dcache_rw <= 0;
								state <= STATE_RMW_READ;
							end
						end
						else begin
							// Finish, no memory access.
							o_pc_next <= i_pc_next; 
							o_inst_rd <= i_inst_rd;
							o_rd <= i_rd;
							o_tag <= i_tag;
						end
					end
				end

				// =============
				// Read-modify-write
				STATE_RMW_READ: begin
					if (dcache_ready) begin
						// Modify value.
						dcache_request <= 0;
						dcache_rw <= 1;
						if (i_mem_width == 1) begin
							case ( address_byte_index  )
								2'd0: dcache_wdata <= { dcache_rdata[31:24], dcache_rdata[23:16], dcache_rdata[15:8],         i_rd[7:0] };
								2'd1: dcache_wdata <= { dcache_rdata[31:24], dcache_rdata[23:16],          i_rd[7:0], dcache_rdata[7:0] };
								2'd2: dcache_wdata <= { dcache_rdata[31:24],           i_rd[7:0], dcache_rdata[15:8], dcache_rdata[7:0] };
								2'd3: dcache_wdata <= {           i_rd[7:0], dcache_rdata[23:16], dcache_rdata[15:8], dcache_rdata[7:0] };
							endcase
						end
						else begin	// width must be 2
							case ( address_byte_index  )
								2'd0: dcache_wdata <= { dcache_rdata[31:16],         i_rd[15:0] };
								2'd2: dcache_wdata <= {          i_rd[15:0], dcache_rdata[15:0] };
								default:
									dcache_wdata <= 0;
							endcase						
						end
						state <= STATE_RMW_RST_REQUEST;
					end
				end

				STATE_RMW_RST_REQUEST: begin
					dcache_request <= 1;
					state <= STATE_RMW_WAIT_WRITE;
				end

				STATE_RMW_WAIT_WRITE: begin
					if (dcache_ready) begin
						// Finish
						dcache_request <= 0;
						dcache_rw <= 0;
						o_pc_next <= i_pc_next; 
						o_inst_rd <= i_inst_rd;
						o_tag <= i_tag;
						state <= 0;
					end
				end
			endcase
		end
	end

endmodule
