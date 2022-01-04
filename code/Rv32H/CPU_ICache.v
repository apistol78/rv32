
module CPU_ICache(
	input wire i_reset,
	input wire i_clock,
	input wire i_request,
	input wire [31:0] i_address,
	output reg [31:0] o_rdata,
	output reg o_ready,

	output reg o_bus_request,
	output wire [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,
	input wire i_bus_ready
);
	reg cache_rw;
	reg [63:0] cache_wdata;
	wire [63:0] cache_rdata;
	wire cache_ready;

	BRAM #(
		.WIDTH(64),
		.SIZE(128),
		.ADDR_LSH(0)
	) cache(
		.i_clock(i_clock),
		.i_request(i_request),
		.i_rw(cache_rw),
		.i_address({ 25'b0, i_address[6:0] }),
		.i_wdata(cache_wdata),
		.o_rdata(cache_rdata),
		.o_ready(cache_ready)
	);

	assign o_bus_address = i_address;

	wire cache_hit = cache_rdata[31:0] == i_address;

	reg [3:0] state = 0;
	reg [3:0] next_state = 0;

	always @(*) begin
		case (state)
		0: begin
			o_ready = 0;
			o_bus_request = 0;
			if (i_request)
				next_state = 1;
		end

		1: begin
			if (cache_hit == 1'b1) begin
				o_rdata = cache_rdata[63:32];
				o_ready = 1;
				next_state = 0;
			end
			else begin
				o_bus_request = 1;
				next_state = 2;
			end
		end

		2: begin
			if (i_bus_ready) begin
				cache_rw = 1;
				cache_wdata = { i_bus_rdata, i_address };
				o_rdata = i_bus_rdata;
				o_ready = 1;
				next_state = 3;
			end
		end

		3: begin
			cache_rw = 0;
			o_ready = 0;
			o_bus_request = 0;
			next_state = 0;
		end
		endcase
	end

	always @(posedge i_clock) begin
		if (i_reset)
			state <= 0;
		else
			state <= next_state;
	end

endmodule
