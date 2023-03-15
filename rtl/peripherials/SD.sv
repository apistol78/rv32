/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

`timescale 1ns/1ns

module SD (
	input i_reset,
	input i_clock,
	
	input i_request,
	input i_rw,
	input [1:0] i_address,
	input [31:0] i_wdata,
	output bit [31:0] o_rdata,
	output bit o_ready,

    output SD_CLK,
`ifndef __VERILATOR__
      inout SD_CMD,
      inout [3:0] SD_DAT
`else
      input SD_CMD_in,
      input [3:0] SD_DAT_in,
      output SD_CMD_out,
      output [3:0] SD_DAT_out
`endif
);
	localparam DIR_IN = 1'b0;
	localparam DIR_OUT = 1'b1;

	typedef enum bit [7:0]
	{
		IDLE,
		WRITE_CMD_BYTE,
		READ_DAT_BYTE_1,
		READ_DAT_BYTE_2,
		READ_DAT_BYTE_3,
		READ_DAT_BYTE_4,
		READ_DAT_DWORD_1,
		READ_DAT_DWORD_2,
		READ_DAT_DWORD_3,
		READ_DAT_DWORD_4,
		READ_DAT_DWORD_5,
		READ_DAT_DWORD_6,
		READ_DAT_DWORD_7,
		READ_DAT_DWORD_8,
		READ_DAT_DWORD_9,
		READ_DAT_DWORD_10,
		READ_DAT_DWORD_11,
		READ_DAT_DWORD_12,
		READ_DAT_DWORD_13,
		READ_DAT_DWORD_14,
		READ_DAT_DWORD_15,
		READ_DAT_DWORD_16,
		WAIT_EOT
	} state_t;
	
	bit clk = 1'b0;
	bit cdir = DIR_IN;
	bit ddir = DIR_IN;
	bit cmd;
	bit [3:0] dat;

	wire [7:0] mask = i_wdata[15:8];
	wire [7:0] write = i_wdata[7:0];

	initial o_rdata = 32'h0000_0000;

	assign SD_CLK = clk;

`ifndef __VERILATOR__
    assign SD_CMD = (cdir == DIR_OUT) ? cmd : 1'bZ;
	assign SD_DAT = (ddir == DIR_OUT) ? dat : 4'bZ;
	
	wire cmd_in = SD_CMD;
	wire [3:0] dat_in = SD_DAT;
`else
	assign SD_CMD_out = cmd;
	assign SD_DAT_out = dat;

	wire cmd_in = SD_CMD_in;
	wire [3:0] dat_in = SD_DAT_in;
