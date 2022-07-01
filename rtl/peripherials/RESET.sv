`timescale 1ns/1ns

module RESET(
	input i_clock,
	input i_reset_sw,	//< Reset switch, active high.
	output o_reset_0,	//< "Soft" reset signal, active high.
	output o_reset_1,	//< "Global" reset signal, active high.
	output o_reset_2	//< "Start" reset signal, active high.
);

	bit [31:0] counter = 0;
	bit [4:0] sample = 0;

	always_ff @(posedge i_clock) begin
		counter <= (counter == 32'd4_000_001 ) ? 32'd0 : counter + 1'b1;
	end

	always_ff @(posedge i_clock) begin
		if (counter == 32'd4_000_000)
			sample[4:0] <= { sample[3:0], i_reset_sw };
	end

	assign o_reset_0 = (sample[1:0] == 2'b10) ? 1'b1 : 1'b0;
	assign o_reset_1 = (sample[3:2] == 2'b10) ? 1'b1 : 1'b0;
	assign o_reset_2 = (sample[4:3] == 2'b01) ? 1'b1 : 1'b0;

endmodule
