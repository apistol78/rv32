module LED(
	input wire i_reset,
	input wire i_clock,
	input wire i_request,
	input wire [31:0] i_wdata,
	output reg o_ready,
	output wire [9:0] LEDR
);

	reg [9:0] leds;
	reg last_request;
	
	assign LEDR = leds;

	initial o_ready = 0;

	initial begin
		leds <= 10'b1010101010;
		last_request <= 0;
	end
	
	always @ (posedge i_clock, posedge i_reset) begin
		if (i_reset) begin
			leds <= 10'b1010101010;
			last_request <= 0;
		end
		else begin
			if (i_request && !last_request) begin
				leds[7:0] <= i_wdata[31:24] | i_wdata[23:16] | i_wdata[15:8] | i_wdata[7:0];
				leds[9:8] <= ~leds[9:8];
			end
			last_request <= i_request;
		end
	end

	always @(posedge i_clock)
		o_ready <= i_request;

endmodule