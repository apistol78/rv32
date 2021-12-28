module BRAM(
	input wire i_clock,
	input wire i_request,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
	output reg o_ready
);

	reg [31:0] data [0:32'h400 - 1];

	initial o_ready = 0;

	//always @(posedge i_clock) begin
	always @(*) begin
		if (i_request) begin
			if (!i_rw) begin
				o_rdata <= data[i_address >> 2];
			end
			else begin
				data[i_address >> 2] <= i_wdata;
			end
		end
	end

	//always @(posedge i_clock)
	always @(*)
		o_ready <= i_request;

endmodule