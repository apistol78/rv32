`include "CPU_Types.sv"

`timescale 1ns/1ns

module CPU_Registers #(
	parameter STACK_POINTER
)(
	input i_reset,
	input i_clock,

	input fetch_data_t i_fetch_data,
	output [31:0] o_rs1,
	output [31:0] o_rs2,
	output [31:0] o_rs3,

	input memory_data_t i_memory_data
);
	assign o_rs1 = rs1;
	assign o_rs2 = rs2;
	assign o_rs3 = rs3;

	reg [`TAG_SIZE] write_tag = 0;

	reg [31:0] r[63:0];
	reg [31:0] rs1 = 0;
	reg [31:0] rs2 = 0;
	reg [31:0] rs3 = 0;

	initial begin
		// Integer registers.
		r[ 0] = 32'h0000_0000;
		r[ 1] = 32'h0000_0000;
		r[ 2] = STACK_POINTER;
		r[ 3] = 32'h0000_0000;
		r[ 4] = 32'h0000_0000;
		r[ 5] = 32'h0000_0000;
		r[ 6] = 32'h0000_0000;
		r[ 7] = 32'h0000_0000;
		r[ 8] = 32'h0000_0000;
		r[ 9] = 32'h0000_0000;
		r[10] = 32'h0000_0000;
		r[11] = 32'h0000_0000;
		r[12] = 32'h0000_0000;
		r[13] = 32'h0000_0000;
		r[14] = 32'h0000_0000;
		r[15] = 32'h0000_0000;
		r[16] = 32'h0000_0000;
		r[17] = 32'h0000_0000;
		r[18] = 32'h0000_0000;
		r[19] = 32'h0000_0000;
		r[20] = 32'h0000_0000;
		r[21] = 32'h0000_0000;
		r[22] = 32'h0000_0000;
		r[23] = 32'h0000_0000;
		r[24] = 32'h0000_0000;
		r[25] = 32'h0000_0000;
		r[26] = 32'h0000_0000;
		r[27] = 32'h0000_0000;
		r[28] = 32'h0000_0000;
		r[29] = 32'h0000_0000;
		r[30] = 32'h0000_0000;
		r[31] = 32'h0000_0000;

		// Float point registers.   
	end

	always_ff @(posedge i_clock, posedge i_reset)
	begin
		if (i_reset) begin
			rs1 <= 0;
			rs2 <= 0;

			write_tag <= 0;

			// Integer registers.
			r[ 0] <= 32'h0000_0000;
			r[ 1] <= 32'h0000_0000;
			r[ 2] <= STACK_POINTER;
			r[ 3] <= 32'h0000_0000;
			r[ 4] <= 32'h0000_0000;
			r[ 5] <= 32'h0000_0000;
			r[ 6] <= 32'h0000_0000;
			r[ 7] <= 32'h0000_0000;
			r[ 8] <= 32'h0000_0000;
			r[ 9] <= 32'h0000_0000;
			r[10] <= 32'h0000_0000;
			r[11] <= 32'h0000_0000;
			r[12] <= 32'h0000_0000;
			r[13] <= 32'h0000_0000;
			r[14] <= 32'h0000_0000;
			r[15] <= 32'h0000_0000;
			r[16] <= 32'h0000_0000;
			r[17] <= 32'h0000_0000;
			r[18] <= 32'h0000_0000;
			r[19] <= 32'h0000_0000;
			r[20] <= 32'h0000_0000;
			r[21] <= 32'h0000_0000;
			r[22] <= 32'h0000_0000;
			r[23] <= 32'h0000_0000;
			r[24] <= 32'h0000_0000;
			r[25] <= 32'h0000_0000;
			r[26] <= 32'h0000_0000;
			r[27] <= 32'h0000_0000;
			r[28] <= 32'h0000_0000;
			r[29] <= 32'h0000_0000;
			r[30] <= 32'h0000_0000;
			r[31] <= 32'h0000_0000;

			// Float point registers.
		end
		else begin

			// Read first.
			rs1 <= (i_fetch_data.inst_rs1 != 0) ? r[i_fetch_data.inst_rs1] : 32'h0;
			rs2 <= (i_fetch_data.inst_rs2 != 0) ? r[i_fetch_data.inst_rs2] : 32'h0;
			rs3 <= (i_fetch_data.inst_rs3 != 0) ? r[i_fetch_data.inst_rs3] : 32'h0;

			// Write later.
			if (i_memory_data.tag != write_tag) begin
				if (|i_memory_data.inst_rd) 
					r[i_memory_data.inst_rd] <= i_memory_data.rd;
				write_tag <= i_memory_data.tag;
			end
		end
	end

endmodule