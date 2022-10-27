`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_ICache_Reg #(
	parameter SIZE = 13
)(
	input i_reset,
	input i_clock,
	
	input [31:0] i_input_pc,
	output bit [31:0] o_rdata,
	output bit o_ready,

	// Bus
	output bit o_bus_request,
	input i_bus_ready,
	output bit [31:0] o_bus_address,
	input [31:0] i_bus_rdata,

	// Debug
	output [31:0] o_hit,
	output [31:0] o_miss
);

	localparam RANGE = 1 << SIZE;
	localparam SET_BITS = SIZE;
	localparam TAG_BITS = 30 - SIZE;

	typedef struct packed
	{
		bit valid;
		bit [TAG_BITS-1:0] tag;
		bit [31:0] data;
	}
	cache_entry_t;

	// Debug, only for verilated.
`ifdef __VERILATOR__
	bit [31:0] hit = 0;
	bit [31:0] miss = 0;

	assign o_hit = hit;
	assign o_miss = miss;
`else
	assign o_hit = 0;
	assign o_miss = 0;
`endif

	bit initialized = 1'b0;
	bit [31:0] clear_set = 0;
	bit cache_invalid = 0;

	wire [SET_BITS-1:0] i_set = i_input_pc[(SET_BITS - 1) + 2 : 2];
	wire [TAG_BITS-1:0] i_tag = i_input_pc[(TAG_BITS - 1) + SET_BITS + 2: SET_BITS + 2];

	bit [SET_BITS-1:0] cache_rd_set;
	cache_entry_t cache_rd_rdata;
	bit cache_wr_request = 1'b0;
	bit [SET_BITS-1:0] cache_wr_set;
	cache_entry_t cache_wr_wdata;

	BRAM_dual #(
		.WIDTH($bits(cache_entry_t)),
		.SIZE(RANGE),
		.ADDR_LSH(0)
	) cache(
		.i_clock(i_clock),

		// Read port
		.i_pa_request(1'b1),
		.i_pa_rw(1'b0),
		.i_pa_address(cache_rd_set),
		.i_pa_wdata(0),
		.o_pa_rdata(cache_rd_rdata),
		.o_pa_ready(),

		// Write port
		.i_pb_request(cache_wr_request),
		.i_pb_rw(1'b1),
		.i_pb_address(cache_wr_set),
		.i_pb_wdata(cache_wr_wdata),
		.o_pb_rdata(),
		.o_pb_ready()
	);

	initial begin
		o_bus_request = 0;
	end

	// Latch last read set, so we know which line is actually active.
	bit [SET_BITS-1:0] set_r = ~0;
	always_ff @(posedge i_clock) begin
		set_r <= cache_rd_set;
	end

	// Compare read line with given input.
	always_comb begin
		o_ready = 0;
		o_rdata = 0;
		
		cache_rd_set = 0;
		cache_invalid = 1'b0;

		if (initialized) begin
			cache_rd_set = i_set;
			if (set_r == i_set) begin
				if (cache_rd_rdata.valid && cache_rd_rdata.tag == i_tag) begin
					o_ready = 1;
					o_rdata = cache_rd_rdata.data;
					cache_rd_set = i_set + 1;
				end
				else
					cache_invalid = 1'b1;
			end
		end
	end

	// Update cache if current line is invalid.
	always_ff @(posedge i_clock) begin
		cache_wr_request <= 1'b0;

		if (initialized) begin
`ifdef __VERILATOR__
			if (cache_rd_rdata.valid && cache_rd_rdata.tag == i_tag)
				hit <= hit + 1;
			else
				miss <= miss + 1;
`endif

			// Check if cache line is invalid and need to be refilled.
			if (
				o_bus_request == 1'b0 &&
				cache_invalid
			) begin
				cache_wr_set <= i_set;
				cache_wr_wdata.valid <= 1'b1;
				cache_wr_wdata.tag <= i_tag;
				o_bus_request <= 1'b1;
				o_bus_address <= i_input_pc;
			end

			// Wait until bus request finished, refill cache when bus ready.
			if (
				o_bus_request == 1'b1 &&
				i_bus_ready
			) begin
				cache_wr_request <= 1'b1;
				cache_wr_wdata.data <= i_bus_rdata;
				o_bus_request <= 1'b0;
			end
		end

		// Clear each cache line one-by-one.
		if (!initialized) begin
			if (clear_set < RANGE) begin
				cache_wr_request <= 1'b1;
				cache_wr_set <= clear_set;
				cache_wr_wdata.valid <= 1'b0;
				cache_wr_wdata.tag <= 0;
				cache_wr_wdata.data <= 0;
				clear_set <= clear_set + 1;
			end
			else begin
				initialized <= 1'b1;
			end
		end

		if (i_reset) begin
			initialized <= 1'b0;
			clear_set <= 0;
		end
	end

endmodule
