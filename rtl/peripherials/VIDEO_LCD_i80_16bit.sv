
`timescale 1ns/1ns

module VIDEO_LCD_i80_16bit(
	input i_reset,
	input i_clock,

	input i_request,
	input i_rw,
	input [1:0] i_address,
	output bit [31:0] o_rdata,
	input [31:0] i_wdata,
	output bit o_ready,

	output o_lcd_rst,
	output o_lcd_cs,
	output o_lcd_rd,
	output o_lcd_rw,
	output o_lcd_rs,
	inout [15:0] io_lcd_db
);

	localparam DLY = 100;

	assign o_lcd_rst = !i_reset;	// reset	(inverted)
	assign o_lcd_cs = cs_n;			// chip select	(inverted)
	assign o_lcd_rd = rd_n;			// read strobe	(inverted)
	assign o_lcd_rw = rw_n;			// write strobe	(inverted)
	assign o_lcd_rs = rs;			// register select, 0 = select index or status, 1 = select control

	bit cs_n = 1;
	bit rd_n = 0;
	bit rw_n = 0;
	bit rs = 0;
	bit [15:0] db_w = 0;
	wire [15:0] db_r;

	bit [7:0] state = 0;
	bit [15:0] dly = 0;

	initial begin
		o_rdata = 0;
		o_ready = 0;
	end

	assign db_r = !i_rw ? io_lcd_db : 16'h0;
	assign io_lcd_db = i_rw ? db_w : 16'hz;

	always_ff @(posedge i_clock) begin

		case (state)
			0: begin
				o_ready <= 0;
				dly <= DLY;
				if (i_request && !i_rw) begin
					if (i_address == 2'h0) begin	// Read index.
						rs <= 0;
						state <= 1;
					end
					else if (i_address == 2'h1) begin	// Read control/data.
						rs <= 1;
						state <= 1;
					end
				end
				else if (i_request && i_rw) begin
					db_w <= i_wdata[15:0];
					if (i_address == 2'h0) begin	// Write index.
						rs <= 0;
						state <= 8;
					end
					else if (i_address == 2'h1) begin	// Write control/data.
						rs <= 1;
						state <= 8;
					end
				end
			end

			// ===========================
			// Read

			1: begin
				dly <= dly - 1;
				if (dly == 0) begin

					rd_n <= 0;
					rw_n <= 1;
					cs_n <= 0;

					dly <= DLY;
					state <= 2;
				end
			end

			2: begin
				dly <= dly - 1;
				if (dly == 0) begin

					rd_n <= 0;
					rw_n <= 0;
					cs_n <= 1;

					o_rdata <= { 16'h0, db_r };
					o_ready <= 1;					

					dly <= DLY;
					state <= 100;
				end
			end

			// ===========================
			// Write

			8: begin
				dly <= dly - 1;
				if (dly == 0) begin

					rd_n <= 1;
					rw_n <= 0;
					cs_n <= 0;

					dly <= DLY;
					state <= 9;
				end
			end

			9: begin
				dly <= dly - 1;
				if (dly == 0) begin

					rd_n <= 0;
					rw_n <= 0;
					cs_n <= 1;

					o_ready <= 1;

					dly <= DLY;
					state <= 100;
				end
			end

			// ===========================

			100: begin
				if (!i_request) begin
					state <= 0;
				end
			end

			default: begin
				state <= 0;
			end

		endcase

	end

endmodule
