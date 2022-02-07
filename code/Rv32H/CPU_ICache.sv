`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_ICache(
	input wire i_reset,
	input wire i_clock,
	
	input wire [31:0] i_input_pc,
	output reg [31:0] o_rdata,
	output reg o_ready,
	input wire i_stall,

	// Bus
	output reg o_bus_request,
	input wire i_bus_ready,
	output wire [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata
);

	localparam SIZE	= 12;
	localparam RANGE = 1 << SIZE;

	typedef enum
	{
		IDLE,
		READ_SETUP,
		READ_BUS
	} state_t;

	state_t next = IDLE;
	state_t state = IDLE;

	// Cache memory.
	wire cache_initialized;
	reg cache_rw = 0;
	wire [SIZE - 1:0] cache_label = i_input_pc[(SIZE - 1):2];	// 2 lowest bits are always zero.
	reg [63:0] cache_wdata = 0;
	wire [63:0] cache_rdata;

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
		next = state;
	
		o_ready = 0;
		o_rdata = 32'h0;
		o_bus_request = 0;
		
		cache_rw = 0;
		cache_wdata = 0;
	
		case (state)
			IDLE: begin
				if (!i_stall) begin
					if (cache_initialized) begin
						next = READ_SETUP;
					end
					else begin
						o_bus_request = 1;
						next = READ_BUS;
					end
				end
			end
			
			READ_SETUP: begin
				if (cache_rdata[31:0] == { i_input_pc[31:2], 1'b0, 1'b1 }) begin
					o_ready = 1;
					o_rdata = cache_rdata[63:32];
					next = IDLE;
				end
				else begin
					o_bus_request = 1;
					next = READ_BUS;
				end
			end
			
			READ_BUS: begin
				o_bus_request = 1;
				if (i_bus_ready) begin
					cache_rw = 1;
					cache_wdata = { i_bus_rdata, { i_input_pc[31:2], 2'b01 } };
					o_ready = 1;
					o_rdata = i_bus_rdata;
					next = IDLE;
				end
			end
		endcase
	end

endmodule
