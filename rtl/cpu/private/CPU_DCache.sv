`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_DCache #(
	parameter SIZE = 14
)(
	input i_reset,
	input i_clock,

	// Bus
	output logic o_bus_rw,
	output logic o_bus_request,
	input i_bus_ready,
	output logic [31:0] o_bus_address,
	input [31:0] i_bus_rdata,
	output logic [31:0] o_bus_wdata,

	// Input
	input i_rw,
	input i_request,
	input i_flush,
	output logic o_ready,
	input [31:0] i_address,
	output logic [31:0] o_rdata,
	input [31:0] i_wdata,
	input i_cacheable
);

	localparam RANGE = 1 << SIZE;

	typedef enum
	{
		IDLE			= 0,
		FLUSH_SETUP		= 1,
		FLUSH_CHECK		= 2,
		FLUSH_WRITE		= 3,
		PASS_THROUGH	= 4,
		WRITE_SETUP		= 5,
		WRITE_WAIT		= 6,
		READ_SETUP		= 7,
		READ_WB_WAIT	= 8,
		READ_BUS_WAIT	= 9
	} state_t;

	logic [9:0] state = 1 << IDLE;
	logic [9:0] next = 1 << IDLE;

	logic [SIZE:0] flush_address = 0;
	logic [SIZE:0] next_flush_address = 0;

	// Cache memory.
	wire cache_initialized;
	logic cache_rw = 0;
	logic [SIZE - 1:0] cache_address = 0;
	logic [63:0] cache_wdata;
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
		next = 0;
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
		
		case (1'b1)	// synthesis full_case
			state[IDLE]: begin
				if (i_request) begin
					if (i_flush) begin
						if (cache_initialized) begin
							next_flush_address = 0;
							next[FLUSH_SETUP] = 1;
						end
						else begin
							o_ready = 1;
							next[IDLE] = 1;
						end
					end
					else if (cache_initialized && i_cacheable) begin
						if (!i_rw)
							next[READ_SETUP] = 1;
						else begin
							next[WRITE_SETUP] = 1;
						end
					end
					else begin
						o_bus_rw = i_rw;
						o_bus_address = i_address;
						o_bus_request = 1;
						o_bus_wdata = i_wdata;
						o_rdata = i_bus_rdata;
						next[PASS_THROUGH] = 1;
					end
				end
				else begin
					next[IDLE] = 1;
				end
			end

			// ================
			// FLUSH
			// ================
			state[FLUSH_SETUP]: begin
				cache_address = flush_address;
				if (flush_address < RANGE)
					next[FLUSH_CHECK] = 1;
				else begin
					o_ready = 1;
					next[IDLE] = 1;
				end
			end

			state[FLUSH_CHECK]: begin
				cache_address = flush_address;
				if (/*cache_entry_valid && */cache_entry_dirty) begin
					o_bus_rw = 1;
					o_bus_address = cache_entry_address;
					o_bus_request = 1;
					o_bus_wdata = cache_entry_data;
					next[FLUSH_WRITE] = 1;
				end
				else begin
					next_flush_address = flush_address + 1;
					next[FLUSH_SETUP] = 1;
				end
			end

			state[FLUSH_WRITE]: begin
				cache_address = flush_address;
				o_bus_rw = 1;
				o_bus_address = cache_entry_address;
				o_bus_request = 1;
				o_bus_wdata = cache_entry_data;
				if (i_bus_ready) begin
					cache_rw = 1;
					cache_wdata = { cache_entry_data, cache_entry_address[31:2], 2'b01 };
					next_flush_address = flush_address + 1;
					next[FLUSH_SETUP] = 1;
				end
				else begin
					next[FLUSH_WRITE] = 1;
				end
			end

			// ================
			// NOT INITIALIZED
			// ================

			// Cache not initialized, pass through to bus.
			state[PASS_THROUGH]: begin
				o_bus_rw = i_rw;
				o_bus_address = i_address;
				o_bus_request = i_request;
				o_bus_wdata = i_wdata;
				o_rdata = i_bus_rdata;
				if (i_bus_ready) begin
					o_ready = 1;
				end
				if (i_request) begin
					next[PASS_THROUGH] = 1;
				end
				else begin
					next[IDLE] = 1;
				end
			end

			// ================
			// WRITE
			// ================

			// Write, write back if necessary.
			state[WRITE_SETUP]: begin
				if (cache_entry_dirty && cache_entry_address != i_address) begin
					o_bus_rw = 1;
					o_bus_address = cache_entry_address;
					o_bus_request = 1;
					o_bus_wdata = cache_entry_data;
					next[WRITE_WAIT] = 1;
				end
				else begin
					cache_rw = 1;
					cache_wdata = { i_wdata, i_address[31:2], 2'b11 };
					o_ready = 1;
					next[IDLE] = 1;
				end
			end

			// Wait until write back finish.
			state[WRITE_WAIT]: begin
				o_bus_rw = 1;
				o_bus_address = cache_entry_address;
				o_bus_request = 1;
				o_bus_wdata = cache_entry_data;
				if (i_bus_ready) begin
					cache_rw = 1;
					cache_wdata = { i_wdata, i_address[31:2], 2'b11 };
					o_ready = 1;
					next[IDLE] = 1;
				end
				else begin
					next[WRITE_WAIT] = 1;
				end
			end

			// ================
			// READ
			// ================

			// Check if cache entry valid, if not then read from bus.
			state[READ_SETUP]: begin
				o_rdata = cache_entry_data;
				if (cache_entry_valid && cache_entry_address == i_address) begin
					o_ready = 1;
					next[IDLE] = 1;
				end
				else begin
					if (/* cache_entry_valid && */ cache_entry_dirty) begin
						o_bus_rw = 1;
						o_bus_address = cache_entry_address;
						o_bus_request = 1;
						o_bus_wdata = cache_entry_data;
						next[READ_WB_WAIT] = 1;
					end
					else begin
						o_bus_address = i_address;
						o_bus_request = 1;
						next[READ_BUS_WAIT] = 1;
					end
				end
			end

			// Write previous entry back to bus.
			state[READ_WB_WAIT]: begin
				o_bus_rw = 1;
				o_bus_address = cache_entry_address;
				o_bus_request = 1;
				o_bus_wdata = cache_entry_data;
				if (i_bus_ready) begin
					o_bus_rw = 0;
					o_bus_address = i_address;
					next[READ_BUS_WAIT] = 1;
				end
				else begin
					next[READ_WB_WAIT] = 1;
				end		
			end

			// Wait until new data read from bus.
			state[READ_BUS_WAIT]: begin
				o_bus_rw = 0;
				o_bus_address = i_address;
				o_bus_request = 1;
				o_rdata = i_bus_rdata;
				if (i_bus_ready) begin
					cache_rw = 1;
					cache_wdata = { i_bus_rdata, i_address[31:2], 2'b01 };
					o_ready = 1;
					next[IDLE] = 1;
				end
				else begin
					next[READ_BUS_WAIT] = 1;
				end
			end
		endcase
	end

endmodule
