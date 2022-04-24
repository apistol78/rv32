`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_FPU_MulAdd(
	input i_reset,
	input i_clock,

	input i_request,
	input [31:0] i_op1,
	input [31:0] i_op2,
	input [31:0] i_op3,

	output o_ready,
	output [31:0] o_result
);

	typedef enum bit [4:0]
	{
		IDLE,
		SPECIAL_CASES,
		M_NORMALIZE_A,
		M_NORMALIZE_B,
		M_MULTIPLY_0,
		M_MULTIPLY_1,
		M_NORMALIZE_1,
		M_NORMALIZE_2,
		M_ROUND,
		A_UNPACK,
		A_ALIGN,
		A_ADD_0,
		A_ADD_1,
		A_NORMALIZE_1,
		A_NORMALIZE_2,
		A_ROUND,
		PACK,
		PUT_Z
	} state_t;

	reg s_output_ready = 0;
	reg [31:0] s_output_z = 0;
	state_t state = IDLE;

	reg [31:0] t, z;
	
	reg [23:0] a_m, b_m;
	reg [26:0] c_m, t_m;
	reg [23:0] z_m;

	reg [9:0] a_e, b_e, c_e, t_e, z_e;
	
	reg a_s, b_s, c_s, t_s, z_s;

	reg guard, round_bit, sticky;
	reg [47:0] product;
	reg [27:0] sum;

	assign o_ready = s_output_ready;
	assign o_result = s_output_z;

	always_ff @(posedge i_clock) begin
		case(state)
			IDLE: begin
				s_output_ready <= 0;
				if (i_request) begin
					a_m <= i_op1[22:0];
					b_m <= i_op2[22:0];
					c_m <= { i_op3[22:0], 3'd0 };

					a_e <= i_op1[30:23] - 127;
					b_e <= i_op2[30:23] - 127;
					c_e <= i_op3[30:23] - 127;
					
					a_s <= i_op1[31];
					b_s <= i_op2[31];
					c_s <= i_op3[31];

					t <= i_op3;

					state <= SPECIAL_CASES;
				end
			end

			SPECIAL_CASES: begin
				// if a, b or c is NaN return NaN 
				if (
					(a_e == 128 && a_m != 0) ||
					(b_e == 128 && b_m != 0) ||
					(c_e == 128 && c_m != 0)
				) begin
					z[31] <= 1;
					z[30:23] <= 255;
					z[22] <= 1;
					z[21:0] <= 0;
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
					state <= PUT_Z;
				// if c is inf return inf
				end else if (c_e == 128) begin
					z[31] <= 1'b0; // a_s ^ b_s;
					z[30:23] <= 255;
					z[22:0] <= 0;
					// if a is zero return NaN
					// if (($signed(a_e) == -127) && (a_m == 0)) begin
					// 	z[31] <= 1;
					// 	z[30:23] <= 255;
					// 	z[22] <= 1;
					// 	z[21:0] <= 0;
					// end
					state <= PUT_Z;

				// If a or b is zero we return add.
				end else if (
					(($signed(a_e) == -127) && (a_m == 0)) ||
					(($signed(b_e) == -127) && (b_m == 0))
				) begin
					z <= t;
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
					// Denormalised Number
					if ($signed(c_e) == -127) begin
						c_e <= -126;
					end else begin
						c_m[26] <= 1;
					end					
					state <= M_NORMALIZE_A;
				end
			end

///

			M_NORMALIZE_A: begin
				if (a_m[23]) begin
					state <= M_NORMALIZE_B;
				end else begin
					a_m <= a_m << 1;
					a_e <= a_e - 1;
				end
			end

			M_NORMALIZE_B: begin
				if (b_m[23]) begin
					state <= M_MULTIPLY_0;
				end else begin
					b_m <= b_m << 1;
					b_e <= b_e - 1;
				end
			end

			M_MULTIPLY_0: begin
				t_s <= a_s ^ b_s;
				t_e <= a_e + b_e + 1;
				product <= a_m * b_m;
				state <= M_MULTIPLY_1;
			end

			M_MULTIPLY_1: begin
				t_m <= product[47:24];
				guard <= product[23];
				round_bit <= product[22];
				sticky <= (product[21:0] != 0);
				state <= M_NORMALIZE_1;
			end

			M_NORMALIZE_1: begin
				if (t_m[23] == 0) begin
					t_e <= t_e - 1;
					t_m <= t_m << 1;
					t_m[0] <= guard;
					guard <= round_bit;
					round_bit <= 0;
				end else begin
					state <= M_NORMALIZE_2;
				end
			end

			M_NORMALIZE_2: begin
				if ($signed(t_e) < -126) begin
					t_e <= t_e + 1;
					t_m <= t_m >> 1;
					guard <= t_m[0];
					round_bit <= guard;
					sticky <= sticky | round_bit;
				end else begin
					state <= M_ROUND;
				end
			end

			M_ROUND: begin
				if (guard && (round_bit | sticky | t_m[0])) begin
					t_m <= t_m + 1;
					if (t_m == 24'hffffff) begin
						t_e <= t_e + 1;
					end
				end
				state <= A_UNPACK;
			end

///

			A_UNPACK: begin
				t_m <= t_m << 3;
				state <= A_ALIGN;
			end

			A_ALIGN: begin
				if ($signed(c_e) > $signed(t_e)) begin
					t_e <= t_e + 1;
					t_m <= t_m >> 1;
					t_m[0] <= t_m[0] | t_m[1];
				end else if ($signed(c_e) < $signed(t_e)) begin
					c_e <= c_e + 1;
					c_m <= c_m >> 1;
					c_m[0] <= c_m[0] | c_m[1];
				end else begin
					state <= A_ADD_0;
				end
			end

			A_ADD_0: begin
				z_e <= c_e;
				if (c_s == t_s) begin
					sum <= c_m + t_m;
					z_s <= c_s;
				end else begin
					if (c_m >= t_m) begin
						sum <= c_m - t_m;
						z_s <= c_s;
					end else begin
						sum <= t_m - c_m;
						z_s <= t_s;
					end
				end
				state <= A_ADD_1;
			end

			A_ADD_1: begin
				if (sum[27]) begin
					z_m <= sum[27:4];
					guard <= sum[3];
					round_bit <= sum[2];
					sticky <= sum[1] | sum[0];
					z_e <= z_e + 1;
				end else begin
					z_m <= sum[26:3];
					guard <= sum[2];
					round_bit <= sum[1];
					sticky <= sum[0];
				end
				state <= A_NORMALIZE_1;
			end

			A_NORMALIZE_1: begin
				if (z_m[23] == 0 && $signed(z_e) > -126) begin
					z_e <= z_e - 1;
					z_m <= z_m << 1;
					z_m[0] <= guard;
					guard <= round_bit;
					round_bit <= 0;
				end else begin
					state <= A_NORMALIZE_2;
				end
			end

			A_NORMALIZE_2: begin
				if ($signed(z_e) < -126) begin
					z_e <= z_e + 1;
					z_m <= z_m >> 1;
					guard <= z_m[0];
					round_bit <= guard;
					sticky <= sticky | round_bit;
				end else begin
					state <= A_ROUND;
				end
			end

			A_ROUND: begin
				if (guard && (round_bit | sticky | z_m[0])) begin
					z_m <= z_m + 1;
					if (z_m == 24'hffffff) begin
						z_e <= z_e + 1;
					end
				end
				state <= PACK;
			end

///

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
