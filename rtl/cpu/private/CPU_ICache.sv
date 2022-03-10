`include "CPU_Defines.sv"

`timescale 1ns/1ns

`define ENABLE_ICACHE

module CPU_ICache#(
	parameter SIZE = 13
)(
	input i_reset,
	input i_clock,
	
	input [31:0] i_input_pc,
	output logic [31:0] o_rdata,
	output logic o_ready,
	input i_stall,

	// Bus
	output logic o_bus_request,
	input i_bus_ready,
	output [31:0] o_bus_address,
	input [31:0] i_bus_rdata
);

	localparam RANGE = 1 << SIZE;

	typedef enum
	{
		IDLE			= 0,
		READ_SETUP		= 1,
		READ_PREFETCH	= 2,
		READ_BUS		= 3
	} state_t;

	logic [3:0] next = 1 << IDLE;
	logic [3:0] state = 1 << IDLE;

	// Cache memory.
	wire cache_initialized;
	logic cache_rw = 0;
	logic [63:0] cache_wdata = 0;
	wire [63:0] cache_rdata;
	logic [31:0] cache_pc;
	wire [SIZE - 1:0] cache_label = cache_pc[(SIZE - 1):2];	// 2 lowest bits are always zero.

	// One cycle latency, important since
	// we rely on address only.
	BRAM_clear #(
		.WIDTH(64),
		.SIZE(RANGE),
		.ADDR_LSH(0)
	) cache(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.o_initialized(cache_initialized),
		.i_request(1'b1),
		.i_rw(cache_rw),
		.i_address(cache_label),
		.i_wdata(cache_wdata),
		.o_rdata(cache_rdata),
		.o_ready()
	);

	assign o_bus_address = i_input_pc;

	initial begin
		o_bus_request = 0;
	end

	always_ff @(posedge i_clock) begin
		state <= next;
	end
	
	always_comb begin
		next = 0;
	
		o_ready = 0;
		o_rdata = 32'h0;
		o_bus_request = 0;
		
		cache_rw = 0;
		cache_wdata = 0;
		cache_pc = i_input_pc;
	
		case (1'b1)
			state[IDLE]: begin
				if (!i_stall) begin
`ifdef ENABLE_ICACHE				
					if (cache_initialized) begin
						next[READ_SETUP] = 1;
					end
					else
`endif
					begin
						o_bus_request = 1;
						next[READ_BUS] = 1;
					end
				end
				else begin
					next[IDLE] = 1;
				end
			end
			
			state[READ_SETUP]: begin
				if (cache_rdata[31:0] == { i_input_pc[31:2], 2'b01 }) begin
					o_ready = 1;
					o_rdata = cache_rdata[63:32];
					cache_pc = i_input_pc + 4;
					next[READ_PREFETCH] = 1;
				end
				else begin
					o_bus_request = 1;
					next[READ_BUS] = 1;
				end
			end

			state[READ_PREFETCH]: begin
				if (!i_stall && cache_rdata[31:0] == { i_input_pc[31:2], 2'b01 }) begin
					o_ready = 1;
					o_rdata = cache_rdata[63:32];
					cache_pc = i_input_pc + 4;
					next[READ_PREFETCH] = 1;
				end
				else if (!i_stall) begin
					o_bus_request = 1;
					next[READ_BUS] = 1;
				end
				else begin
					next[IDLE] = 1;
				end			
			end

			state[READ_BUS]: begin
				o_bus_request = 1;
				if (i_bus_ready) begin
					cache_rw = 1;
					cache_wdata = { i_bus_rdata, { i_input_pc[31:2], 2'b01 } };
					o_ready = 1;
					o_rdata = i_bus_rdata;
					next[IDLE] = 1;
				end
				else begin
					next[READ_BUS] = 1;
				end
			end
		endcase
	end

endmodule
