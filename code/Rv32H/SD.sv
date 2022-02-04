
`timescale 1ns/1ns

module SD (
	input wire i_reset,
	input wire i_clock,
	
	input wire i_request,
	input wire i_rw,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
	output reg o_ready,

    output wire SD_CLK,
`ifndef __VERILATOR__
      inout wire SD_CMD,
      inout wire [3:0]  SD_DAT
`else
      input wire SD_CMD_in,
      input wire [3:0]  SD_DAT_in,
      output wire SD_CMD_out,
      output wire [3:0] SD_DAT_out
`endif
);
	localparam DIR_IN = 1'b0;
	localparam DIR_OUT = 1'b1;
	
	reg clk = 1'b0;
	reg cdir = DIR_IN;
	reg ddir = DIR_IN;
	reg cmd;
	reg [3:0] dat;

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

	always @(posedge i_clock) begin
		if (i_reset) begin
			clk <= 1'b0;
			cdir <= DIR_IN;
			ddir <= DIR_IN;
			o_rdata <= 32'h0000_0000;
		end
		else begin
			if (i_request) begin
				if (!i_rw) begin
					o_rdata <= {
						24'b0,
						(ddir == DIR_IN) ? dat_in : dat,
						(cdir == DIR_IN) ? cmd_in : cmd,
						ddir,
						cdir,
						clk
					};
				end
				else begin	// write
					clk  <= ( clk & ~mask[0]  ) | write[0];
					cdir <= (cdir & ~mask[1]  ) | write[1];
					ddir <= (ddir & ~mask[2]  ) | write[2];
					cmd  <= ( cmd & ~mask[3]  ) | write[3];
					dat  <= ( dat & ~mask[7:4]) | write[7:4];
				end
			end
		end
	end
	
	always @(posedge i_clock)
		o_ready <= i_request;
		
endmodule
