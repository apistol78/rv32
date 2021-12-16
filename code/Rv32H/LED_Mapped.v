module LED_Mapped(
	input wire i_clock,
	input wire i_enable,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output wire [9:0] o_leds
);

	reg [9:0] leds;
	
	assign o_leds = leds;

	initial begin
		leds <= 10'b1010101010;
	end
	
	always @ (posedge i_clock) begin
		leds[9] <= !leds[9];
	end
	
	always @ (posedge i_enable) begin
 		leds[8] <= !leds[8];
		leds[7:0] <= i_wdata[31:24] | i_wdata[23:16] | i_wdata[15:8] | i_wdata[7:0];
	end

endmodule