module FIFO(
	output reg o_empty,
	output reg o_full,

	input wire i_write,
	input wire [7:0] i_wdata,

	input wire i_read,
	output reg [7:0] o_rdata
);
	reg [7:0] data [0:63];
	reg [7:0] in = 0;
	reg [7:0] out = 0;

	always @ (*) begin
		o_empty <= (in == out);
		o_full <= (((in + 1) & 63) == out);
	end

	always @ (posedge i_write) begin
		if (!o_full) begin
			data[in] <= i_wdata;
			in <= (in + 1) & 63;
		end
	end

	always @ (posedge i_read) begin
		if (!o_empty) begin
			o_rdata <= data[out];
			out <= (out + 1) & 63;
		end
	end

endmodule
