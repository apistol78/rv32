
module VIDEO_ILI9331(
	input i_reset,
	input i_clock,

	output o_lcd_rst,
	output o_lcd_cs,
	output o_lcd_rd,
	output o_lcd_rw,
	output o_lcd_rs,
	inout [15:0] io_lcd_db
);

	assign o_lcd_rst = !i_reset;	// reset	(inverted)
	assign o_lcd_cs = !cs;	// chip select	(inverted)
	assign o_lcd_rd = !rd;	// read strobe	(inverted)
	assign o_lcd_rw = !rw;	// write strobe	(inverted)
	assign o_lcd_rs = 0;	// register select, 0 = select index or status, 1 = select control

	logic cs = 1;
	logic rd = 0;
	logic rw = 0;

	logic [5:0] r = 5'b11111;
	logic [5:0] g = 5'b00000;
	logic [5:0] b = 5'b00000;

	logic [3:0] state = 0;
	logic [3:0] prescaler = 0;

	assign io_lcd_db = { r, g, b[5:2] };

	always_ff @(posedge i_clock) begin
		if (prescaler == 0) begin
			case (state)

				0: begin
					rw <= 1;
					state <= 1;
				end

				1: begin
					rw <= 0;
					state <= 0;
				end

			endcase
			prescaler <= 40;
		end
		else begin
			prescaler <= prescaler - 1;
		end
	end

endmodule
