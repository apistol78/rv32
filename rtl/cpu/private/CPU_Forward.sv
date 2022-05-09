`include "CPU_Types.sv"

`timescale 1ns/1ns

module CPU_Forward (
	input decode_data_t i_decode_data,
	input execute_data_t i_execute_data,
	input memory_data_t i_memory_data,
	input writeback_data_t i_writeback_data,

	input [31:0] i_rs1,
	input [31:0] i_rs2,

	output [31:0] o_rs1,
	output [31:0] o_rs2
);
	assign o_rs1 = rs1;
	assign o_rs2 = rs2;

	bit [31:0] rs1;
	bit [31:0] rs2;

	// Forward RS1 from pipeline.
	always_comb begin
		if (i_decode_data.have_rs[0]) begin
			rs1 =
				(i_decode_data.inst_rs1 == i_execute_data.inst_rd) ? i_execute_data.rd :
				(i_decode_data.inst_rs1 == i_memory_data.inst_rd) ? i_memory_data.rd :
				(i_decode_data.inst_rs1 == i_writeback_data.inst_rd) ? i_writeback_data.rd :
				i_rs1;
		end
		else
			rs1 = 32'h0;
	end

	// Forward RS2 from pipeline.
	always_comb begin
		if (i_decode_data.have_rs[1]) begin
			rs2 =
				(i_decode_data.inst_rs2 == i_execute_data.inst_rd) ? i_execute_data.rd :
				(i_decode_data.inst_rs2 == i_memory_data.inst_rd) ? i_memory_data.rd :
				(i_decode_data.inst_rs2 == i_writeback_data.inst_rd) ? i_writeback_data.rd :
				i_rs2;
		end
		else
			rs2 = 32'h0;
	end

endmodule
