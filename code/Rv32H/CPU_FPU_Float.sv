`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_FPU_Float(
	input i_reset,
	input i_clock,

	input i_request,
	input [31:0] i_op1,
	input i_signed,

	output o_ready,
	output [31:0] o_result
);

	typedef enum bit [3:0]
	{
		IDLE,
		CONVERT_0,
		CONVERT_1,
		CONVERT_2,
		ROUND,
		PACK,
		PUT_Z
	} state_t;

	reg s_output_ready = 0;
	reg [31:0] s_output_z = 0;
	state_t state = IDLE;

	reg [31:0] a, z, value;
	reg [23:0] z_m;
	reg [7:0] z_r;
	reg [7:0] z_e;
	reg z_s;
	reg guard, round_bit, sticky;

	assign o_ready = s_output_ready;
	assign o_result = s_output_z;

	always_ff @(posedge i_clock) begin
		case(state)
			IDLE: begin
				s_output_ready <= 0;
				a <= i_op1;
				if (i_request)
					state <= CONVERT_0;
			end

			CONVERT_0: begin
				if (a == 0) begin
					z_s <= 0;
					z_m <= 0;
					z_e <= -127;
					state <= PACK;
				end else begin
					value <= (i_signed & a[31]) ? -a : a;
					z_s <= (i_signed & a[31]);
					state <= CONVERT_1;
				end
			end

			CONVERT_1: begin
				z_e <= 31;
				z_m <= value[31:8];
				z_r <= value[7:0];
				state <= CONVERT_2;
			end

			CONVERT_2: begin
				if (!z_m[23]) begin
					z_e <= z_e - 1;
					z_m <= z_m << 1;
					z_m[0] <= z_r[7];
					z_r <= z_r << 1;
				end else begin
					guard <= z_r[7];
					round_bit <= z_r[6];
					sticky <= z_r[5:0] != 0;
					state <= ROUND;
				end
			end

			ROUND: begin
				if (guard && (round_bit || sticky || z_m[0])) begin
					z_m <= z_m + 1;
					if (z_m == 24'hffffff) begin
						z_e <=z_e + 1;
					end
				end
				state <= PACK;
			end

			PACK: begin
				z[22 : 0] <= z_m[22:0];
				z[30 : 23] <= z_e + 127;
				z[31] <= z_s;
				state <= PUT_Z;
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
