
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
	always_ff @(posedge i_clock_out) begin
		pl_clock_a <= i_clock;
		o_clock <= pl_clock_a;
	end
	
	bit pl_hsync_a = 0;
	bit pl_vsync_a = 0;

	always_ff @(posedge i_clock_out) begin
		pl_hsync_a <= (vga_h >= HLINE - HPULSE - 1) ? HSPOL : (1 - HSPOL);
		pl_vsync_a <= (vga_v >= VLINE - VPULSE - 1) ? VSPOL : (1 - VSPOL);
		o_hsync <= pl_hsync_a;
		o_vsync <= pl_vsync_a;
	end

	bit pl_hblank_a = 0;
	bit pl_vblank_a = 0;

	always_ff @(posedge i_clock_out) begin
		pl_hblank_a <= (vga_h > HBACK + 1 && vga_h < (HLINE - HPULSE - HFRONT - 1)) ? 1 : 0;
		pl_vblank_a <= (vga_v > VBACK + 1 && vga_v < (VLINE - VPULSE - VFRONT - 1)) ? 1 : 0;
		o_hblank <= pl_hblank_a;
		o_vblank <= pl_vblank_a;
	end	

	bit [10:0] pl_pos_x_a = 0;
	bit [10:0] pl_pos_y_a = 0;

	always_ff @(posedge i_clock_out) begin
		pl_pos_x_a <= vga_h - HBACK;
		pl_pos_y_a <= vga_v - VBACK;
		o_pos_x <= pl_pos_x_a;
		o_pos_y <= pl_pos_y_a;
	end

endmodule
