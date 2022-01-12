`include "CPU_Defines.v"

`timescale 1ns/1ns

module CPU_DCache(
	input wire i_reset,
	input wire i_clock,

	// Bus
	output reg o_bus_rw,
	output reg o_bus_request,
	input wire i_bus_ready,
	output wire [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,
	output wire [31:0] o_bus_wdata,

	// Input
	input wire i_rw,
	input wire i_request,
	output reg o_ready,
	input wire [31:0] i_address,
	output reg [31:0] o_rdata,
	input wire [31:0] i_wdata
);

	localparam SIZE	= 10;
	localparam RANGE = 1 << SIZE;

	reg [7:0] state;
	reg [RANGE - 1:0] valid;

	reg cache_rw;
	wire [SIZE - 1:0] cache_label = i_address[(SIZE - 1) + 2:2];	// 2 lowest bits are always zero.
	reg [63:0] cache_wdata;
	wire [63:0] cache_rdata;
	wire cache_ready;

	// One cycle latency, important since
	// we rely on address only.
	BRAM #(
		.WIDTH(64),
		.SIZE(256),
		.ADDR_LSH(0)
	) cache(
		.i_clock(i_clock),
		.i_request(1'b1),
		.i_rw(cache_rw),
		.i_address({ { (32 - SIZE){ 1'b0 } }, cache_label }),
		.i_wdata(cache_wdata),
		.o_rdata(cache_rdata),
		.o_ready(cache_ready)
	);

	assign o_bus_address = i_address;
	assign o_bus_wdata = i_wdata;

	initial begin
		o_bus_rw = 0;
		o_bus_request = 0;

		o_ready = 0;
		o_rdata = 0;

		state = 0;
		valid = { RANGE{ 1'b0 } };
		cache_rw = 0;
		cache_wdata = 0;
	end

	always @(posedge i_clock) begin
		case (state)
			0: begin
				o_ready <= 0;
				if (i_request) begin
					if (!i_rw) begin
						// \fixme Hack to only cache RAM and not mapped devices.
						if (i_address < 32'h40000000 && valid[cache_label]) begin
							state <= 1;
						end
						else begin
							o_bus_rw <= 0;
							o_bus_request <= 1;
							state <= 2;
						end
					end
					else begin
						o_bus_rw <= 1;
						o_bus_request <= 1;
						cache_rw <= 1;
						cache_wdata <= { i_wdata, i_address };
						state <= 4;
					end
				end
			end

			// Read

			1: begin
				if (cache_rdata[31:0] == i_address) begin
					// Cached data valid and matching.
					o_rdata <= cache_rdata[63:32];
					o_ready <= 1;
					state <= 5;
				end
				else begin
					// Cached data not matching, read new data from bus.
					o_bus_request <= 1;
					state <= 2;
				end
			end

			2: begin
				if (i_bus_ready) begin
					valid[cache_label] <= 1;
					o_bus_request <= 0;
					o_rdata <= i_bus_rdata;
					o_ready <= 1;
					cache_rw <= 1;
					cache_wdata <= { i_bus_rdata, i_address };
					state <= 3;
				end
			end

			3: begin
				cache_rw <= 0;
				state <= 5;
			end

			// Write

			4: begin
				if (i_bus_ready) begin
					valid[cache_label] <= 1;
					o_bus_rw <= 0;
					o_bus_request <= 0;
					o_ready <= 1;
					cache_rw <= 0;
					state <= 5;
				end
			end

			// Wait until request ends.
			5: begin
				if (!i_request) begin
					state <= 0;
					o_ready <= 0;
				end
			end

		endcase
	end


endmodule
