`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_DCache #(
	parameter SIZE = 14
)(
	input i_reset,
	input i_clock,

	// Bus
	output bit o_bus_rw,
	output bit o_bus_request,
	input i_bus_ready,
	output bit [31:0] o_bus_address,
	input [31:0] i_bus_rdata,
	output bit [31:0] o_bus_wdata,

	// Input
	input i_rw,
	input i_request,
	input i_flush,
	output bit o_ready,
	input [31:0] i_address,
	output bit [31:0] o_rdata,
	input [31:0] i_wdata,
	input i_cacheable
);

	localparam RANGE = 1 << SIZE;

	typedef enum bit [3:0]
	{
		IDLE			= 4'd0,
		FLUSH_SETUP		= 4'd1,
		FLUSH_CHECK		= 4'd2,
		FLUSH_WRITE		= 4'd3,
		PASS_THROUGH	= 4'd4,
		WRITE_SETUP		= 4'd5,
		WRITE_WAIT		= 4'd6,
		READ_SETUP		= 4'd7,
		READ_WB_WAIT	= 4'd8,
		READ_BUS_WAIT	= 4'd9,
		INITIALIZE		= 4'd10
	} state_t;

	state_t state = INITIALIZE;
	state_t next = INITIALIZE;

	bit [SIZE:0] flush_address = 0;
	bit [SIZE:0] next_flush_address = 0;

	// Debug, only for verilated.
`ifdef __VERILATOR__
	bit [31:0] hit = 0;
	bit [31:0] next_hit = 0;
	bit [31:0] miss = 0;
	bit [31:0] next_miss = 0;
`endif

	// Cache memory.
	bit cache_rw = 0;
	bit [SIZE - 1:0] cache_address = 0;
	bit [63:0] cache_wdata;
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
		.i_address(cache_address),
		.i_wdata(cache_wdata),
		.o_rdata(cache_rdata),
		.o_ready()
	);

	wire cache_entry_valid = cache_rdata[0];
	wire cache_entry_dirty = cache_rdata[1];
	wire [31:0] cache_entry_address = { cache_rdata[31:2], 2'b00 };
	wire [31:0] cache_entry_data = cache_rdata[63:32];

	initial begin
		o_bus_rw = 0;
		o_bus_request = 0;
		o_bus_address = 0;
		o_bus_wdata = 0;
		o_rdata = 0;
	end

	always_ff @(posedge i_clock) begin
		state <= next;
		flush_address <= next_flush_address;

`ifdef __VERILATOR__
		hit <= next_hit;
		miss <= next_miss;
`endif		
	end

	always_comb begin
		next = state;
		next_flush_address = flush_address;

		o_bus_rw = 0;
		o_bus_address = 0;
		o_bus_request = 0;
		o_bus_wdata = 0;

		o_rdata = 0;
		o_ready = 0;

		cache_rw = 0;
		cache_wdata = 0;
		cache_address = i_address[(SIZE - 1) + 2:2];
		
`ifdef __VERILATOR__
		next_hit = hit;
		next_miss = miss;
