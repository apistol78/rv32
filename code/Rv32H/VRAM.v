
module VRAM(
	input wire i_clock,

	// Video
	input wire [15:0] i_video_address,
	input wire i_video_enable,
	output wire [23:0] o_video_rdata,

	// CPU
	input wire i_request,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg o_ready
);

	// rrrgggbb
	reg [23:0] data [0:320 * 240];
	
	initial o_ready = 0;
	
	assign o_video_rdata = i_video_enable ? data[i_video_address] : 0;
	
	always @ (posedge i_clock) begin
		if (i_request && i_rw) begin
			if (i_address != 32'h4ffffff0)
				data[i_address >> 2] <= i_wdata[23:0];
			else
				/* present */;
		end
	end

	always @(posedge i_clock)
		o_ready <= i_request;

endmodule
