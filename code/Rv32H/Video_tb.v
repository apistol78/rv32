module Video_tb(
	input wire i_enable,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata
);

	always @ (posedge i_enable) begin
        if (!i_rw) begin
		    o_rdata <= 32'h0;
        end
        else begin
            $write("%s", i_wdata[7:0]);
        end
	end

endmodule