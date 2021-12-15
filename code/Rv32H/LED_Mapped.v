module LED_Mapped(
	input wire i_enable,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output wire [9:0] o_leds
);

	reg [9:0] leds;
	
	assign o_leds = leds;

	always @ (posedge i_enable) begin
        if (!i_rw) begin
				leds <= { 2'b0, i_wdata[7:0] };
        end
	end

endmodule