`endif


	state_t state = IDLE;
	bit [7:0] wcmddata;
	bit [7:0] wcmdcount;


	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			clk <= 1'b0;
			cdir <= DIR_IN;
			ddir <= DIR_IN;
			o_rdata <= 32'h0000_0000;
			state <= IDLE;
		end
		else begin

			o_ready <= 1'b0;

			case (state)
				IDLE: begin
					if (i_request) begin
						if (!i_rw) begin
							if (i_address == 0) begin
								o_rdata <= {
									24'b0,
									(ddir == DIR_IN) ? dat_in : dat,
									(cdir == DIR_IN) ? cmd_in : cmd,
									ddir,
									cdir,
									clk
								};
								o_ready <= 1'b1;
							end
							else if (i_address == 2) begin
								// Read data byte.
								o_rdata <= 0;
								ddir <= DIR_IN;
								clk <= 1'b0;
								state <= READ_DAT_BYTE_1;
							end
							else if (i_address == 3) begin
								// Read data dword.
								o_rdata <= 0;
								ddir <= DIR_IN;
								clk <= 1'b0;
								state <= READ_DAT_DWORD_1;
							end
						end
						else begin
							if (i_address == 0) begin
								clk  <= ( clk & ~mask[0]  ) | write[0];
								cdir <= (cdir & ~mask[1]  ) | write[1];
								ddir <= (ddir & ~mask[2]  ) | write[2];
								cmd  <= ( cmd & ~mask[3]  ) | write[3];
								dat  <= ( dat & ~mask[7:4]) | write[7:4];
								o_ready <= 1'b1;
							end
							else if (i_address == 1) begin
								// Write command byte.
								cdir <= DIR_OUT;
								wcmddata <= i_wdata[7:0];
								wcmdcount <= 0;
								state <= WRITE_CMD_BYTE;
							end
						end
					end
				end

				WRITE_CMD_BYTE: begin
					if (wcmdcount[0] == 1'b0) begin
						clk <= 1'b0;
						cmd <= wcmddata[7];
						wcmddata <= wcmddata << 1;
					end
					else begin
						clk <= 1'b1;
					end
					wcmdcount <= wcmdcount + 1;
					if (wcmdcount >= 15) begin
						o_ready <= 1'b1;
						state <= WAIT_EOT;
					end
				end

				// Read byte

				READ_DAT_BYTE_1: begin
					clk <= 1'b1;
					state <= READ_DAT_BYTE_2;
				end

				READ_DAT_BYTE_2: begin
					clk <= 1'b0;
					o_rdata[7:4] <= dat_in;
					state <= READ_DAT_BYTE_3;
				end

				READ_DAT_BYTE_3: begin
					clk <= 1'b1;
					state <= READ_DAT_BYTE_4;
				end

				READ_DAT_BYTE_4: begin
					o_rdata[3:0] <= dat_in;
					o_ready <= 1'b1;
					state <= WAIT_EOT;
				end

				// Read dword

				READ_DAT_DWORD_1: begin
					clk <= 1'b1;
					state <= READ_DAT_DWORD_2;
				end

				READ_DAT_DWORD_2: begin		// b0
					clk <= 1'b0;
					o_rdata[7:4] <= dat_in;
					state <= READ_DAT_DWORD_3;
				end

				READ_DAT_DWORD_3: begin
					clk <= 1'b1;
					state <= READ_DAT_DWORD_4;
				end

				READ_DAT_DWORD_4: begin
					clk <= 1'b0;
					o_rdata[3:0] <= dat_in;
					state <= READ_DAT_DWORD_5;
				end

				READ_DAT_DWORD_5: begin
					clk <= 1'b1;
					state <= READ_DAT_DWORD_6;
				end

				READ_DAT_DWORD_6: begin		// b1
					clk <= 1'b0;
					o_rdata[15:12] <= dat_in;
					state <= READ_DAT_DWORD_7;
				end

				READ_DAT_DWORD_7: begin
					clk <= 1'b1;
					state <= READ_DAT_DWORD_8;
				end

				READ_DAT_DWORD_8: begin
					clk <= 1'b0;
					o_rdata[11:8] <= dat_in;
					state <= READ_DAT_DWORD_9;
				end

				READ_DAT_DWORD_9: begin
					clk <= 1'b1;
					state <= READ_DAT_DWORD_10;
				end

				READ_DAT_DWORD_10: begin	// b2
					clk <= 1'b0;
					o_rdata[23:20] <= dat_in;
					state <= READ_DAT_DWORD_11;
				end

				READ_DAT_DWORD_11: begin
					clk <= 1'b1;
					state <= READ_DAT_DWORD_12;
				end

				READ_DAT_DWORD_12: begin
					clk <= 1'b0;
					o_rdata[19:16] <= dat_in;
					state <= READ_DAT_DWORD_13;
				end

				READ_DAT_DWORD_13: begin
					clk <= 1'b1;
					state <= READ_DAT_DWORD_14;
				end

				READ_DAT_DWORD_14: begin	// b3
					clk <= 1'b0;
					o_rdata[31:28] <= dat_in;
					state <= READ_DAT_DWORD_15;
				end

				READ_DAT_DWORD_15: begin
					clk <= 1'b1;
					state <= READ_DAT_DWORD_16;
				end

				READ_DAT_DWORD_16: begin
					o_rdata[27:24] <= dat_in;
					o_ready <= 1'b1;
					state <= WAIT_EOT;
				end

				// End of transmission

				WAIT_EOT: begin
					o_ready <= i_request;
					if (!i_request) begin
						state <= IDLE;
					end
				end

				default: begin
					state <= IDLE;
				end
			endcase
		end
	end
	
endmodule
