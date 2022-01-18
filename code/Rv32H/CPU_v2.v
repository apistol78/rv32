`include "CPU_Defines.v"

`timescale 1ns/1ns

// 5831 ALUT
// 5178 DLR

module CPU_v2 (
	input wire i_reset,
	input wire i_clock,					// CPU clock

	// Instruction bus
	output wire o_ibus_request,			// IO request.
	input wire i_ibus_ready,			// IO request ready.
	output wire [31:0] o_ibus_address,	// Address
	input wire [31:0] i_ibus_rdata,		// Read data
	
	// Data bus
	output wire o_dbus_rw,				// Data read/write
	output wire o_dbus_request,			// IO request.
	input wire i_dbus_ready,			// IO request ready.
	output wire [31:0] o_dbus_address,	// Address
	input wire [31:0] i_dbus_rdata,		// Read data
	output wire [31:0] o_dbus_wdata,	// Write data
	
	// Debug
	output reg [31:0] o_retire_count,
	output wire [31:0] o_icache_hit_count,
	output wire [31:0] o_icache_miss_count,
	output wire [31:0] o_dcache_hit_count,
	output wire [31:0] o_dcache_miss_count
);

	//====================================================
	// REGISTERS

	// RS1 and RS2 are read from file
	// simultaneously as decode stage.

	wire [31:0] rs1;
	wire [31:0] rs2;

	wire [4:0] fetch_inst_rs1 = fetch_instruction[19:15];
	wire [4:0] fetch_inst_rs2 = fetch_instruction[24:20];

	CPU_Registers registers(
		.i_reset(i_reset),
		.i_clock(i_clock),

		.i_read_tag(fetch_tag),
		.i_read_rs1_idx(fetch_inst_rs1),
		.i_read_rs2_idx(fetch_inst_rs2),
		.o_rs1(rs1),
		.o_rs2(rs2),

		.i_write_tag(memory_tag),
		.i_write_rd_idx(memory_inst_rd),
		.i_rd(memory_rd)
	);

	//====================================================
	// FETCH

	wire [`TAG_SIZE] fetch_tag;
	wire [31:0] fetch_instruction;
	wire [31:0] fetch_pc;
	
	CPU_Fetch fetch(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_stall(memory_stall || execute_stall),

		// Bus
		.o_bus_request(o_ibus_request),
		.i_bus_ready(i_ibus_ready),
		.o_bus_address(o_ibus_address),
		.i_bus_rdata(i_ibus_rdata),

		// Input
		.i_tag(execute_tag), //writeback_tag),
		.i_pc_next(execute_pc_next), //writeback_pc_next),

		// Output
		.o_tag(fetch_tag),
		.o_instruction(fetch_instruction),
		.o_pc(fetch_pc),

		// Debug
		.o_icache_hit_count(o_icache_hit_count),
		.o_icache_miss_count(o_icache_miss_count)
	);

	//====================================================
	// DECODE

	wire [`TAG_SIZE] decode_tag;
	wire [31:0] decode_instruction;
	wire [31:0] decode_pc;
	wire [4:0] decode_inst_rs1;
	wire [4:0] decode_inst_rs2;
	wire [4:0] decode_inst_rd;
	wire [31:0] decode_imm;
	
	wire decode_arithmetic;
	wire decode_compare;
	wire decode_jump;
	wire decode_jump_conditional;

	wire [3:0] decode_alu_operation;
	wire [2:0] decode_alu_operand1;
	wire [2:0] decode_alu_operand2;
	
	wire decode_memory_read;
	wire decode_memory_write;
	wire [2:0] decode_memory_width;
	wire decode_memory_signed;

	wire [4:0] decode_op;

	CPU_Decode decode(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_stall(memory_stall || execute_stall),
	
		// Input
		.i_tag(fetch_tag),
		.i_instruction(fetch_instruction),
		.i_pc(fetch_pc),

		// Output
		.o_tag(decode_tag),
		.o_instruction(decode_instruction),
		.o_pc(decode_pc),
		.o_inst_rs1(decode_inst_rs1),
		.o_inst_rs2(decode_inst_rs2),
		.o_inst_rd(decode_inst_rd),
		.o_imm(decode_imm),
		
		.o_arithmetic(decode_arithmetic),
		.o_compare(decode_compare),
		.o_jump(decode_jump),
		.o_jump_conditional(decode_jump_conditional),

		.o_alu_operation(decode_alu_operation),
		.o_alu_operand1(decode_alu_operand1),
		.o_alu_operand2(decode_alu_operand2),
		
		.o_memory_read(decode_memory_read),
		.o_memory_write(decode_memory_write),
		.o_memory_width(decode_memory_width),
		.o_memory_signed(decode_memory_signed),

		.o_op(decode_op)
	);

	//====================================================
	// EXECUTE

	// Forward register values from pipeline if already in flight.
	wire [31:0] fwd_rs1 = 
		(decode_inst_rs1 == 0) ? 32'h0 :
		(decode_inst_rs1 == execute_inst_rd && execute_mem_read == 0) ? execute_rd :
		(decode_inst_rs1 == memory_inst_rd) ? memory_rd :
		(decode_inst_rs1 == writeback_inst_rd) ? writeback_rd :
		rs1;

	wire [31:0] fwd_rs2 =
		(decode_inst_rs2 == 0) ? 32'h0 :
		(decode_inst_rs2 == execute_inst_rd && execute_mem_read == 0) ? execute_rd :
		(decode_inst_rs2 == memory_inst_rd) ? memory_rd :
		(decode_inst_rs2 == writeback_inst_rd) ? writeback_rd :
		rs2;

	wire [`TAG_SIZE] execute_tag;
	wire [4:0] execute_inst_rd;
	wire [31:0] execute_rd;
	wire [31:0] execute_pc_next;
	wire execute_mem_read;
	wire execute_mem_write;
	wire [2:0] execute_mem_width;
	wire execute_mem_signed;
	wire [31:0] execute_mem_address;
	wire [31:0] execute_mem_wdata;
	wire execute_stall;
	
	CPU_Execute execute(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_stall(memory_stall),

		// Input from decode.
		.i_tag(decode_tag),
		.i_pc(decode_pc),
		.i_instruction(decode_instruction),
		.i_rs1(fwd_rs1),
		.i_rs2(fwd_rs2),
		.i_inst_rd(decode_inst_rd),
		.i_imm(decode_imm),

		.i_arithmetic(decode_arithmetic),
		.i_compare(decode_compare),
		.i_jump(decode_jump),
		.i_jump_conditional(decode_jump_conditional),

		.i_alu_operation(decode_alu_operation),
		.i_alu_operand1(decode_alu_operand1),
		.i_alu_operand2(decode_alu_operand2),

		.i_memory_read(decode_memory_read),
		.i_memory_write(decode_memory_write),
		.i_memory_width(decode_memory_width),
		.i_memory_signed(decode_memory_signed),

		.i_op(decode_op),

		// Output from execute.
		.o_tag(execute_tag),
		.o_inst_rd(execute_inst_rd),
		.o_rd(execute_rd),
		.o_pc_next(execute_pc_next),
		.o_mem_read(execute_mem_read),
		.o_mem_write(execute_mem_write),
		.o_mem_width(execute_mem_width),
		.o_mem_signed(execute_mem_signed),
		.o_mem_address(execute_mem_address),
		.o_stall(execute_stall)
	);

	//====================================================
	// MEMORY

	wire [`TAG_SIZE] memory_tag;
	wire [4:0] memory_inst_rd;
	wire [31:0] memory_rd;
	wire [31:0] memory_pc_next;
	wire memory_stall;

	CPU_Memory memory(
		.i_reset(i_reset),
		.i_clock(i_clock),
	
		// Bus
		.o_bus_rw(o_dbus_rw),
		.o_bus_request(o_dbus_request),
		.i_bus_ready(i_dbus_ready),
		.o_bus_address(o_dbus_address),
		.i_bus_rdata(i_dbus_rdata),
		.o_bus_wdata(o_dbus_wdata),

		// Input from execute.
		.i_tag(execute_tag),
		.i_inst_rd(execute_inst_rd),
		.i_rd(execute_rd),
		.i_pc_next(execute_pc_next),
		.i_mem_read(execute_mem_read),
		.i_mem_write(execute_mem_write),
		.i_mem_width(execute_mem_width),
		.i_mem_signed(execute_mem_signed),
		.i_mem_address(execute_mem_address),

		// Output from memory.
		.o_tag(memory_tag),
		.o_inst_rd(memory_inst_rd),
		.o_rd(memory_rd),
		.o_pc_next(memory_pc_next),
		.o_stall(memory_stall),

		// Debug
		.o_dcache_hit_count(o_dcache_hit_count),
		.o_dcache_miss_count(o_dcache_miss_count)
	);

	//====================================================
	// WRITEBACK

	wire [`TAG_SIZE] writeback_tag;
	wire [4:0] writeback_inst_rd;
	wire [31:0] writeback_rd;
	wire [31:0] writeback_pc_next;
	
	CPU_Writeback writeback(
		.i_reset(i_reset),
		.i_clock(i_clock),
	
		// Input from memory.
		.i_tag(memory_tag),
		.i_inst_rd(memory_inst_rd),
		.i_rd(memory_rd),
		.i_pc_next(memory_pc_next),

		// Output from writeback.
		.o_tag(writeback_tag),
		.o_inst_rd(writeback_inst_rd),
		.o_rd(writeback_rd),
		.o_pc_next(writeback_pc_next)
	);

	//====================================================
	
	reg [`TAG_SIZE] retire_tag = 0;

	initial o_retire_count = 0;

	always @(posedge i_clock) begin
		if (i_reset) begin
			retire_tag <= 0;
			o_retire_count <= 0;
		end
		else begin
			if (memory_tag != retire_tag) begin
				o_retire_count <= o_retire_count + 1;
				retire_tag <= memory_tag;
			end
		end
	end
	
endmodule