`endif

		case (state)
			IDLE: begin
				if (i_request) begin
					if (i_flush) begin
						next_flush_address = 0;
						next = FLUSH_SETUP;
					end
					else if (i_cacheable) begin
						if (!i_rw)
							next = READ_SETUP;
						else begin
							next = WRITE_SETUP;
						end
					end
					else begin
						o_bus_rw = i_rw;
						o_bus_address = i_address;
						o_bus_request = i_request;
						o_bus_wdata = i_wdata;
						o_rdata = i_bus_rdata;
						next = PASS_THROUGH;
					end
				end
			end

			// ================
			// FLUSH
			// ================
			FLUSH_SETUP: begin
				cache_address = flush_address;
				if (flush_address < RANGE)
					next = FLUSH_CHECK;
				else begin
					o_ready = 1;
					next = IDLE;
				end
			end

			FLUSH_CHECK: begin
				cache_address = flush_address;
				if (cache_entry_dirty) begin
					o_bus_rw = 1;
					o_bus_address = cache_entry_address;
					o_bus_request = 1;
					o_bus_wdata = cache_entry_data;
					next = FLUSH_WRITE;
				end
				else begin
					next_flush_address = flush_address + 1;
					next = FLUSH_SETUP;
				end
			end

			FLUSH_WRITE: begin
				cache_address = flush_address;
				o_bus_rw = 1;
				o_bus_address = cache_entry_address;
				o_bus_request = 1;
				o_bus_wdata = cache_entry_data;
				if (i_bus_ready) begin
					cache_rw = 1;
					cache_wdata = { cache_entry_data, cache_entry_address[31:2], 2'b01 };
					next_flush_address = flush_address + 1;
					next = FLUSH_SETUP;
				end
			end

			// ================
			// NOT INITIALIZED
			// ================

			// Cache not initialized, pass through to bus.
			PASS_THROUGH: begin
				o_bus_rw = i_rw;
				o_bus_address = i_address;
				o_bus_request = i_request;
				o_bus_wdata = i_wdata;
				o_rdata = i_bus_rdata;
				o_ready = i_bus_ready;
				if (!i_request) begin
					next = IDLE;
				end
			end

			// ================
			// WRITE
			// ================

			// Write, write back if necessary.
			WRITE_SETUP: begin
				if (cache_entry_dirty && cache_entry_address != i_address) begin
					o_bus_rw = 1;
					o_bus_address = cache_entry_address;
					o_bus_request = 1;
					o_bus_wdata = cache_entry_data;
					next = WRITE_WAIT;
`ifdef __VERILATOR__
					next_miss = miss + 1;
`endif
				end
				else begin
					cache_rw = 1;
					cache_wdata = { i_wdata, i_address[31:2], 2'b11 };
					o_ready = 1;
					next = IDLE;
`ifdef __VERILATOR__
					next_hit = hit + 1;
`endif
				end
			end

			// Wait until write back finish.
			WRITE_WAIT: begin
				o_bus_rw = 1;
				o_bus_address = cache_entry_address;
				o_bus_request = 1;
				o_bus_wdata = cache_entry_data;
				if (i_bus_ready) begin
					cache_rw = 1;
					cache_wdata = { i_wdata, i_address[31:2], 2'b11 };
					o_ready = 1;
					next = IDLE;
				end
			end

			// ================
			// READ
			// ================

			// Check if cache entry valid, if not then read from bus.
			READ_SETUP: begin
				o_rdata = cache_entry_data;
				if (cache_entry_valid && cache_entry_address == i_address) begin
					o_ready = 1;
					next = IDLE;
`ifdef __VERILATOR__
					next_hit = hit + 1;
`endif
				end
				else begin
					if (/* cache_entry_valid && */ cache_entry_dirty) begin
						o_bus_rw = 1;
						o_bus_address = cache_entry_address;
						o_bus_request = 1;
						o_bus_wdata = cache_entry_data;
						next = READ_WB_WAIT;
					end
					else begin
						o_bus_address = i_address;
						o_bus_request = 1;
						next = READ_BUS_WAIT;
					end
`ifdef __VERILATOR__
					next_miss = miss + 1;
`endif
				end
			end

			// Write previous entry back to bus.
			READ_WB_WAIT: begin
				o_bus_rw = 1;
				o_bus_address = cache_entry_address;
				o_bus_request = 1;
				o_bus_wdata = cache_entry_data;
				if (i_bus_ready) begin
					o_bus_rw = 0;
					o_bus_address = i_address;
					next = READ_BUS_WAIT;
				end
			end

			// Wait until new data read from bus.
			READ_BUS_WAIT: begin
				o_bus_rw = 0;
				o_bus_address = i_address;
				o_bus_request = 1;
				o_rdata = i_bus_rdata;
				if (i_bus_ready) begin
					cache_rw = 1;
					cache_wdata = { i_bus_rdata, i_address[31:2], 2'b01 };
					o_ready = 1;
					next = IDLE;
				end
			end

			// ================
			// INITIALIZE
			// ================

			INITIALIZE: begin
				if (flush_address < RANGE) begin
					cache_rw = 1'b1;
					cache_wdata = 32'hffff_fff0;
					cache_address = flush_address;
					next_flush_address = flush_address + 1;
				end
				else begin
					next_flush_address = 0;
					next = IDLE;
				end
			end

			default: begin
				next = IDLE;
			end
		endcase

		// Re-initialize cache at reset.
		if (i_reset) begin
			next = INITIALIZE;
			next_flush_address = 0;
		end
	end

endmodule
