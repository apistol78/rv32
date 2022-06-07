
`timescale 1ns/1ns

module SystemRegisters #(
	parameter FREQUENCY,
	parameter DEVICEID
)(
	input i_reset,
	input i_clock,

	// CPU access
	input i_request,
	input i_rw,
	input [1:0] i_address,
	input [31:0] i_wdata,
	output bit [31:0] o_rdata,
	output bit o_ready,

	// Signals
	input i_boot_mode_switch,		// 0 - normal (load boot.elf from SD), 1 - remote load through UART.
	output [7:0] o_leds,
	output o_sil9024_reset
);

	bit boot_mode = 0;
	bit sil9024_reset = 0;
	bit [7:0] leds = 8'h0;

	assign o_sil9024_reset = sil9024_reset;
	assign o_leds = leds;

    initial o_ready = 0;

	always_ff @(posedge i_clock) begin
		o_ready <= 0;
		if (i_reset) begin
			boot_mode <= i_boot_mode_switch;
			leds <= 8'h0;
		end
		else if (i_request) begin
			if (!i_rw) begin
				if (i_address == 2'b00)
					o_rdata <= { 30'b0, sil9024_reset, boot_mode };
				else if (i_address == 2'b01)
					o_rdata <= { 24'b0, leds };
				else if (i_address == 2'b10)
					o_rdata <= FREQUENCY;
				else if (i_address == 2'b11)
					o_rdata <= DEVICEID;
			end
			else begin
				if (i_address == 2'b00) begin
					boot_mode <= i_wdata[0];
					sil9024_reset <= i_wdata[1];
				end
				else if (i_address == 2'b01) begin
					leds <= i_wdata[7:0];
				end
			end
			o_ready <= 1;
		end
	end

endmodule
