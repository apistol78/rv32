
`timescale 1ns/1ns

module VGA #(
    parameter PRESCALE = 50000000 / 25000000
)(
	input i_clock,

	output wire o_hsync,
	output wire o_vsync,
	output wire o_data_enable,
	output wire [8:0] o_pos_x,
	output wire [8:0] o_pos_y,
	output reg o_vga_clock
);
	parameter hori_line  = 800;
	parameter hori_back  = 144;
	parameter hori_front = 16;
	
	parameter vert_line  = 449;
	parameter vert_back  = 36;
	parameter vert_front = 13;
	
	parameter H_sync_cycle = 96;
	parameter V_sync_cycle = 2;
	
	reg [3:0] prescale = 0;
	reg [9:0] vga_h = 0;
	reg [9:0] vga_v = 0;

	always @(posedge i_clock) begin
		prescale <= prescale + 1;
		if (prescale >= PRESCALE - 1) begin
			if (vga_h == hori_line - 1) begin
				vga_h <= 0;
				if (vga_v == vert_line - 1)
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
	
	wire [9:0] px = vga_h - hori_back;
	wire [9:0] py = vga_v - vert_back;

	assign o_hsync = (vga_h < H_sync_cycle) ? 0 : 1;
	assign o_vsync = (vga_v < V_sync_cycle) ? 0 : 1;
	
	assign o_data_enable = (vga_h >= hori_back && vga_h < hori_line - hori_front && vga_v >= vert_back && vga_v < vert_line - vert_front);
	assign o_pos_x = px[9:1];
	assign o_pos_y = py[9:1];

endmodule
