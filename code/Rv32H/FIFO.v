module FIFO(
	output wire o_empty,
	output wire o_full,

	input wire i_write,
	input wire [7:0] i_wdata,

	input wire i_read,
	output wire [7:0] o_rdata
);
	reg [7:0] data [0:63];
	reg [7:0] in = 0;
	reg [7:0] out = 0;

	assign o_empty = (in == out);
	assign o_full = (((out + 1) & 63) == in);
	assign o_rdata = data[out];

	always @ (posedge i_write) begin
		if (!o_full) begin
			data[in] <= i_wdata;
			in <= (in + 1) & 63;
		end
	end

	always @ (posedge i_read) begin
		if (!o_empty) begin
			out <= (out + 1) & 63;
		end
	end

endmodule
