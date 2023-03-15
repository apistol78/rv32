/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
`include "../CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_FPU_Mul(
	input i_reset,
	input i_clock,

	input i_request,
	input [31:0] i_op1,
	input [31:0] i_op2,

	output o_ready,
	output [31:0] o_result
);

	typedef enum bit [3:0]
	{
		IDLE,
		SPECIAL_CASES,
		NORMALIZE_A,
		NORMALIZE_B,
		MULTIPLY_0,
		MULTIPLY_1,
		NORMALIZE_1,
		NORMALIZE_2,
		ROUND,
		PACK,
		PUT_Z
	} state_t;

	bit s_output_ready = 0;
	state_t state = IDLE;

	bit [31:0] z;
	bit [23:0] a_m, b_m, z_m;
	bit [9:0] a_e, b_e, z_e;
	bit a_s, b_s, z_s;
	bit guard, round_bit, sticky;
	bit [47:0] product;

	assign o_ready = s_output_ready;
	assign o_result = z;

	always_ff @(posedge i_clock) begin
		case(state)
			IDLE: begin
				s_output_ready <= 0;
				if (i_request) begin
					a_m <= i_op1[22:0];
					a_e <= i_op1[30:23] - 127;
					a_s <= i_op1[31];
					b_e <= i_op2[30:23] - 127;
					b_m <= i_op2[22:0];
					b_s <= i_op2[31];
					state <= SPECIAL_CASES;
				end
			end

			SPECIAL_CASES: begin
				// if a is NaN or b is NaN return NaN 
				if ((a_e == 128 && a_m != 0) || (b_e == 128 && b_m != 0)) begin
					z[31] <= 1;
					z[30:23] <= 255;
					z[22] <= 1;
					z[21:0] <= 0;
					s_output_ready <= 1;
					state <= PUT_Z;
				// if a is inf return inf
				end else if (a_e == 128) begin
					z[31] <= a_s ^ b_s;
					z[30:23] <= 255;
					z[22:0] <= 0;
					// if b is zero return NaN
					if (($signed(b_e) == -127) && (b_m == 0)) begin
						z[31] <= 1;
						z[30:23] <= 255;
						z[22] <= 1;
						z[21:0] <= 0;
					end
					s_output_ready <= 1;
					state <= PUT_Z;
				// if b is inf return inf
				end else if (b_e == 128) begin
					z[31] <= a_s ^ b_s;
					z[30:23] <= 255;
					z[22:0] <= 0;
					// if a is zero return NaN
					if (($signed(a_e) == -127) && (a_m == 0)) begin
						z[31] <= 1;
						z[30:23] <= 255;
						z[22] <= 1;
						z[21:0] <= 0;
					end
					s_output_ready <= 1;
					state <= PUT_Z;
				// if a is zero return zero
				end else if (($signed(a_e) == -127) && (a_m == 0)) begin
					z[31] <= a_s ^ b_s;
					z[30:23] <= 0;
					z[22:0] <= 0;
					s_output_ready <= 1;
					state <= PUT_Z;
				// if b is zero return zero
				end else if (($signed(b_e) == -127) && (b_m == 0)) begin
					z[31] <= a_s ^ b_s;
					z[30:23] <= 0;
					z[22:0] <= 0;
					s_output_ready <= 1;
					state <= PUT_Z;
				end else begin
					// Denormalised Number
					if ($signed(a_e) == -127) begin
						a_e <= -126;
					end else begin
						a_m[23] <= 1;
					end
					// Denormalised Number
					if ($signed(b_e) == -127) begin
						b_e <= -126;
					end else begin
						b_m[23] <= 1;
					end
					state <= NORMALIZE_A;
				end
			end

			NORMALIZE_A: begin
				if (a_m[23]) begin
					state <= NORMALIZE_B;
				end else begin
					a_m <= a_m << 1;
					a_e <= a_e - 1;
				end
			end

			NORMALIZE_B: begin
				if (b_m[23]) begin
					state <= MULTIPLY_0;
				end else begin
					b_m <= b_m << 1;
					b_e <= b_e - 1;
				end
			end

			MULTIPLY_0: begin
				z_s <= a_s ^ b_s;
				z_e <= a_e + b_e + 1;
				product <= a_m * b_m;
				state <= MULTIPLY_1;
			end

			MULTIPLY_1: begin
				z_m <= product[47:24];
				guard <= product[23];
				round_bit <= product[22];
				sticky <= (product[21:0] != 0);
				state <= NORMALIZE_1;
			end

			NORMALIZE_1: begin
				if (z_m[23] == 0) begin
					z_e <= z_e - 1;
					z_m <= z_m << 1;
					z_m[0] <= guard;
					guard <= round_bit;
					round_bit <= 0;
				end else begin
					state <= NORMALIZE_2;
				end
			end

			NORMALIZE_2: begin
				if ($signed(z_e) < -126) begin
					z_e <= z_e + 1;
					z_m <= z_m >> 1;
					guard <= z_m[0];
					round_bit <= guard;
					sticky <= sticky | round_bit;
				end else begin
					state <= ROUND;
				end
			end

			ROUND: begin
				if (guard && (round_bit | sticky | z_m[0])) begin
					z_m <= z_m + 1;
					if (z_m == 24'hffffff) begin
						z_e <= z_e + 1;
					end
				end
				state <= PACK;
			end

			PACK: begin
				z[22 : 0] <= z_m[22:0];
				z[30 : 23] <= z_e[7:0] + 127;
				z[31] <= z_s;
				if ($signed(z_e) == -126 && z_m[23] == 0) begin
					z[30 : 23] <= 0;
				end
				// if overflow occurs, return inf
				if ($signed(z_e) > 127) begin
					z[22 : 0] <= 0;
					z[30 : 23] <= 255;
					z[31] <= z_s;
				end
				s_output_ready <= 1;
				state <= PUT_Z;
			end

			PUT_Z: begin
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
