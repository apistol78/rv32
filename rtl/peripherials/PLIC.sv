
`timescale 1ns/1ns

module PLIC(
	input i_reset,
	input i_clock,

	// Input
	input i_interrupt_0,
	input i_interrupt_1,
	input i_interrupt_2,
	input i_interrupt_3,

	// Output
	output o_interrupt,

	// CPU interface
	input i_request,
	input i_rw,
	input [23:0] i_address,
	input [31:0] i_wdata,
	output logic [31:0] o_rdata,
	output logic o_ready
);

	logic [3:0] pending = 0;

	assign o_interrupt = |pending;

	initial begin
		 o_rdata = 0;
		 o_ready = 0;
	end

	always_ff @(posedge i_clock) begin
		if (i_interrupt_0) begin
			pending[0] <= 1;
		end
		if (i_interrupt_1) begin
			pending[1] <= 1;
		end
		if (i_interrupt_2) begin
			pending[2] <= 1;
		end
		if (i_interrupt_3) begin
			pending[3] <= 1;
		end

		o_rdata <= 0;
		o_ready <= 0;

		if (i_request && !i_rw) begin
			if (i_address == 24'h200004) begin	// claim context 0
				if (pending[0]) begin
					o_rdata <= 1;
					pending[0] <= 0;
				end
				else if (pending[1]) begin
					o_rdata <= 2;
					pending[1] <= 0;
				end
				else if (pending[2]) begin
					o_rdata <= 3;
					pending[2] <= 0;
				end
				else if (pending[3]) begin
					o_rdata <= 4;
					pending[3] <= 0;
				end
			end
		end

		if (i_request) begin
			o_ready <= 1;
		end
	end

endmodule
