
// Fake SRAM module, for testing wait signal.
module SRAM_tb(
	input wire i_clock,
	input wire i_enable,
	input wire i_rw,
	input wire [17:0] i_address,
	input wire [15:0] i_wdata,
	output reg [15:0] o_rdata,
	output reg o_ready
);

	reg [15:0] data [0:32'h0040_0000 - 1];

	initial begin
		o_ready = 0;
	end

	always @ (posedge i_clock) begin
		if (i_enable) begin
			if (!i_rw) begin
				$display("mem read ready %x, value %x", i_address, data[i_address]);
				o_rdata <= data[i_address];
				o_ready <= 1;
			end
			else begin
				$display("mem write ready %x, value %x", i_address, i_wdata);
				data[i_address] <= i_wdata;
				o_ready <= 1;
			end
		end
		else begin
			o_ready <= 0;
		end
	end

endmodule