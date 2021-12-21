
module VGA(
	input i_clock,

	output reg o_hsync,
	output reg o_vsync,
	output reg o_data_enable,
	output reg [15:0] o_vga_address,
	output reg o_vga_clock
);

	initial o_vga_clock = 0;
	reg counter = 0;
	reg [9:0] vga_h = 0;
	reg [9:0] vga_v = 0;

	initial begin
		o_hsync = 1;
		o_vsync = 1;
		o_vga_address = 0;
		o_data_enable = 0;
		o_vga_clock = 0;
	end

	always @(posedge i_clock) begin
		if (!counter) begin
			// Display Horizontal
			if (vga_h == 0 && vga_v != 524) begin
				vga_h <= vga_h + 1'b1;
				vga_v <= vga_v + 1'b1;
			end
			else if (vga_h == 0 && vga_v == 524) begin
				vga_h <= vga_h + 1'b1;
				vga_v <= 0; // pixel 525
			end
			else if (vga_h <= 640)
				vga_h <= vga_h + 1'b1;
			// Front Porch
			else if (vga_h <= 656)
				vga_h <= vga_h + 1'b1;
			// Sync Pulse
			else if (vga_h <= 752) begin
				vga_h <= vga_h + 1'b1;
				o_hsync <= 0;
			end
			// Back Porch
			else if(vga_h < 799) begin
				vga_h <= vga_h + 1'b1;
				o_hsync <= 1;
			end
			else
				vga_h <= 0; // pixel 800

			if (vga_v == 491 || vga_v == 492)
				o_vsync <= 0;
			else
				o_vsync <= 1;
		end
		counter <= ~counter;
	end

	always @(posedge i_clock) begin
		if (vga_h >= 0 && vga_h < 640 && vga_v >= 0 && vga_v < 480) begin
			o_vga_address <= (vga_h >> 1) + (vga_v >> 1) * 320;
			o_data_enable <= 1;
		end
		else
			o_data_enable <= 0;
	end

	always @(posedge i_clock) begin
		o_vga_clock <= ~o_vga_clock;
	end

endmodule
