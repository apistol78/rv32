
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

	initial begin
		o_data_enable = 0;
		o_pos_x = 0;
		o_pos_y = 0;
		o_vga_clock = 0;
	end

	generate if (PRESCALE <= 1) begin
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

		always_comb begin
			o_vga_clock <= i_clock;
		end
	end endgenerate

	generate if (PRESCALE > 1)  begin

		logic [7:0] prescale = 0;

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
			o_vga_clock <= prescale[0];
		end
	end endgenerate

	assign o_data_enable = (vga_h >= HBACK && vga_h < HLINE - HFRONT && vga_v >= VBACK && vga_v < VLINE - VFRONT);

	logic [10:0] pl_pos_x;
	logic [10:0] pl_pos_y;

	always @(posedge i_clock_out) begin
		pl_pos_x <= (vga_h - HBACK);
		pl_pos_y <= (vga_v - VBACK);
		o_pos_x <= pl_pos_x;
		o_pos_y <= pl_pos_y;
	end

endmodule
