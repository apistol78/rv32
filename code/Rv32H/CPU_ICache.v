
module CPU_ICache(
	input wire i_reset,
	input wire i_clock,
	
	input wire [7:0] i_input_tag,
	output reg [7:0] o_output_tag,
	input wire [31:0] i_address,
	output reg [31:0] o_rdata,

	output reg o_bus_request,
	output reg [31:0] o_bus_address,
	input wire [31:0] i_bus_rdata,
	input wire i_bus_ready
);

	reg [3:0] state;

	reg cache_rw;
	reg [6:0] cache_label;
	reg [63:0] cache_wdata;
	wire [63:0] cache_rdata;
	wire cache_ready;

	// One cycle latency, important since
	// we rely on address only.
	BRAM #(
		.WIDTH(64),
		.SIZE(128),
		.ADDR_LSH(0)
	) cache(
		.i_clock(i_clock),
		.i_request(1'b1),
		.i_rw(cache_rw),
		.i_address({ 25'b0, cache_label }),
		.i_wdata(cache_wdata),
		.o_rdata(cache_rdata),
		.o_ready(cache_ready)
	);

	initial begin
		o_output_tag = 0;
		o_bus_request = 0;
		o_bus_address = 0;

		state = 0;

		cache_rw = 0;
		cache_label = 0;
		cache_wdata = 0;
	end

	always @(posedge i_clock) begin

		case (state)
			0: begin
				if (i_input_tag != o_output_tag && !i_bus_ready) begin
					o_bus_request <= 1;
					o_bus_address <= i_address;
					state <= 1;
				end
			end

			1: begin
				if (i_bus_ready) begin
					o_bus_request <= 0;
					o_rdata <= i_bus_rdata;
					o_output_tag <= i_input_tag;
					state <= 0;
				end
			end
		endcase

	end




/*

	assign o_bus_address = i_address;

	reg [127:0] valid;
	reg [3:0] state;

	reg [7:0] last;

	//wire cache_valid = valid[cache_tag];

	//wire [31:0] cache_entry_address = cache_valid && cache_ready ? cache_rdata[31:0] : 32'hffff_ffff;
	//wire cache_hit = cache_entry_address == i_address;

	wire [6:0] i_cache_label = i_address[6:0];

	initial begin
		cache_rw = 0;
		cache_tag = 0;
		cache_wdata = 0;
		valid = 128'b0;
		state = 0;
		o_ready = 0;
		o_bus_request = 0;
		last = 8'h0;
	end

	always @(posedge i_clock) begin
		case (state)
			0: begin
				o_ready <= 0;
				if (i_tag != last) begin

					cache_label <= i_cache_label;
					
					if (valid[i_cache_label]) begin
						state <= 1;					
					end
					else begin
						o_bus_request <= 1;
						state <= 2;
					end

					last <= i_tag;
				end
			end

			1: begin
				if (cache_rdata[31:0] == i_address) begin
					o_rdata <= cache_rdata[63:32];
					o_ready <= 1;
					state <= 0;
				end
				else begin
					o_bus_request <= 1;
					state <= 2;
				end
			end

			2: begin
				if (i_bus_ready) begin
					cache_rw <= 1;
					cache_wdata <= { i_bus_rdata, i_address };

					o_rdata <= i_bus_rdata;
					o_ready <= 1;
					o_bus_request <= 0;

					state <= 3;
				end
			end

			3: begin
				valid[cache_tag] <= 1'b1;
				cache_rw <= 0;
				//cache_wdata <= 0;
				o_ready <= 0;
				state <= 0;
			end

		endcase

	end
*/

endmodule
