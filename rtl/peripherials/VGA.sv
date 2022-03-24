
`timescale 1ns/1ns

// 640 * 400
module VGA #(
	parameter SYSTEM_FREQUENCY = 100_000_000,
	parameter VGA_FREQUENCY = 25_175_000,
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

	output wire o_hsync,
	output wire o_vsync,
	output wire o_data_enable,
	output wire [9:0] o_pos_x,
	output wire [9:0] o_pos_y,
	output reg o_vga_clock
);
	parameter PRESCALE = SYSTEM_FREQUENCY / VGA_FREQUENCY;
	
	logic [3:0] prescale = 0;
	logic [9:0] vga_h = 0;
	logic [9:0] vga_v = 0;

	always_ff @(posedge i_clock) begin
		prescale <= prescale + 1;
		if (prescale >= PRESCALE - 1) begin
			if (vga_h == HLINE - 1) begin
				vga_h <= 0;
				if (vga_v == VLINE - 1)
					vga_v <= 0;
				else
					vga_v <= vga_v + 1;
			end
			else
				vga_h <= vga_h + 1;

			prescale <= 0;
		end
		o_vga_clock <= prescale[1];	// \fixme Assume 100 MHz clock.
	end

	assign o_hsync = (vga_h < HPULSE) ? 0 : 1;
	assign o_vsync = (vga_v < VPULSE) ? 0 : 1;
	
	assign o_data_enable = (vga_h >= HBACK && vga_h < HLINE - HFRONT && vga_v >= VBACK && vga_v < VLINE - VFRONT);
	assign o_pos_x = vga_h - HBACK;
	assign o_pos_y = vga_v - VBACK;

endmodule
