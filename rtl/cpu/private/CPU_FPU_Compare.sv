`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_FPU_Compare(
	input i_reset,
	input i_clock,

	input i_request,	
	input [31:0] i_op1,
	input [31:0] i_op2,

	output o_ready,
	output o_less,
	output o_equal
);
	typedef enum bit [0:0]
	{
		IDLE,
		PUT_Z
	} state_t;

	logic s_output_ready = 0;
	state_t state = IDLE;

	logic [22:0] a_m;
	logic [22:0] b_m;

	logic [9:0] a_e;
	logic [9:0] b_e;

	logic a_s;
	logic b_s;

	logic less = 0;
	logic equal = 0;

	assign o_ready = s_output_ready;
	assign o_less = less;
	assign o_equal = equal;

	always_ff @(posedge i_clock) begin
		case(state)
			IDLE: begin
				s_output_ready <= 0;
				if (i_request) begin
					a_m <= { i_op1[22:0] };
					b_m <= { i_op2[22:0] };

					a_e <= { i_op1[30:23] };
					b_e <= { i_op2[30:23] };

					a_s <= i_op1[31];
					b_s <= i_op2[31];

					state <= PUT_Z;
				end
			end

			PUT_Z: begin
				less <= 0;
				equal <= 0;

				if (a_s != b_s) begin
					if (!a_s) begin
						less <= 0;
					end
					else begin
						less <= 1;
					end
				end
				else if (a_e != b_e) begin
					if (a_e > b_e) begin
						less <= a_s;
					end
					else begin
						less <= !a_s;
					end
				end
				else if (a_m != b_m) begin
					if (a_m > b_m) begin
						less <= a_s;
					end
					else begin
						less <= !a_s;
					end
				end
				else begin
					equal <= 1;
				end

				s_output_ready <= 1;
				if (!i_request) begin
					s_output_ready <= 0;
					state <= IDLE;
				end
			end

			default: begin
				state <= IDLE;
			end
		endcase

		if (i_reset) begin
			s_output_ready <= 0;
			state <= IDLE;
		end		
	end

endmodule
