`include "CPU_Defines.v"

`timescale 1ns/1ns

module CPU_ICache(
	input wire i_reset,
	input wire i_clock,
	
	input wire [`TAG_SIZE] i_input_tag,
	output reg [`TAG_SIZE] o_output_tag,
	input wire [31:0] i_address,
	output reg [31:0] o_rdata,

	output reg o_bus_request,
	input wire i_bus_ready,
	output wire [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata
);

	reg [1:0] state;
	reg [255:0] valid;

	reg cache_rw;
	wire [7:0] cache_label = i_address[7:0];
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
		.i_address({ 24'b0, cache_label }),
		.i_wdata(cache_wdata),
		.o_rdata(cache_rdata),
		.o_ready(cache_ready)
	);

	assign o_bus_address = i_address;

	initial begin
		o_output_tag = 0;
		o_bus_request = 0;
		state = 0;
		valid = 256'b0;
		cache_rw = 0;
		cache_wdata = 0;
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			o_output_tag <= 0;
			o_bus_request <= 0;
			state <= 0;
			valid <= 256'b0;
			cache_rw <= 0;
			cache_wdata <= 0;
		end
		else begin
			case (state)
				0: begin
					if (i_input_tag != o_output_tag) begin
						if (valid[cache_label]) begin
							state <= 1;
						end
						else begin
							o_bus_request <= 1;
							state <= 2;
						end
					end
				end

				1: begin
					if (cache_rdata[31:0] == i_address) begin
						o_rdata <= cache_rdata[63:32];
						o_output_tag <= i_input_tag;
						state <= 0;
					end
					else begin
						o_bus_request <= 1;
						state <= 2;
					end
				end

				2: begin
					if (i_bus_ready) begin
						o_bus_request <= 0;
						o_rdata <= i_bus_rdata;
						o_output_tag <= i_input_tag;
						cache_rw <= 1;
						cache_wdata <= { i_bus_rdata, i_address };
						state <= 3;
					end
				end

				3: begin
					valid[cache_label] <= 1;
					cache_rw <= 0;
					state <= 0;
				end
			endcase
		end
	end

endmodule
