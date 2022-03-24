
`timescale 1ns/1ns

// 800*480
module VIDEO_LCD_AT070NTN92 #(
	parameter SYSTEM_FREQUENCY = 100_000_000,
	parameter VGA_FREQUENCY = 25_175_000,
	parameter HLINE = 1056,
	parameter HBACK = 46,
	parameter HFRONT = 210,
	parameter HPULSE = 1,
	parameter VLINE = 525,
	parameter VBACK = 23,
	parameter VFRONT = 22,
	parameter VPULSE = 1
)(
	input i_clock,

	// Video output.
	output logic o_vga_clock,
	output logic o_data_enable,
	output logic [10:0] o_pos_x,
	output logic [10:0] o_pos_y
);
	// Configuration
	parameter cfg_prescale = SYSTEM_FREQUENCY / VGA_FREQUENCY;
	parameter cfg_hline = HLINE;
	parameter cfg_hback = HBACK;
	parameter cfg_hfront = HFRONT;
	parameter cfg_hpulse = HPULSE;
	parameter cfg_vline = VLINE;
	parameter cfg_vback = VBACK;
	parameter cfg_vfront = VFRONT;
	parameter cfg_vpulse = VPULSE;

	logic [7:0] prescale = 0;
	logic [10:0] vga_h = 0;
	logic [10:0] vga_v = 0;

	initial begin
		o_data_enable = 0;
		o_pos_x = 0;
		o_pos_y = 0;
		o_vga_clock = 0;
	end

	always_ff @(posedge i_clock) begin
		prescale <= prescale + 1;
		if (prescale >= cfg_prescale - 1) begin
			if (vga_h == cfg_hline - 1) begin
				vga_h <= 0;
				if (vga_v == cfg_vline - 1)
					vga_v <= 0;
				else
					vga_v <= vga_v + 1;
			end
			else
				vga_h <= vga_h + 1;
			prescale <= 0;
		end
		o_vga_clock <= prescale[0];
	end
	
	// assign o_hsync = (vga_h < cfg_hpulse) ? 0 : 1;
	// assign o_vsync = (vga_v < cfg_vpulse) ? 0 : 1;
	
	assign o_data_enable = (vga_h >= cfg_hback && vga_h < cfg_hline - cfg_hfront && vga_v >= cfg_vback && vga_v < cfg_vline - cfg_vfront);

	logic [10:0] pl_pos_x;
	logic [10:0] pl_pos_y;

	always @(posedge i_clock) begin

		pl_pos_x <= (vga_h - cfg_hback);
		pl_pos_y <= (vga_v - cfg_vback);

		o_pos_x <= pl_pos_x;
		o_pos_y <= pl_pos_y;

	end

endmodule
