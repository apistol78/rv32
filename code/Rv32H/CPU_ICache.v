`include "CPU_Defines.v"

`timescale 1ns/1ns

module CPU_ICache(
	input wire i_reset,
	input wire i_clock,
	
	input wire [`TAG_SIZE] i_input_tag,
	output reg [`TAG_SIZE] o_output_tag,
	input wire [31:0] i_address,
	output reg [31:0] o_rdata,

	output wire o_bus_request,
	input wire i_bus_ready,
	output wire [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,

	// Debug
	output wire [31:0] o_icache_hit_count,
	output wire [31:0] o_icache_miss_count	
);

	localparam SIZE	= 10;
	localparam RANGE = 1 << SIZE;

	reg [1:0] state;
	reg [RANGE - 1:0] valid;

	reg cache_rw;
	wire [SIZE - 1:0] cache_label = i_address[(SIZE - 1):2];	// 2 lowest bits are always zero.
	reg [63:0] cache_wdata;
	wire [63:0] cache_rdata;

	reg [31:0] hit_count;
	reg [31:0] miss_count;

	assign o_icache_hit_count = hit_count;
	assign o_icache_miss_count = miss_count;

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

	reg prefetch_request;
	wire prefetch_ready;
	wire [31:0] prefetch_rdata;
	CPU_Prefetch prefetch(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_request(prefetch_request),
		.o_ready(prefetch_ready),
		.i_address(i_address),
		.o_rdata(prefetch_rdata),

		.o_bus_request(o_bus_request),
		.i_bus_ready(i_bus_ready),
		.o_bus_address(o_bus_address),
		.i_bus_rdata(i_bus_rdata)
	);

	initial begin
		o_output_tag = 0;
		prefetch_request = 0;
		state = 0;
		valid = { RANGE{ 1'b0 } };
		cache_rw = 0;
		cache_wdata = 0;
		hit_count = 0;
		miss_count = 0;
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			o_output_tag <= 0;
			prefetch_request <= 0;
			state <= 0;
			valid <= { RANGE{ 1'b0 } };
			cache_rw <= 0;
			cache_wdata <= 0;
		end
		else begin
			case (state)
				0: begin
					if (i_input_tag != o_output_tag) begin
						if (valid[cache_label]) begin
							state <= 1;
						end
						else begin
							miss_count <= miss_count + 1;
							prefetch_request <= 1;
							state <= 2;
						end
					end
				end

				1: begin
					if (cache_rdata[31:0] == i_address) begin
						hit_count <= hit_count + 1;
						o_rdata <= cache_rdata[63:32];
						o_output_tag <= i_input_tag;
						state <= 0;
					end
					else begin
						miss_count <= miss_count + 1;
						prefetch_request <= 1;
						state <= 2;
					end
				end

				2: begin
					if (prefetch_ready) begin
						prefetch_request <= 0;
						o_rdata <= prefetch_rdata;
						o_output_tag <= i_input_tag;
						cache_rw <= 1;
						cache_wdata <= { prefetch_rdata, i_address };
						state <= 3;
					end
				end

				3: begin
					valid[cache_label] <= 1;
					cache_rw <= 0;
					state <= 0;
				end
			endcase
		end
	end

endmodule
