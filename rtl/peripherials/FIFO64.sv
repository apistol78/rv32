
`timescale 1ns/1ns

module FIFO64 #(
	parameter DEPTH = 64
)(
	input wire i_clock,

	output wire o_empty,
	output wire o_full,

	input wire i_write,
	input wire [63:0] i_wdata,

	input wire i_read,
	output wire [63:0] o_rdata
);

//`ifdef __TESTBENCH__

	reg [63:0] rdata;
	reg [63:0] data [0:DEPTH - 1];
	reg [$clog2(DEPTH) - 1:0] in;
	reg [$clog2(DEPTH) - 1:0] out;
	//reg last_write;
	//reg last_read;

	assign o_empty = (in == out) ? 1'b1 : 1'b0;
	assign o_full = (((in + 1) & (DEPTH - 1)) == out) ? 1'b1 : 1'b0;
	assign o_rdata = rdata;

	initial begin
		rdata = 0;
		in = 0;
		out = 0;
		//last_write = 0;
		//last_read = 0;
	end

	always @ (posedge i_clock) begin
		//if (i_write && !last_write) begin
		if (i_write) begin
			data[in] <= i_wdata;
			in <= (in + 1) & (DEPTH - 1);
		end
		//last_write <= i_write;

		//if (i_read && !last_read) begin
		if (i_read) begin
			rdata <= data[out];
			out <= (out + 1) & (DEPTH - 1);
		end
		//last_read <= i_read;
	end

/*
`else

	IP_FIFO64 fifo(
		.clock(i_clock),
		.data(i_wdata),
		.rdreq(i_read),
		.wrreq(i_write),
		.empty(o_empty),
		.full(o_full),
		.q(o_rdata)
	);

`endif
*/

endmodule
