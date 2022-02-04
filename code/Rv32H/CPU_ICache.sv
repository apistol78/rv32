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

	reg [3:0] next;
	reg [3:0] state;

	// Cache memory.
	wire cache_initialized;
	reg cache_rw;
	wire [SIZE - 1:0] cache_label = i_input_pc[(SIZE - 1):2];	// 2 lowest bits are always zero.
	reg [63:0] cache_wdata;
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

	initial begin
		o_bus_request = 0;

		next = 0;
		state = 0;

		cache_rw = 0;
		cache_wdata = 0;
	end

	assign o_bus_address = i_input_pc;
	
	always @(posedge i_clock) begin
		state <= next;
	end
	
	always @(*) begin
	
		next = state;
	
		o_ready = 0;
		o_rdata = 32'h0;
		o_bus_request = 0;
		
		cache_rw = 0;
		cache_wdata = 0;
	
		case (state)
			0: begin
				if (!i_stall) begin
					if (cache_initialized) begin
						next = 1;
					end
					else begin
						o_bus_request = 1;
						next = 2;
					end
				end
			end
			
			1: begin
				if (cache_rdata[31:0] == { i_input_pc[31:2], 1'b0, 1'b1 }) begin
					o_ready = 1;
					o_rdata = cache_rdata[63:32];
					next = 0;
				end
				else begin
					o_bus_request = 1;
					next = 2;
				end
			end
			
			2: begin
				o_bus_request = 1;
				if (i_bus_ready) begin
					cache_rw = 1;
					cache_wdata = { i_bus_rdata, { i_input_pc[31:2], 2'b01 } };
					o_ready = 1;
					o_rdata = i_bus_rdata;
					next = 0;
				end
			end
		endcase
	end

endmodule
