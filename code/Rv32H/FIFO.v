module FIFO #(
	parameter DEPTH = 64
)(
	input wire i_clock_rd,
	input wire i_clock_wr,

	output wire o_empty,
	output wire o_full,

	input wire i_write,
	input wire [7:0] i_wdata,

	input wire i_read,
	output wire [7:0] o_rdata
);

//`ifdef __ICARUS__

	reg [7:0] rdata;
	reg [7:0] data [0:DEPTH - 1];
	reg [$clog2(DEPTH) - 1:0] in;
	reg [$clog2(DEPTH) - 1:0] out;
	reg last_write;
	reg last_read;

	assign o_empty = (in == out) ? 1'b1 : 1'b0;
	assign o_full = (((in + 1) & (DEPTH - 1)) == out) ? 1'b1 : 1'b0;
	assign o_rdata = rdata;

	initial begin
		rdata <= 0;
		in <= 0;
		out <= 0;
		last_write <= 0;
		last_read <= 0;
	end

	// \fixme We're using RD clock for both read and write.
	always @ (posedge i_clock_rd) begin
		if (i_write && !last_write) begin
			data[in] <= i_wdata;
			in <= (in + 1) & (DEPTH - 1);
		end
		last_write <= i_write;

		if (i_read && !last_read) begin
			rdata <= data[out];
			out <= (out + 1) & (DEPTH - 1);
		end
		last_read <= i_read;
	end

//`else
/*
	assign o_full = 1'b0;

	IP_FIFO fifo(
		.data(i_wdata),
		.rdclk(i_clock_rd),
		.rdreq(i_read),
		.wrclk(i_clock_wr),
		.wrreq(i_write),
		.q(o_rdata),
		.rdempty(o_empty)
	);
*/
//`endif

endmodule
