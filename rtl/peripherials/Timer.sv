
`timescale 1ns/1ns

module Timer#(
    parameter FREQUENCY
)(
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

	localparam PRESCALE = FREQUENCY / 1000;
	localparam PRESCALE_WIDTH = $clog2(PRESCALE);

	bit [PRESCALE_WIDTH - 1:0] prescale = 0;
	bit [63:0] cycles [3:0];
	bit [3:0] enabled = 4'b0000;
	bit [63:0] compare = { 64{1'b1} };
	bit [31:0] ms = 0;
	bit request = 0;
	bit raised = 0;

	initial begin
		o_ready = 1'b0;
		o_interrupt = 1'b0;

		cycles[0] = 0;
		cycles[1] = 0;
		cycles[2] = 0;
		cycles[3] = 0;
	end

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			cycles[0] <= 0;
			cycles[1] <= 0;
			cycles[2] <= 0;
			cycles[3] <= 0;
		end
		else begin
			if (enabled[0])
				cycles[0] <= cycles[0] + 64'h1;
			if (enabled[1])
				cycles[1] <= cycles[1] + 64'd1;
			if (enabled[2])
				cycles[2] <= cycles[2] + 64'd1;
			if (enabled[3])
				cycles[3] <= cycles[3] + 64'd1;

			if (enabled[0])
				o_interrupt <= raised || ((cycles[0] == compare) ? 1'b1 : 1'b0);
		end
	end

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			ms <= 0;
			prescale <= 0;
		end
		else begin
			prescale <= prescale + 1;
			if (prescale >= PRESCALE) begin
				ms <= ms + 1;
				prescale <= 0;
			end
		end
	end

	always_ff @(posedge i_clock) begin
		request <= i_request;
		raised <= 1'b0;

		if (i_request && !request) begin
			if (!i_rw) begin
				case (i_address)
					4'h0: o_rdata <= ms;
					4'h1: o_rdata <= cycles[0][31:0];
					4'h2: o_rdata <= cycles[0][63:32];
					4'h3: o_rdata <= cycles[1][31:0];
					4'h4: o_rdata <= cycles[1][63:32];
					4'h5: o_rdata <= cycles[2][31:0];
					4'h6: o_rdata <= cycles[2][63:32];
					4'h7: o_rdata <= cycles[3][31:0];
					4'h8: o_rdata <= cycles[3][63:32];
					4'h9: o_rdata <= compare[31:0];
					4'ha: o_rdata <= compare[63:32];
					4'he: o_rdata <= { 28'b0, enabled };
					default: o_rdata <= 0;
				endcase
			end
			else begin
				case (i_address)
					4'h9: compare[31:0] <= i_wdata;
					4'ha: compare[63:32] <= i_wdata;
					4'he: enabled <= i_wdata[3:0];
					4'hf: raised <= 1'b1;
					default:;
				endcase
			end
			o_ready <= 1;
		end
		else if (!i_request)
			o_ready <= 0;
	end
	
endmodule
