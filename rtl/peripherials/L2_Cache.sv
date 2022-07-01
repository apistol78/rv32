`include "CPU_Defines.sv"

`timescale 1ns/1ns

module L2_Cache(
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
	output bit o_ready,
	input [31:0] i_address,
	output bit [31:0] o_rdata,
	input [31:0] i_wdata
);

	localparam SIZE	= 18;
	localparam RANGE = 1 << SIZE;

	typedef enum bit [2:0]
	{
		IDLE,
		READ,
		READ_BUS,
		WRITE,
		PASS_THROUGH
	} state_t;

	// Cache memory.
	wire cache_initialized;
	bit cache_rw;
	bit [SIZE - 1:0] cache_address;
	bit [63:0] cache_wdata;
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

	state_t state = IDLE;
	bit [31:0] hit = 0;
	bit [31:0] miss = 0;

	always_ff @(posedge i_clock) begin
		case (state)
			IDLE: begin

				cache_rw <= 0;
				o_bus_rw <= 0;
				o_bus_request <= 0;
				o_ready <= 0;

				if (i_request && !o_ready) begin
					if (cache_initialized) begin
						cache_address <= i_address[(SIZE - 1) + 2:2];
						if (!i_rw) begin
							state <= READ;
						end
						else begin
							cache_rw <= 1;
							cache_wdata <= { i_wdata, i_address[31:2], 2'b01 };
							o_bus_rw <= 1;
							o_bus_request <= 1;
							o_bus_address <= i_address;
							o_bus_wdata <= i_wdata;
							state <= WRITE;
						end
					end
					else begin
						o_bus_rw <= i_rw;
						o_bus_request <= 1;
						o_bus_address <= i_address;
						o_bus_wdata <= i_wdata;
						state <= PASS_THROUGH;
					end
				end
			end

			READ: begin
				if (cache_rdata[31:0] == { i_address[31:2], 2'b01 } ) begin
					o_rdata <= cache_rdata[63:32];
					o_ready <= 1;
					hit <= hit + 1;
					state <= IDLE;
				end
				else begin
					o_bus_request <= 1;
					o_bus_address <= i_address;
					miss <= miss + 1;
					state <= READ_BUS;
				end
			end

			READ_BUS: begin
				if (i_bus_ready) begin
					cache_rw <= 1;
					cache_address <= i_address[(SIZE - 1) + 2:2];
					cache_wdata <= { i_bus_rdata, i_address[31:2], 2'b01 };
					o_rdata <= i_bus_rdata;
					o_ready <= 1;
					o_bus_request <= 0;
					state <= IDLE;
				end
			end

			WRITE: begin
				if (i_bus_ready) begin
					cache_rw <= 0;
					o_bus_rw <= 0;
					o_bus_request <= 0;
					o_ready <= 1;
					state <= IDLE;
				end
			end

			PASS_THROUGH: begin
				if (i_bus_ready) begin
					if (!i_rw) begin
						o_rdata <= i_bus_rdata;
					end
					o_bus_rw <= 0;
					o_bus_request <= 0;
					o_ready <= 1;
					state <= IDLE;
				end
			end

			default:
				state <= IDLE;

		endcase
	end

endmodule
