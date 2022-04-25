
`timescale 1ns/1ns

// 640 * 400
module VIDEO_VGA #(
	parameter HLINE = 800,
	parameter HBACK = 144,
	parameter HFRONT = 16,
	parameter HPULSE = 96,
	parameter VLINE = 449,
	parameter VBACK = 36,
	parameter VFRONT = 13,
	parameter VPULSE = 2
)(
	input i_clock,
	inout i_clock_out,

	output logic o_hsync,
	output logic o_vsync,
	output logic o_data_enable,
	output logic [9:0] o_pos_x,
	output logic [9:0] o_pos_y
);
	logic [9:0] vga_h = 0;
	logic [9:0] vga_v = 0;

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
	logic pl_hsync_a = 0;
	logic pl_vsync_a = 0;
	logic pl_hsync_b = 0;
	logic pl_vsync_b = 0;

	always @(posedge i_clock_out) begin
		pl_hsync_a <= (vga_h < HPULSE) ? 0 : 1;
		pl_vsync_a <= (vga_v < VPULSE) ? 0 : 1;
		pl_hsync_b <= pl_hsync_a;
		pl_vsync_b <= pl_vsync_a;
		o_hsync <= pl_hsync_b;
		o_vsync <= pl_vsync_b;
	end

	logic pl_data_enable_a = 0;
	logic pl_data_enable_b = 0;
	
	always @(posedge i_clock_out) begin
		pl_data_enable_a <= (vga_h >= HBACK && vga_h < HLINE - HFRONT && vga_v >= VBACK && vga_v < VLINE - VFRONT);
		pl_data_enable_b <= pl_data_enable_a;
		o_data_enable <= pl_data_enable_b;
	end
	
	logic [10:0] pl_pos_x_a = 0;
	logic [10:0] pl_pos_y_a = 0;
	logic [10:0] pl_pos_x_b = 0;
	logic [10:0] pl_pos_y_b = 0;

	always @(posedge i_clock_out) begin
		pl_pos_x_a <= vga_h;
		pl_pos_y_a <= vga_v;
		pl_pos_x_b <= pl_pos_x_a - HBACK;
		pl_pos_y_b <= pl_pos_y_a - VBACK;
		o_pos_x <= pl_pos_x_b;
		o_pos_y <= pl_pos_y_b;
	end

endmodule
