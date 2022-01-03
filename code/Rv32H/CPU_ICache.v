
module CPU_ICache(
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

	reg [63:0] cache [0:63];

	wire [63:0] entry = cache[i_address[5:0]];

	assign o_bus_address = i_address;

	initial begin
		o_bus_request <= 0;
	end

	always @(posedge i_clock) begin
		if (i_request) begin
			if (entry[31:0] == i_address) begin
				o_rdata <= entry [63:32];
				o_ready <= 1;
			end
			else begin

				// Not in cache, need to fetch from bus.
				o_bus_request <= 1;

				if (i_bus_ready) begin

					cache[i_address[5:0]] <= { i_bus_rdata, i_address };
					o_rdata <= i_bus_rdata;
					o_ready <= 1;

				end
			end
		end
		else begin
			o_ready <= 0;
			o_bus_request <= 0;
		end
	end

endmodule
