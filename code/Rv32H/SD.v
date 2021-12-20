
module SD (
	input wire i_enable,
	input wire i_rw,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,

    output reg SD_CLK,
    inout wire SD_CMD,
    inout wire [3:0] SD_DAT
);
    reg sd_cmd = 0;
	reg [3:0] sd_dat = 0;

	wire [7:0] mask = i_wdata[15:8];
	wire [7:0] write = i_wdata[7:0];

    assign SD_CMD = sd_cmd;
	assign SD_DAT = sd_dat;

	always @ (posedge i_enable) begin
		if (!i_rw) begin
			o_rdata <= { 26'b0, SD_DAT, SD_CMD, SD_CLK };
		end
		else begin	// write
            SD_CLK <= (SD_CLK & mask[0]) | write[0];
            sd_cmd <= (sd_cmd & mask[1]) | write[1];
            sd_dat <= (sd_dat & mask[5:2]) | write[5:2];
		end
	end
endmodule
