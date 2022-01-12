
`timescale 1ns/1ns

module VRAM_page(
	input wire i_clock,

	input wire [15:0] i_address,

	input wire i_request,
    input wire i_rw,

	// Video
	output reg [23:0] o_rdata,

	// CPU
	input wire [23:0] i_wdata
);

	// rrrgggbb
	reg [23:0] data [0:320 * 200];
	
	always @ (posedge i_clock) begin
		if (i_request && i_rw) begin
			data[i_address] <= i_wdata;
		end
        else if (i_request && !i_rw) begin
            o_rdata <= data[i_address];
        end
	end

endmodule
