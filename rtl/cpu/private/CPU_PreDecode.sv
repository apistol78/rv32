`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_PreDecode(
	input wire i_reset,
	input wire i_clock,

	// Input
	input fetch_data_t i_data,

	// Output
	output fetch_data_t o_data
);
	assign o_data = data;

	fetch_data_t data = 0;

	// 
	`undef INSTRUCTION
	`define INSTRUCTION i_data.instruction
	`include "private/generated/Instructions_decode.sv"

	wire have_RS1 = is_B | is_I | is_R | is_S | is_CSR | is_R4;
	wire have_RS2 = is_B | is_R | is_S | is_R4;
	wire have_RS3 = is_R4;
	wire have_RD  = is_I | is_J | is_R | is_U | is_CSR | is_R4;

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			data <= 0;
		end
		else begin
			if (i_data.tag != data.tag) begin

				data <= i_data;

				// Decode register indices here since we
				// need those for fetching registers while
				// we are decoding rest of instruction.
				data.inst_rs1 <= register_t'(have_RS1 ? { `INSTRUCTION[19:15] } : 5'h0);
				data.inst_rs2 <= register_t'(have_RS2 ? { `INSTRUCTION[24:20] } : 5'h0);
				data.inst_rs3 <= register_t'(have_RS3 ? { `INSTRUCTION[31:27] } : 5'h0);
				data.inst_rd  <= register_t'(have_RD  ? { `INSTRUCTION[ 11:7] } : 5'h0);
				
			end
		end
	end

endmodule
