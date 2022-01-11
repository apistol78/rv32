
`timescale 1ns/1ns

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
	input wire [23:0] i_wdata,
	output reg o_ready
);
	initial o_ready = 0;

	// Active page is currently being presented to GPU.
	reg active = 1'b0;

	// Check if input address from CPU is a control register.
	wire is_ctrl = (i_address == 32'h0fff_fff0);

	// ===================
	// PAGE 1
	wire page1_active = active;

	wire [15:0] page1_address = 
		page1_active == 0 ?
		i_video_address :
		i_address >> 2;

	wire page1_request = page1_active == 0 ? i_video_enable : i_request;
	wire page1_rw = page1_active == 0 ? 0 : 1;
	wire [23:0] page1_video_rdata;

	VRAM_page page1(
		.i_clock(i_clock),
		.i_address(page1_address),
		.i_request(page1_request),
		.i_rw(page1_rw),
		.o_rdata(page1_video_rdata),
		.i_wdata(i_wdata)
	);

	// ===================
	// PAGE 2
	wire page2_active = ~active;

	wire [15:0] page2_address = 
		page2_active == 0 ?
		i_video_address :
		i_address >> 2;

	wire page2_request = page2_active == 0 ? i_video_enable : i_request;
	wire page2_rw = page2_active == 0 ? 0 : 1;
	wire [23:0] page2_video_rdata;

	VRAM_page page2(
		.i_clock(i_clock),
		.i_address(page2_address),
		.i_request(page2_request),
		.i_rw(page2_rw),
		.o_rdata(page2_video_rdata),
		.i_wdata(i_wdata)
	);

	// Select output to GPU from active page.
	assign o_video_rdata = (active == 0) ? page1_video_rdata : page2_video_rdata;

	// Swap active page when control register is written to.
	always @(posedge i_clock) begin
		if (i_request && is_ctrl) begin
			active <= i_wdata[0];
		end
	end

	always @(posedge i_clock)
		o_ready <= i_request;

endmodule
