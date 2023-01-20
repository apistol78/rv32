
`timescale 1ns/1ns

module VIDEO_VGA #(
	parameter HLINE,
	parameter HBACK,
	parameter HFRONT,
	parameter HPULSE,
	parameter VLINE,
	parameter VBACK,
	parameter VFRONT,
	parameter VPULSE,
	parameter VSPOL,			// Polarity (0 = negative, 1 = positive)
	parameter HSPOL
)(
	input i_clock,				//!< VGA timing clock.
	input i_clock_out,			//!< Signal output clock domain.

	output bit o_clock,
	
	output bit o_hsync,			//!< 1 when sync.
	output bit o_vsync,			//!< 1 when sync.

	output bit o_hblank,		//!< 1 when output enable.
	output bit o_vblank,		//!< 1 when output enable.

	output bit o_data_enable,

	output bit [10:0] o_pos_x,
	output bit [10:0] o_pos_y
);
	bit [10:0] vga_h = 0;
	bit [10:0] vga_v = 0;

	initial begin
		o_hsync = 0;
		o_vsync = 0;
		o_hblank = 0;
		o_vblank = 0;
		o_pos_x = 0;
		o_pos_y = 0;
	end

	always_ff @(posedge i_clock) begin
		if (vga_h == HLINE - 1) begin
			vga_h <= 0;
			if (vga_v == VLINE - 1)
				vga_v <= 0;
			else
				vga_v <= vga_v + 1;
		end
		else
			vga_h <= vga_h + 1;
	end

	// Cross clock domains of data enable to scan out positions.
	bit pl_clock_a = 0;
	bit pl_clock_b = 0;
	always_ff @(posedge i_clock_out) begin
		pl_clock_a <= i_clock;
		pl_clock_b <= pl_clock_a;
		o_clock <= pl_clock_b;
	end
	
	bit pl_hsync_a = 0;
	bit pl_hsync_b = 0;
	bit pl_vsync_a = 0;
	bit pl_vsync_b = 0;

	always_ff @(posedge i_clock_out) begin
		pl_hsync_a <= (vga_h >= HLINE - HPULSE) ? HSPOL : (1 - HSPOL);
		pl_vsync_a <= (vga_v >= VLINE - VPULSE) ? VSPOL : (1 - VSPOL);
		pl_hsync_b <= pl_hsync_a;
		pl_vsync_b <= pl_vsync_a;
		o_hsync <= pl_hsync_b;
		o_vsync <= pl_vsync_b;
	end

	bit pl_hblank_a = 0;
	bit pl_hblank_b = 0;
	bit pl_vblank_a = 0;
	bit pl_vblank_b = 0;

	always_ff @(posedge i_clock_out) begin
		pl_hblank_a <= (vga_h >= HBACK && vga_h < (HLINE - HPULSE - HFRONT - 1)) ? 1 : 0;
		pl_vblank_a <= (vga_v >= VBACK - 1 && vga_v < (VLINE - VPULSE - VFRONT - 1)) ? 1 : 0;
		pl_hblank_b <= pl_hblank_a;
		pl_vblank_b <= pl_vblank_a;
		o_hblank <= pl_hblank_b;
		o_vblank <= pl_vblank_b;
	end	

	bit [10:0] pl_pos_x_a = 0;
	bit [10:0] pl_pos_x_b = 0;
	bit [10:0] pl_pos_y_a = 0;
	bit [10:0] pl_pos_y_b = 0;

	always_ff @(posedge i_clock_out) begin
		pl_pos_x_a <= vga_h - HBACK;
		pl_pos_y_a <= vga_v - VBACK;
		pl_pos_x_b <= pl_pos_x_a;
		pl_pos_y_b <= pl_pos_y_a;
		o_pos_x <= pl_pos_x_b;
		o_pos_y <= pl_pos_y_b;
	end

	bit pl_data_enable_a = 0;
	bit pl_data_enable_b = 0;
	always_ff @(posedge i_clock_out) begin
		pl_data_enable_a <=
			((vga_h >= HBACK && vga_h < (HLINE - HPULSE - HFRONT - 1)) ? 1 : 0) &
			((vga_v >= VBACK && vga_v < (VLINE - VPULSE - VFRONT - 1)) ? 1 : 0);
		pl_data_enable_b <= pl_data_enable_a;
		o_data_enable <= pl_data_enable_b;
	end

endmodule
