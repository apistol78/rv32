`include "CPU_Defines.v"

`timescale 1ns/1ns

module CPU_DCache(
	input wire i_reset,
	input wire i_clock,

	// Bus
	output reg o_bus_rw,
	output reg o_bus_request,
	input wire i_bus_ready,
	output reg [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,
	output reg [31:0] o_bus_wdata,

	// Input
	input wire i_rw,
	input wire i_request,
	output reg o_ready,
	input wire [31:0] i_address,
	output reg [31:0] o_rdata,
	input wire [31:0] i_wdata
);

	localparam SIZE	= 12;
	localparam RANGE = 1 << SIZE;

	localparam ST_IDLE = 0;
	localparam ST_RD_BUS = 1;
	localparam ST_RD_BUS_WB = 2;
	localparam ST_RD_BUS_UNCACHEABLE = 3;
	localparam ST_WR_BUS = 4;
	localparam ST_WAIT_END = 5;

	reg [7:0] state;
	reg [RANGE - 1:0] valid;

	reg cache_rw;
	wire [SIZE - 1:0] cache_label = i_address[(SIZE - 1) + 2:2];	// 2 lowest bits are always zero.
	reg [63:0] cache_wdata;
	wire [63:0] cache_rdata;

	// One cycle latency, important since
	// we rely on address only.
	BRAM #(
		.WIDTH(64),
		.SIZE(RANGE),
		.ADDR_LSH(0)
	) cache(
		.i_clock(i_clock),
		.i_request(1'b1),
		.i_rw(cache_rw),
		.i_address(cache_label),
		.i_wdata(cache_wdata),
		.o_rdata(cache_rdata)
	);

	wire is_cacheable = i_address < 32'h40000000;

	initial begin
		o_bus_rw = 0;
		o_bus_request = 0;
		o_bus_address = 0;
		o_bus_wdata = 0;
		o_rdata = 0;
		state = ST_IDLE;
		valid = { RANGE{ 1'b0 } };
		cache_rw = 0;
		cache_wdata = 0;
	end

	always @(*) begin
		o_ready = (i_request && state == ST_WAIT_END);
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			o_bus_rw <= 0;
			o_bus_request <= 0;
			o_bus_address <= 0;
			o_bus_wdata <= 0;
			o_rdata <= 0;
			state <= ST_IDLE;
			valid <= { RANGE{ 1'b0 } };
			cache_rw <= 0;
			cache_wdata <= 0;			
		end
		else begin
			case (state)
				ST_IDLE: begin
					if (i_request) begin
						if (!i_rw) begin
							if (is_cacheable) begin
								if (valid[cache_label] && cache_rdata[31:0] == i_address) begin
									// Cache read hit.
									o_rdata <= cache_rdata[63:32];
									state <= ST_WAIT_END;
								end
								else begin
									// Cache read miss.
									if (valid[cache_label]) begin
										// Write back current cache line before
										// reading requested value.
										o_bus_rw <= 1;
										o_bus_request <= 1;
										o_bus_address <= cache_rdata[31:0];
										o_bus_wdata <= cache_rdata[63:32];
										state <= ST_RD_BUS_WB;
									end
									else begin
										// No valid cache line entry; just read
										// requested value directly.
										o_bus_rw <= 0;
										o_bus_request <= 1;
										o_bus_address <= i_address;
										state <= ST_RD_BUS;
									end
								end
							end
							else begin
								// Read from uncacheable memory.
								o_bus_rw <= 0;
								o_bus_request <= 1;
								o_bus_address <= i_address;
								state <= ST_RD_BUS_UNCACHEABLE;
							end
						end
						else begin
							if (is_cacheable) begin
								// Write to cache.
								cache_rw <= 1;
								cache_wdata <= { i_wdata, i_address };
								valid[cache_label] <= 1;

								if (!valid[cache_label] || cache_rdata[31:0] == i_address) begin
									// No need to flush cache line, same address or unused.
									state <= ST_WAIT_END;
								end
								else if (valid[cache_label]) begin
									// Cache line need to be written to memory.
									o_bus_rw <= 1;
									o_bus_request <= 1;
									o_bus_address <= cache_rdata[31:0];
									o_bus_wdata <= cache_rdata[63:32];
									state <= ST_WR_BUS;
								end
							end
							else begin
								// Write to uncacheable memory.
								o_bus_rw <= 1;
								o_bus_request <= 1;
								o_bus_address <= i_address;
								o_bus_wdata <= i_wdata;
								state <= ST_WR_BUS;
							end
						end
					end
				end

				// Read

				ST_RD_BUS: begin
					if (i_bus_ready) begin
						valid[cache_label] <= 1;
						o_bus_request <= 0;
						o_rdata <= i_bus_rdata;
						cache_rw <= 1;
						cache_wdata <= { i_bus_rdata, i_address };
						state <= ST_WAIT_END;
					end
				end

				ST_RD_BUS_WB: begin
					if (i_bus_ready) begin
						o_bus_rw <= 0;
						o_bus_request <= 1;	// \note We're keeping bus requested for next read.
						o_bus_address <= i_address;
						state <= ST_RD_BUS;
					end
				end

				ST_RD_BUS_UNCACHEABLE: begin
					if (i_bus_ready) begin
						o_bus_request <= 0;
						o_rdata <= i_bus_rdata;
						state <= ST_WAIT_END;
					end					
				end

				// Write

				ST_WR_BUS: begin
					cache_rw <= 0;
					if (i_bus_ready) begin
						o_bus_rw <= 0;
						o_bus_request <= 0;
						state <= ST_WAIT_END;
					end				
				end

				// Common

				// Wait until request ends.
				ST_WAIT_END: begin
					cache_rw <= 0;
					if (!i_request) begin
						state <= ST_IDLE;
					end
				end

			endcase
		end
	end


endmodule
