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
	input i_stall,

	// Bus
	output bit o_bus_request,
	input i_bus_ready,
	output [31:0] o_bus_address,
	input [31:0] i_bus_rdata,

	// Debug
	output [31:0] o_hit,
	output [31:0] o_miss
);

	localparam RANGE = 1 << SIZE;

	typedef enum bit [1:0]
	{
		IDLE		= 2'd0,
		READ_SETUP	= 2'd1,
		READ_BUS	= 2'd2,
		INITIALIZE	= 2'd3
	} state_t;

	state_t state = INITIALIZE;

	bit [SIZE:0] clear_address = 0;

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

	// Cache memory.
	bit cache_rw = 0;
	bit [63:0] cache_wdata = 0;
	wire [63:0] cache_rdata;
	bit [31:0] cache_pc;
	wire [SIZE - 1:0] cache_label = cache_pc[(SIZE - 1) + 2:2];	// 2 lowest bits are always zero.

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
		.o_ready(),
		.o_valid()
	);

	assign o_bus_address = i_input_pc;

	initial begin
		o_bus_request = 0;
	end

	always_ff @(posedge i_clock) begin

		o_ready <= 0;
		o_rdata <= 32'h0;
		o_bus_request <= 0;
		
		cache_rw <= 0;
		cache_wdata <= 0;

		case (state)
			IDLE: begin
				if (!i_stall) begin
					cache_pc <= i_input_pc;
					state <= READ_SETUP;
				end
			end
			
			READ_SETUP: begin
				if (!i_stall) begin
					if (cache_rdata[31:0] == { i_input_pc[31:2], 2'b01 }) begin
						o_ready <= 1;
						o_rdata <= cache_rdata[63:32];
						cache_pc <= i_input_pc + 4;
`ifdef __VERILATOR__
						hit <= hit + 1;
`endif
					end
					else begin
						o_bus_request <= 1;
						state <= READ_BUS;
`ifdef __VERILATOR__
						miss <= miss + 1;
`endif
					end
				end
				else begin
					state <= IDLE;		// We need to go back to IDLE, seems to become unstable if we stay.
				end
			end

			READ_BUS: begin
				o_bus_request <= 1;
				if (i_bus_ready) begin
					cache_pc <= i_input_pc;
					cache_rw <= 1;
					cache_wdata <= { i_bus_rdata, { i_input_pc[31:2], 2'b01 } };
					o_ready <= 1;
					o_rdata <= i_bus_rdata;
					o_bus_request <= 0;
					state <= IDLE;
				end
			end

			INITIALIZE: begin
				if (clear_address < RANGE) begin
					cache_pc <= { clear_address, 2'b00 };
					cache_rw <= 1;
					cache_wdata <= 32'h0000_0000;
					clear_address <= clear_address + 1;
				end
				else begin
					clear_address <= 0;
					state <= IDLE;
				end				
			end
		endcase

		// Re-initialize cache at reset.
		if (i_reset) begin
			state <= INITIALIZE;
			clear_address <= 0;
		end
	end

endmodule
