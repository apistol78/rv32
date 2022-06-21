`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_DCache_Reg #(
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
		IDLE,
		FLUSH_SETUP,
		FLUSH_CHECK,
		FLUSH_WRITE,
		PASS_THROUGH,
		WRITE_SETUP,
		WRITE_WAIT,
		READ_SETUP,
		READ_WB_WAIT,
		READ_BUS_WAIT,
		INITIALIZE
	} state_t;

	state_t state = INITIALIZE;
	bit [SIZE:0] flush_address = 0;

	// Debug, only for verilated.
`ifdef __VERILATOR__
	bit [31:0] hit = 0;
	bit [31:0] miss = 0;
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
		.o_ready(),
		.o_valid()
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

		o_ready <= 1'b0;

		cache_rw <= 1'b0;
		cache_address <= i_address[(SIZE - 1) + 2:2];

		case (state)
			IDLE: begin
				if (i_request && !o_ready) begin
					if (i_flush) begin
						flush_address <= 0;
						state <= FLUSH_SETUP;
					end
					else if (i_cacheable) begin
						if (!i_rw) begin

							// Check "super hot" cache line first, since
							// RMW pattern access same address multiple times
							// it's a high probability this will be true.
							if (cache_entry_valid && cache_entry_address == i_address) begin
								o_rdata <= cache_entry_data;
								o_ready <= 1'b1;
`ifdef __VERILATOR__
								hit <= hit + 1;
`endif
							end
							else
								state <= READ_SETUP;

						end
						else begin

							// Check "super hot" cache line here as well,
							// see comment above.
							if (cache_entry_valid && cache_entry_address == i_address) begin
								cache_rw <= 1'b1;
								cache_wdata <= { i_wdata, i_address[31:2], 2'b11 };
								o_ready <= 1'b1;
`ifdef __VERILATOR__
								hit <= hit + 1;
`endif
							end
							else
								state <= WRITE_SETUP;

						end
					end
					else begin
						o_bus_rw <= i_rw;
						o_bus_address <= i_address;
						o_bus_request <= 1'b1;
						o_bus_wdata <= i_wdata;
						state <= PASS_THROUGH;
					end
				end
			end

			// ================
			// FLUSH
			// ================
			FLUSH_SETUP: begin
				cache_address <= flush_address;
				if (flush_address < RANGE)
					state <= FLUSH_CHECK;
				else begin
					o_ready <= 1'b1;
					state <= IDLE;
				end
			end

			FLUSH_CHECK: begin
				if (cache_entry_dirty) begin
					o_bus_rw <= 1'b1;
					o_bus_address <= cache_entry_address;
					o_bus_request <= 1'b1;
					o_bus_wdata <= cache_entry_data;
					state <= FLUSH_WRITE;
				end
				else begin
					flush_address <= flush_address + 1;
					state <= FLUSH_SETUP;
				end
			end

			FLUSH_WRITE: begin
				if (i_bus_ready) begin
					cache_rw <= 1'b1;
					cache_wdata <= { cache_entry_data, cache_entry_address[31:2], 2'b01 };
					flush_address <= flush_address + 1;
					o_bus_request <= 1'b0;
					state <= FLUSH_SETUP;
				end
			end

			// ================
			// NOT INITIALIZED
			// ================

			// Cache not initialized, pass through to bus.
			PASS_THROUGH: begin
				if (i_bus_ready) begin
					o_bus_request <= 1'b0;
					o_rdata <= i_bus_rdata;
					o_ready <= 1'b1;
					state <= IDLE;
				end
			end

			// ================
			// WRITE
			// ================

			// Write, write back if necessary.
			WRITE_SETUP: begin
				if (cache_entry_dirty && cache_entry_address != i_address) begin
					o_bus_rw <= 1'b1;
					o_bus_address <= cache_entry_address;
					o_bus_request <= 1'b1;
					o_bus_wdata <= cache_entry_data;
					state <= WRITE_WAIT;
`ifdef __VERILATOR__
					miss <= miss + 1;
`endif
				end
				else begin
					cache_rw <= 1'b1;
					cache_wdata <= { i_wdata, i_address[31:2], 2'b11 };
					o_ready <= 1'b1;
					state <= IDLE;
`ifdef __VERILATOR__
					hit <= hit + 1;
`endif
				end
			end

			// Wait until write back finish.
			WRITE_WAIT: begin
				if (i_bus_ready) begin
					cache_rw <= 1'b1;
					cache_wdata <= { i_wdata, i_address[31:2], 2'b11 };
					o_bus_request <= 1'b0;
					o_ready <= 1'b1;
					state <= IDLE;
				end
			end

			// ================
			// READ
			// ================

			// Check if cache entry valid, if not then read from bus.
			READ_SETUP: begin
				if (cache_entry_valid && cache_entry_address == i_address) begin
					o_rdata <= cache_entry_data;
					o_ready <= 1'b1;
					state <= IDLE;
`ifdef __VERILATOR__
					hit <= hit + 1;
`endif
				end
				else begin
					if (/* cache_entry_valid && */ cache_entry_dirty) begin
						o_bus_rw <= 1'b1;
						o_bus_address <= cache_entry_address;
						o_bus_request <= 1'b1;
						o_bus_wdata <= cache_entry_data;
						state <= READ_WB_WAIT;
					end
					else begin
						o_bus_rw <= 1'b0;
						o_bus_address <= i_address;
						o_bus_request <= 1'b1;
						state <= READ_BUS_WAIT;
					end
`ifdef __VERILATOR__
					miss <= miss + 1;
`endif
				end
			end

			// Write previous entry back to bus.
			READ_WB_WAIT: begin
				if (i_bus_ready) begin
					o_bus_request <= 1'b0;
					state <= READ_BUS_WAIT;
				end
			end

			// Wait until new data read from bus.
			READ_BUS_WAIT: begin
				o_bus_rw <= 1'b0;
				o_bus_address <= i_address;
				o_bus_request <= 1'b1;
				if (i_bus_ready) begin
					cache_rw <= 1'b1;
					cache_wdata <= { i_bus_rdata, i_address[31:2], 2'b01 };
					o_bus_request <= 1'b0;
					o_rdata <= i_bus_rdata;
					o_ready <= 1'b1;
					state <= IDLE;
				end
			end

			// ================
			// INITIALIZE
			// ================

			INITIALIZE: begin
				if (flush_address < RANGE) begin
					cache_rw <= 1'b1;
					cache_wdata <= 32'hffff_fff0;
					cache_address <= flush_address;
					flush_address <= flush_address + 1;
				end
				else begin
					flush_address <= 0;
					state <= IDLE;
				end
			end

			default: begin
				state <= IDLE;
			end
		endcase

		// Re-initialize cache at reset.
		if (i_reset) begin
			state <= INITIALIZE;
			flush_address <= 0;
		end
	end

endmodule
