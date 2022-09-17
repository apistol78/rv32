
`timescale 1ns/1ns

module SystemRegisters #(
	parameter FREQUENCY,
	parameter DEVICEID,
	parameter RAM_SIZE
)(
	input i_reset,
	input i_clock,

	// CPU access
	input i_request,
	input i_rw,
	input [2:0] i_address,
	input [31:0] i_wdata,
	output bit [31:0] o_rdata,
	output bit o_ready,

	// Signals
	output bit o_reset_switch,
	output [7:0] o_leds,
	output o_sil9024_reset
);

	bit sil9024_reset = 1'b0;
	bit [7:0] leds = 8'h0;
	bit boot_mode = 1'b0;		//!< 0 - normal (load boot.elf from SD), 1 - remote load through UART.

	assign o_sil9024_reset = sil9024_reset;
	assign o_leds = leds;

    initial begin
		o_ready = 1'b0;
		o_reset_switch = 1'b0;
	end

	always_ff @(posedge i_clock) begin
		o_ready <= 0;
		if (i_reset) begin
			leds <= 8'h0;
			o_reset_switch <= 1'b0;
		end
		else if (i_request) begin
			if (!i_rw) begin
				case (i_address)
				3'd0:
					o_rdata <= { 30'b0, sil9024_reset, boot_mode };
				3'd1:
					o_rdata <= { 24'b0, leds };
				3'd2:
					o_rdata <= FREQUENCY;
				3'd3:
					o_rdata <= DEVICEID;
				3'd4:
					o_rdata <= RAM_SIZE;
				default:
					o_rdata <= 0;
				endcase
			end
			else begin
				case (i_address)
				3'd0: begin
					boot_mode <= i_wdata[0];
					sil9024_reset <= i_wdata[1];
				end
				3'd1:
					leds <= i_wdata[7:0];
				3'd5:
					o_reset_switch <= 1'b1;
				default:
					;
				endcase
			end
			o_ready <= 1;
		end
	end

endmodule
