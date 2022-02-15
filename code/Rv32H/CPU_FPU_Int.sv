`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_FPU_Int(
	input i_reset,
	input i_clock,

	input i_request,
	input [31:0] i_op1,

	output o_ready,
	output [31:0] o_result
);

	typedef enum bit [2:0]
	{
		IDLE,
		SPECIAL_CASES,
		CONVERT,
		PUT_Z
	} state_t;

	reg s_output_ready = 0;
	reg [31:0] s_output_z = 0;
	state_t state = IDLE;

	reg [31:0] a_m, z;
	reg [8:0] a_e;
	reg a_s;

	assign o_ready = s_output_ready;
	assign o_result = s_output_z;

	always_ff @(posedge i_clock) begin
		case(state)
			IDLE: begin
				s_output_ready <= 0;
				if (i_request) begin
					a_m[31:8] <= { 1'b1, i_op1[22 : 0] };
					a_m[7:0] <= 0;
					a_e <= i_op1[30 : 23] - 127;
					a_s <= i_op1[31];
					state <= SPECIAL_CASES;
				end
			end

			SPECIAL_CASES: begin
				if ($signed(a_e) == -127) begin
					z <= 0;
					state <= PUT_Z;
				end else if ($signed(a_e) > 31) begin
					z <= 32'h80000000;
					state <= PUT_Z;
				end else begin
					state <= CONVERT;
				end
			end

			CONVERT: begin
				if ($signed(a_e) < 31 && a_m) begin
					a_e <= a_e + 1;
					a_m <= a_m >> 1;
				end else begin
					if (a_m[31]) begin
						z <= 32'h80000000;
					end else begin
						z <= a_s ? -a_m : a_m;
					end
					state <= PUT_Z;
				end
			end

			PUT_Z: begin
				s_output_ready <= 1;
				s_output_z <= z;
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
