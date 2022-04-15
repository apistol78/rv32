
`timescale 1ns/1ns

// 800*480
module VIDEO_LCD_AT070NTN92 #(
	parameter SYSTEM_FREQUENCY = 100_000_000,
	parameter VGA_FREQUENCY = 25_175_000,
	parameter HLINE = 1056,
	parameter HBACK = 46,
	parameter HFRONT = 210,
	parameter VLINE = 525,
	parameter VBACK = 23,
	parameter VFRONT = 22
)(
	input i_reset,
	input i_clock,
	input i_clock_out,

	// Video output.
	output logic o_vga_clock,
	output logic o_data_enable,
	output logic [10:0] o_pos_x,
	output logic [10:0] o_pos_y
);

	localparam PRESCALE = SYSTEM_FREQUENCY / VGA_FREQUENCY;

	logic [10:0] vga_h = 0;
	logic [10:0] vga_v = 0;
	logic vga_clock = 0;

	initial begin
		o_data_enable = 0;
		o_pos_x = 0;
		o_pos_y = 0;
		o_vga_clock = 0;
	end

	generate if (PRESCALE <= 1) begin
		always_ff @(posedge i_clock) begin
			if (i_reset) begin
				vga_h <= 0;
				vga_v <= 0;
			end
			else begin
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
		end

		always_comb begin
			vga_clock <= i_clock;
		end
	end endgenerate

	generate if (PRESCALE > 1)  begin

		logic [7:0] prescale = 0;

		always_ff @(posedge i_clock) begin
			if (i_reset) begin
				prescale <= 0;
				vga_h <= 0;
				vga_v <= 0;
			end
			else begin
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
				vga_clock <= prescale[0];
			end
		end
	end endgenerate

	// Cross clock domains of data enable to scan out positions.
	logic pl_data_enable_a = 0;
	logic pl_data_enable_b = 0;
	
	logic pl_vga_clock_a = 0;
	logic pl_vga_clock_b = 0;

	always @(posedge i_clock_out) begin
		pl_data_enable_a <= (vga_h >= HBACK && vga_h < HLINE - HFRONT && vga_v >= VBACK && vga_v < VLINE - VFRONT);
		pl_data_enable_b <= pl_data_enable_a;
		o_data_enable <= pl_data_enable_b;

		pl_vga_clock_a <= vga_clock;
		pl_vga_clock_b <= pl_vga_clock_a;
		o_vga_clock <= pl_vga_clock_b;
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
