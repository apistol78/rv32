`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_FPU_Add(
	input i_reset,
	input i_clock,

	input i_request,
	input [31:0] i_op1,
	input [31:0] i_op2,

	output o_ready,
	output [31:0] o_result
);

	typedef enum // [3:0]
	{
		IDLE,
		UNPACK,
		SPECIAL_CASES,
		ALIGN,
		ADD_0,
		ADD_1,
		NORMALIZE_1,
		NORMALIZE_2,
		ROUND,
		PACK,
		PUT_Z
	} state_t;

	reg s_output_ready = 0;
	reg [31:0] s_output_z = 0;
	state_t state = IDLE;

	reg [31:0] a, b, z;
	reg [26:0] a_m, b_m;
	reg [23:0] z_m;
	reg [9:0] a_e, b_e, z_e;
	reg a_s, b_s, z_s;
	reg guard, round_bit, sticky;
	reg [27:0] sum;

	assign o_ready = s_output_ready;
	assign o_result = s_output_z;

	always_ff @(posedge i_clock) begin
		case(state)
			IDLE: begin
				s_output_ready <= 0;
				a <= i_op1;
				b <= i_op2;
				if (i_request)
					state <= UNPACK;
			end

			UNPACK: begin
				a_m <= { a[22 : 0], 3'd0 };
				b_m <= { b[22 : 0], 3'd0 };
				a_e <= a[30 : 23] - 127;
				b_e <= b[30 : 23] - 127;
				a_s <= a[31];
				b_s <= b[31];
				state <= SPECIAL_CASES;
			end

			SPECIAL_CASES: begin
				// if a is NaN or b is NaN return NaN 
				if ((a_e == 128 && a_m != 0) || (b_e == 128 && b_m != 0)) begin
					z[31] <= 1;
					z[30:23] <= 255;
					z[22] <= 1;
					z[21:0] <= 0;
					state <= PUT_Z;
				// if a is inf return inf
				end else if (a_e == 128) begin
					z[31] <= a_s;
					z[30:23] <= 255;
					z[22:0] <= 0;
					// if a is inf and signs don't match return nan
					if ((b_e == 128) && (a_s != b_s)) begin
							z[31] <= b_s;
							z[30:23] <= 255;
							z[22] <= 1;
							z[21:0] <= 0;
					end
					state <= PUT_Z;
				// if b is inf return inf
				end else if (b_e == 128) begin
					z[31] <= b_s;
					z[30:23] <= 255;
					z[22:0] <= 0;
					state <= PUT_Z;
				//if a is zero return b
				end else if ((($signed(a_e) == -127) && (a_m == 0)) && (($signed(b_e) == -127) && (b_m == 0))) begin
					z[31] <= a_s & b_s;
					z[30:23] <= b_e[7:0] + 127;
					z[22:0] <= b_m[26:3];
					state <= PUT_Z;
				//if a is zero return b
				end else if (($signed(a_e) == -127) && (a_m == 0)) begin
					z[31] <= b_s;
					z[30:23] <= b_e[7:0] + 127;
					z[22:0] <= b_m[26:3];
					state <= PUT_Z;
				//if b is zero return a
				end else if (($signed(b_e) == -127) && (b_m == 0)) begin
					z[31] <= a_s;
					z[30:23] <= a_e[7:0] + 127;
					z[22:0] <= a_m[26:3];
					state <= PUT_Z;
				end else begin
					//Denormalised Number
					if ($signed(a_e) == -127) begin
						a_e <= -126;
					end else begin
						a_m[26] <= 1;
					end
					//Denormalised Number
					if ($signed(b_e) == -127) begin
						b_e <= -126;
					end else begin
						b_m[26] <= 1;
					end
					state <= ALIGN;
				end
			end

			ALIGN: begin
				if ($signed(a_e) > $signed(b_e)) begin
					b_e <= b_e + 1;
					b_m <= b_m >> 1;
					b_m[0] <= b_m[0] | b_m[1];
				end else if ($signed(a_e) < $signed(b_e)) begin
					a_e <= a_e + 1;
					a_m <= a_m >> 1;
					a_m[0] <= a_m[0] | a_m[1];
				end else begin
					state <= ADD_0;
				end
			end

			ADD_0: begin
				z_e <= a_e;
				if (a_s == b_s) begin
					sum <= a_m + b_m;
					z_s <= a_s;
				end else begin
					if (a_m >= b_m) begin
						sum <= a_m - b_m;
						z_s <= a_s;
					end else begin
						sum <= b_m - a_m;
						z_s <= b_s;
					end
				end
				state <= ADD_1;
			end

			ADD_1: begin
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
				state <= NORMALIZE_1;
			end

			NORMALIZE_1: begin
				if (z_m[23] == 0 && $signed(z_e) > -126) begin
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
						z_e <=z_e + 1;
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
				if ($signed(z_e) == -126 && z_m[23:0] == 24'h0) begin
					z[31] <= 1'b0; // FIX SIGN BUG: -a + a = +0.
				end
				//if overflow occurs, return inf
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
