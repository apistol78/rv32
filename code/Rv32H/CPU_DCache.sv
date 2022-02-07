`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_DCache(
	input i_reset,
	input i_clock,

	// Bus
	output reg o_bus_rw,
	output reg o_bus_request,
	input i_bus_ready,
	output reg [31:0] o_bus_address,
	input [31:0] i_bus_rdata,
	output reg [31:0] o_bus_wdata,

	// Input
	input i_rw,
	input i_request,
	input i_flush,
	output reg o_ready,
	input [31:0] i_address,
	output reg [31:0] o_rdata,
	input [31:0] i_wdata
);

	localparam SIZE	= 14;
	localparam RANGE = 1 << SIZE;

	typedef enum
	{
		IDLE,
		FLUSH_SETUP,
		FLUSH_CHECK,
		FLUSH_WRITE,
		PASS_THROUGH,
		WRITE_SETUP,
		WRITE_WAIT,
		READ_SETUP,
		READ_WB_WAIT,
		READ_BUS_WAIT
	} state_t;

	state_t state = IDLE;
	state_t next = IDLE;

	reg [SIZE:0] flush_address = 0;
	reg [SIZE:0] next_flush_address = 0;

	// Cache memory.
	wire cache_initialized;
	reg cache_rw = 0;
	reg [SIZE - 1:0] cache_address = 0;
	reg [63:0] cache_wdata;
	wire [63:0] cache_rdata;

	// One cycle latency, important since
	// we rely on address only.
	BRAM_clear #(
		.WIDTH(64),
		.SIZE(RANGE),
		.ADDR_LSH(0),
		.CLEAR_VALUE(32'hffff_fff0)
	) cache(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.o_initialized(cache_initialized),
		.i_request(1'b1),
		.i_rw(cache_rw),
		.i_address(cache_address),
		.i_wdata(cache_wdata),
		.o_rdata(cache_rdata),
		.o_ready()
	);

	// Only access SDRAM using DCACHE, since other are fast enough or periferials.
	wire is_cacheable = i_address[31:28] == 4'h2;
	
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
		
		case (state)
			IDLE: begin
				if (i_request) begin
					if (i_flush) begin
						next_flush_address = 0;
						next = FLUSH_SETUP;
					end
					else if (cache_initialized && is_cacheable) begin
						if (!i_rw)
							next = READ_SETUP;
						else begin
							next = WRITE_SETUP;
						end
					end
					else begin
						o_bus_rw = i_rw;
						o_bus_address = i_address;
						o_bus_request = 1;
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
				if (cache_entry_valid && cache_entry_dirty) begin
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
				if (i_bus_ready) begin
					o_ready = 1;
				end
				if (!i_request)
					next = IDLE;
			end

			// ================
			// WRITE
			// ================

			// Write, write back if necessary.
			WRITE_SETUP: begin
				if (cache_entry_valid && cache_entry_dirty && cache_entry_address != i_address) begin
					o_bus_rw = 1;
					o_bus_address = cache_entry_address;
					o_bus_request = 1;
					o_bus_wdata = cache_entry_data;
					next = WRITE_WAIT;
				end
				else begin
					cache_rw = 1;
					cache_wdata = { i_wdata, i_address[31:2], 2'b11 };
					o_ready = 1;
					next = IDLE;
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
				end
				else begin
					if (cache_entry_valid && cache_entry_dirty) begin
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

		endcase
	end

endmodule
