
`timescale 1ns/1ns

// ILI9341
// 4-wire SPI

module VIDEO_LCD_i80_SPI(
	input i_reset,
	input i_clock,

	input i_request,
	input i_rw,
	input [1:0] i_address,
	output logic [31:0] o_rdata,
	input [31:0] i_wdata,
	output o_ready,

	output o_lcd_rst,	// Reset
	output o_lcd_cs,	// Chip select
	output o_lcd_rs,	// Register select
	output o_lcd_mosi,	// SPI MOSI
	input i_lcd_miso,	// SPI MISO
	output o_lcd_sck	// SPI clock
);

	localparam DLY = 1;

	assign o_lcd_rst = !i_reset;	// reset	(inverted)
	assign o_lcd_cs = cs_n;			// chip select	(inverted)
	assign o_lcd_rs = rs;			// register select, 0 = index or command, 1 = data
	assign o_lcd_mosi = mosi;
	assign o_lcd_sck = sck;
	assign o_ready = i_request && ready;

	logic cs_n = 1;
	logic rs = 0;
	logic mosi = 0;
	logic sck = 0;

	wire miso = i_lcd_miso;

	logic [7:0] rdata = 0;
	logic [7:0] wdata = 0;
	logic [7:0] state = 0;
	logic [15:0] dly = 0;
	logic [3:0] cnt = 0;
	logic ready = 0;

	initial begin
		o_rdata = 0;
	end

	always_ff @(posedge i_clock) begin

		case (state)
			0: begin
				ready <= 0;
				sck <= 0;
				dly <= DLY;
				cnt <= 8 - 1;
				if (i_request) begin

					if (i_address == 2'h2) begin
						cs_n <= i_wdata[0];
						state <= 3;
					end
					else begin
						wdata <= i_rw ? i_wdata[7:0] : 8'h0;
						if (i_address == 2'h0) begin
							rs <= 0;
						end
						else if (i_address == 2'h1) begin
							rs <= 1;
						end
						state <= 1;
					end
					
				end
			end

			// ===========================

			1: begin

				mosi <= wdata[7];
				dly <= dly - 1;
				if (dly == 0) begin

					sck <= 1'b1;

					dly <= DLY;
					state <= 2;
				end
			end

			2: begin
				dly <= dly - 1;
				if (dly == 0) begin

					wdata <= { wdata[6:0], 1'b0 };
					rdata <= { rdata[6:0], miso };
					sck <= 1'b0;

					dly <= DLY;
					cnt <= cnt - 1;

					if (|cnt) begin
						state <= 1;
					end
					else begin
						state <= 3;
					end
				end
			end

			3: begin
				o_rdata <= { 24'b0, rdata };
				ready <= 1'b1;
				if (!i_request) begin
					state <= 0;
				end
			end

			// ===========================

			default: begin
				state <= 0;
			end

		endcase

	end

endmodule
