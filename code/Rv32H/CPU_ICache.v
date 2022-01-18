`include "CPU_Defines.v"

`timescale 1ns/1ns

module CPU_ICache(
	input wire i_reset,
	input wire i_clock,
	
	input wire [31:0] i_input_pc,
	output reg [31:0] o_output_pc,
	output reg [31:0] o_rdata,

	output reg o_bus_request,
	input wire i_bus_ready,
	output reg [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,

	// Debug
	output wire [31:0] o_icache_hit_count,
	output wire [31:0] o_icache_miss_count	
);

	localparam SIZE	= 10;
	localparam RANGE = 1 << SIZE;

	reg [3:0] state;
	reg [RANGE - 1:0] valid;
	reg [31:0] hit_count;
	reg [31:0] miss_count;

	assign o_icache_hit_count = hit_count;
	assign o_icache_miss_count = miss_count;

	// Cache memory.
	reg cache_rw;
	wire [SIZE - 1:0] cache_label = i_input_pc[(SIZE - 1):2];	// 2 lowest bits are always zero.
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
		.o_rdata(cache_rdata),
		.o_ready()
	);

	initial begin
		o_output_pc = 32'hffffffff;
		o_bus_request = 0;
		o_bus_address = 32'h0;

		state = 0;
		valid = { RANGE{ 1'b0 } };

		cache_rw = 0;
		cache_wdata = 0;

		hit_count = 0;
		miss_count = 0;
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			o_output_pc <= 32'hffffffff;
			o_bus_request <= 0;
			o_bus_address <= 32'h0;

			state <= 0;
			valid <= { RANGE{ 1'b0 } };

			cache_rw <= 0;
			cache_wdata <= 0;

			hit_count <= 0;
			miss_count <= 0;
		end
		else begin
			case (state)
				0: begin
					cache_rw <= 0;
					if (i_input_pc != o_output_pc) begin
						if (valid[cache_label]) begin
							state <= 1;
						end
						else begin
							miss_count <= miss_count + 1;
							o_bus_request <= 1;
							o_bus_address <= i_input_pc;
							state <= 2;
						end
					end
				end

				1: begin
					// Use cached data if valid.
					if (cache_rdata[31:0] == i_input_pc) begin
						hit_count <= hit_count + 1;
						o_rdata <= cache_rdata[63:32];
						o_output_pc <= i_input_pc;
						state <= 0;
					end
					// Read into prefetcher.
					else begin
						miss_count <= miss_count + 1;
						o_bus_request <= 1;
						o_bus_address <= i_input_pc;
						state <= 2;
					end
				end

				2: begin
					if (i_bus_ready) begin
						o_bus_request <= 0;

						o_rdata <= i_bus_rdata;
						o_output_pc <= i_input_pc;

						cache_rw <= 1;
						cache_wdata <= { i_bus_rdata, i_input_pc };
						valid[cache_label] <= 1;

						state <= 0;
					end
				end

			endcase
		end
	end

endmodule
