module FIFO #(
	parameter DEPTH = 64
)(
	input wire i_reset,
	input wire i_clock,

	output wire o_empty,
	output wire o_full,

	input wire i_write,
	input wire [7:0] i_wdata,

	input wire i_read,
	output reg [7:0] o_rdata
);
	reg [7:0] data [0:DEPTH - 1];
	reg [$clog2(DEPTH) - 1:0] in;
	reg [$clog2(DEPTH) - 1:0] out;
	reg last_write;
	reg last_read;

	assign o_empty = (in == out) ? 1'b1 : 1'b0;
	assign o_full = (((in + 1) & (DEPTH - 1)) == out) ? 1'b1 : 1'b0;

	initial begin
		in <= 0;
		out <= 0;
		last_write <= 0;
		last_read <= 0;
		o_rdata <= 0;
	end

	always @ (posedge i_clock, posedge i_reset) begin
		if (i_reset) begin
			in <= 0;
			out <= 0;
			last_write <= 0;
			last_read <= 0;
			o_rdata <= 0;
		end
		else begin
			if (i_write && !last_write) begin
				data[in] <= i_wdata;
				in <= (in + 1) & (DEPTH - 1);
			end
			last_write <= i_write;

			if (i_read && !last_read) begin
				o_rdata <= data[out];
				out <= (out + 1) & (DEPTH - 1);
			end
			last_read <= i_read;
		end
	end

endmodule
