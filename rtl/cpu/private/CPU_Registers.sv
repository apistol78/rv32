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

	bit last_write_strobe = 1'b0;

`ifdef FPU_ENABLE
	bit [31:0] r[63:0];
`else
	bit [31:0] r[31:0];
`endif

	bit [31:0] rs1 = 0;
	bit [31:0] rs2 = 0;
	bit [31:0] rs3 = 0;
	integer I;

	initial begin
		// Integer registers.
		for (I = 0; I < 32; ++I)
			r[I] = 32'h0000_0000;

`ifdef FPU_ENABLE
		// Float point registers.
		for (I = 32; I < 64; ++I)
			r[I] = 32'h0000_0000;
`endif

		r[ 2] = STACK_POINTER;
	end

	always_ff @(posedge i_clock, posedge i_reset)
	begin
		if (i_reset) begin
			rs1 <= 0;
			rs2 <= 0;
			rs3 <= 0;

			last_write_strobe <= 1'b0;

		// Integer registers.
		for (I = 0; I < 32; ++I)
			r[I] = 32'h0000_0000;

`ifdef FPU_ENABLE
		// Float point registers.
		for (I = 32; I < 64; ++I)
			r[I] = 32'h0000_0000;
`endif

		r[ 2] = STACK_POINTER;
		end
		else begin

			// Read first.
			rs1 <= (i_fetch_data.inst_rs1 != 0) ? r[i_fetch_data.inst_rs1] : 32'h0;
			rs2 <= (i_fetch_data.inst_rs2 != 0) ? r[i_fetch_data.inst_rs2] : 32'h0;
			rs3 <= (i_fetch_data.inst_rs3 != 0) ? r[i_fetch_data.inst_rs3] : 32'h0;

			// Write later.
			if (i_memory_data.strobe != last_write_strobe) begin
				if (|i_memory_data.inst_rd) 
					r[i_memory_data.inst_rd] <= i_memory_data.rd;
				last_write_strobe <= i_memory_data.strobe;
			end
		end
	end

endmodule