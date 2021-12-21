module LED(
	input wire i_reset,
	input wire i_clock,
	input wire i_enable,
	input wire [31:0] i_wdata,
	output wire [9:0] o_leds
);

	reg [9:0] leds;
	reg last_enable;
	
	assign o_leds = leds;

	initial begin
		leds <= 10'b1010101010;
		last_enable <= 0;
	end
	
	always @ (posedge i_clock, posedge i_reset) begin
		if (i_reset) begin
			leds <= 10'b1010101010;
			last_enable <= 0;
		end
		else begin
			if (i_enable && !last_enable) begin
				leds[7:0] <= i_wdata[31:24] | i_wdata[23:16] | i_wdata[15:8] | i_wdata[7:0];
				leds[9:8] <= ~leds[9:8];
			end
			last_enable <= i_enable;
		end
	end

endmodule