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
	bit [31:0] clear_address = 0;

	bit [SIZE - 1:0] cache_rd_label;
	wire [63:0] cache_rd_rdata;

	bit cache_wr_request = 1'b0;
	bit [SIZE - 1:0] cache_wr_label;
	bit [63:0] cache_wr_wdata;

	BRAM_dual #(
		.WIDTH(64),
		.SIZE(RANGE),
		.ADDR_LSH(0)
	) cache(
		.i_clock(i_clock),

		// Read port
		.i_pa_request(1'b1),
		.i_pa_rw(1'b0),
		.i_pa_address(cache_rd_label),
		.i_pa_wdata(0),
		.o_pa_rdata(cache_rd_rdata),
		.o_pa_ready(),

		// Write port
		.i_pb_request(cache_wr_request),
		.i_pb_rw(1'b1),
		.i_pb_address(cache_wr_label),
		.i_pb_wdata(cache_wr_wdata),
		.o_pb_rdata(),
		.o_pb_ready()
	);

	initial begin
		o_bus_request = 0;
	end

	always_comb begin
		o_ready = 0;
		o_rdata = 0;

		if (initialized) begin
			cache_rd_label = i_input_pc[(SIZE - 1) + 2:2];	// 2 lowest bits are always zero.
			if (cache_rd_rdata[31:0] == { i_input_pc[31:2], 2'b01 }) begin
				o_ready = 1;
				o_rdata = cache_rd_rdata[63:32];
				cache_rd_label = i_input_pc[(SIZE - 1) + 2:2] + 1;
			end
		end
	end

	bit [31:0] pc_r = 0;

	always_ff @(posedge i_clock) begin
		cache_wr_request <= 1'b0;
		pc_r <= i_input_pc;

		if (initialized) begin
`ifdef __VERILATOR__
			if (cache_rd_rdata[31:0] == { i_input_pc[31:2], 2'b01 })
				hit <= hit + 1;
			else
				miss <= miss + 1;
`endif

			// Check if cache line is invalid and need to be refilled.
			if (
				o_bus_request == 1'b0 &&
				pc_r == i_input_pc &&
				cache_rd_rdata[31:0] != { i_input_pc[31:2], 2'b01 }
			) begin
				o_bus_request <= 1'b1;
				o_bus_address <= i_input_pc;
			end

			// Wait until bus request finished, refill cache when bus ready.
			if (
				o_bus_request == 1'b1 &&
				i_bus_ready
			) begin
				cache_wr_request <= 1'b1;
				cache_wr_label <= o_bus_address[(SIZE - 1) + 2:2];
				cache_wr_wdata <= { i_bus_rdata, { o_bus_address[31:2], 2'b01 } };
				o_bus_request <= 1'b0;
			end
		end
		else begin
			// Clear each cache line one-by-one.
			if (clear_address < RANGE) begin
				cache_wr_request <= 1'b1;
				cache_wr_label <= clear_address;
				cache_wr_wdata <= 64'h0;
				clear_address <= clear_address + 1;
			end
			else begin
				initialized <= 1'b1;
			end
		end
	end

endmodule
