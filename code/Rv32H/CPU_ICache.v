`include "CPU_Defines.v"

`timescale 1ns/1ns

module CPU_ICache(
	input wire i_reset,
	input wire i_clock,
	
	input wire [31:0] i_input_pc,
	output reg [31:0] o_output_pc,
	output reg [31:0] o_rdata,

	output wire o_bus_request,
	input wire i_bus_ready,
	output wire [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata
);

	localparam SIZE	= 10;
	localparam RANGE = 1 << SIZE;

	reg [3:0] state;
	reg [RANGE - 1:0] valid;

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

	reg [31:0] prefetch_input_address;
	wire [31:0] prefetch_output_address;
	wire [31:0] prefetch_rdata;
	CPU_Prefetch prefetch(
		.i_reset(i_reset),
		.i_clock(i_clock),

		.i_address(prefetch_input_address),
		.o_address(prefetch_output_address),
		.o_rdata(prefetch_rdata),

		.o_bus_request(o_bus_request),
		.i_bus_ready(i_bus_ready),
		.o_bus_address(o_bus_address),
		.i_bus_rdata(i_bus_rdata)
	);

	initial begin
		o_output_pc = 32'hffff_ffff;
		prefetch_input_address = 32'h0000_0000;
		state = 0;
		valid = { RANGE{ 1'b0 } };
		cache_rw = 0;
		cache_wdata = 0;
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			o_output_pc <= 32'hffff_ffff;
			prefetch_input_address <= 32'h0000_0000;
			state <= 0;
			valid <= { RANGE{ 1'b0 } };
			cache_rw <= 0;
			cache_wdata <= 0;
		end
		else begin
			case (state)
				0: begin
					cache_rw <= 0;
					if (i_input_pc != o_output_pc) begin

						// #1 Pop from prefetcher
						if (prefetch_output_address == i_input_pc) begin
							o_rdata <= prefetch_rdata;
							o_output_pc <= i_input_pc;

							cache_rw <= 1;
					 		cache_wdata <= { prefetch_rdata, i_input_pc };
							valid[cache_label] <= 1;

							prefetch_input_address <= i_input_pc + 4;
						end
						// #2 Read from cache
						else if (valid[cache_label]) begin
							state <= 1;
						end
						// #3 Read into prefetcher
						else begin
							prefetch_input_address <= i_input_pc;
						end
					end
				end

				1: begin
					// Use cached data if valid.
					if (cache_rdata[31:0] == i_input_pc) begin
						o_rdata <= cache_rdata[63:32];
						o_output_pc <= i_input_pc;
					end
					// Read into prefetcher.
					else begin
						prefetch_input_address <= i_input_pc;
					end
					state <= 0;
				end

			endcase
		end
	end

endmodule
