
`timescale 1ns/1ns

module VIDEO_VGA #(
	parameter HLINE,
	parameter HBACK,
	parameter HFRONT,
	parameter HPULSE,
	parameter VLINE,
	parameter VBACK,
	parameter VFRONT,
	parameter VPULSE
)(
	input i_clock,				//!< VGA timing clock.
	inout i_clock_out,			//!< Signal output clock domain.

	output bit o_hsync,
	output bit o_vsync,
	output bit o_data_enable,
	output bit [10:0] o_pos_x,
	output bit [10:0] o_pos_y
);
	bit [10:0] vga_h = 0;
	bit [10:0] vga_v = 0;

	initial begin
		o_hsync = 0;
		o_vsync = 0;
		o_data_enable = 0;
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
	bit pl_hsync_a = 0;
	bit pl_vsync_a = 0;
	bit pl_hsync_b = 0;
	bit pl_vsync_b = 0;

	always @(posedge i_clock_out) begin
		pl_hsync_a <= (vga_h >= HLINE - HPULSE) ? 1 : 0;
		pl_vsync_a <= (vga_v >= VLINE - VPULSE) ? 1 : 0;
		pl_hsync_b <= pl_hsync_a;
		pl_vsync_b <= pl_vsync_a;
		o_hsync <= pl_hsync_b;
		o_vsync <= pl_vsync_b;
	end

	bit pl_data_enable_a = 0;
	bit pl_data_enable_b = 0;
	
	always @(posedge i_clock_out) begin
		pl_data_enable_a <= (vga_h >= HBACK && vga_h < (HLINE - HPULSE - HFRONT) && vga_v >= VBACK && vga_v < (VLINE - VPULSE - VFRONT));
		pl_data_enable_b <= pl_data_enable_a;
		o_data_enable <= pl_data_enable_b;
	end
	
	bit [10:0] pl_pos_x_a = 0;
	bit [10:0] pl_pos_y_a = 0;
	bit [10:0] pl_pos_x_b = 0;
	bit [10:0] pl_pos_y_b = 0;

	always @(posedge i_clock_out) begin
		pl_pos_x_a <= vga_h;
		pl_pos_y_a <= vga_v;
		pl_pos_x_b <= pl_pos_x_a - HBACK;
		pl_pos_y_b <= pl_pos_y_a - VBACK;
		o_pos_x <= pl_pos_x_b;
		o_pos_y <= pl_pos_y_b;
	end

endmodule
