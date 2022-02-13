`include "CPU_Types.sv"

`timescale 1ns/1ns

module CPU_Forward (
	input decode_data_t i_decode_data,
	input execute_data_t i_execute_data,
	input memory_data_t i_memory_data,
	input writeback_data_t i_writeback_data,

	input [31:0] i_rs1,
	input [31:0] i_rs2,
	input [31:0] i_rs3,

	output [31:0] o_rs1,
	output [31:0] o_rs2,
	output [31:0] o_rs3,

	output o_memory_raw
);
	assign o_rs1 = rs1;
	assign o_rs2 = rs2;
	assign o_rs3 = rs3;

	logic [31:0] rs1;
	logic [31:0] rs2;
	logic [31:0] rs3;

	// Read-After-Write hazard detection.
	always_comb begin
		if (
			i_execute_data.tag != i_memory_data.tag &&
			i_execute_data.mem_read &&
			(
				i_execute_data.inst_rd == i_decode_data.inst_rs1 ||
				i_execute_data.inst_rd == i_decode_data.inst_rs2 ||
				i_execute_data.inst_rd == i_decode_data.inst_rs3
			)
		)
			o_memory_raw = 1'b1;
		else
			o_memory_raw = 1'b0;
	end

	// Forward RS1 from pipeline.
	always_comb begin
		rs1 =
			(i_decode_data.inst_rs1 == 0) ? 32'h0 :
			(i_decode_data.inst_rs1 == i_execute_data.inst_rd && i_execute_data.mem_read == 0) ? i_execute_data.rd :
			(i_decode_data.inst_rs1 == i_memory_data.inst_rd) ? i_memory_data.rd :
			(i_decode_data.inst_rs1 == i_writeback_data.inst_rd) ? i_writeback_data.rd :
			i_rs1;
	end

	// Forward RS2 from pipeline.
	always_comb begin
		rs2 =
			(i_decode_data.inst_rs2 == 0) ? 32'h0 :
			(i_decode_data.inst_rs2 == i_execute_data.inst_rd && i_execute_data.mem_read == 0) ? i_execute_data.rd :
			(i_decode_data.inst_rs2 == i_memory_data.inst_rd) ? i_memory_data.rd :
			(i_decode_data.inst_rs2 == i_writeback_data.inst_rd) ? i_writeback_data.rd :
			i_rs2;
	end

	// Forward RS3 from pipeline.
	always_comb begin
		rs3 =
			(i_decode_data.inst_rs3 == 0) ? 32'h0 :
			(i_decode_data.inst_rs3 == i_execute_data.inst_rd && i_execute_data.mem_read == 0) ? i_execute_data.rd :
			(i_decode_data.inst_rs3 == i_memory_data.inst_rd) ? i_memory_data.rd :
			(i_decode_data.inst_rs3 == i_writeback_data.inst_rd) ? i_writeback_data.rd :
			i_rs3;
	end

endmodule
