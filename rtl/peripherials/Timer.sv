
`timescale 1ns/1ns

module Timer (
	input i_reset,
	input i_clock,
	input i_request,
	input i_rw,
	input [3:0] i_address,
	input [31:0] i_wdata,
	output bit [31:0] o_rdata,
	output bit o_ready,

	output bit o_interrupt
);

	bit [63:0] cycles = 64'd0;
	bit [63:0] compare = { 64{1'b1} };
	bit [31:0] countdown = 32'd0;
	bit request = 1'b0;

	initial begin
		o_ready = 1'b0;
		o_interrupt = 1'b0;
	end

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			cycles <= 64'd0;
		end
		else begin
			cycles <= cycles + 64'd1;

			//o_interrupt <= ((cycles == compare) ? 1'b1 : 1'b0);

			o_interrupt <= 1'b0;
			if (countdown > 0) begin
				countdown <= countdown - 32'd1;
				if (countdown == 32'd1) begin
					o_interrupt <= 1'b1;
				end
			end

		end
	end

	always_ff @(posedge i_clock) begin
		request <= i_request;
		if (i_request && !request) begin
			if (!i_rw) begin
				case (i_address)
					4'h0: o_rdata <= cycles[31:0];
					4'h1: o_rdata <= cycles[63:32];
					4'h2: o_rdata <= compare[31:0];
					4'h3: o_rdata <= compare[63:32];
					4'h4: o_rdata <= countdown;
					default: o_rdata <= 0;
				endcase
			end
			else begin
				case (i_address)
					4'h2: compare[31:0] <= i_wdata;
					4'h3: compare[63:32] <= i_wdata;
					4'h4: countdown <= i_wdata;
					default:;
				endcase
			end
			o_ready <= 1'b1;
		end
		else if (!i_request)
			o_ready <= 1'b0;
	end
	
endmodule
