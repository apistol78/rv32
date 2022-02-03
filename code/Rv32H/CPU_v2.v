`include "CPU_Defines.v"

`timescale 1ns/1ns

// https://zipcpu.com/blog/2017/08/14/strategies-for-pipelining.html

module CPU_v2 (
	input wire i_reset,
	input wire i_clock,					// CPU clock

	// Control
	input wire i_interrupt,

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
	output wire o_fault,
	output reg [31:0] o_retire_count
);

	//====================================================
	// CSR

	wire [11:0] csr_index;
	wire [31:0] csr_rdata;
	wire csr_wdata_wr;
	wire [31:0] csr_wdata;
	wire [31:0] csr_epc;
	wire csr_irq_pending;
	wire [31:0] csr_irq_pc;
	wire csr_irq_dispatched;
	wire [31:0] csr_irq_epc;

	CPU_CSR csr(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_interrupt(i_interrupt),

		.i_index(csr_index),
		.o_rdata(csr_rdata),
		.i_wdata_wr(csr_wdata_wr),
		.i_wdata(csr_wdata),
		.o_epc(csr_epc),

		.o_irq_pending(csr_irq_pending),
		.o_irq_pc(csr_irq_pc),
		.i_irq_dispatched(csr_irq_dispatched),
		.i_irq_epc(csr_irq_epc)
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

		.i_read(fetch_valid),
		.i_read_rs1_idx(fetch_inst_rs1),
		.i_read_rs2_idx(fetch_inst_rs2),
		.o_rs1(rs1),
		.o_rs2(rs2),

		.i_write(memory_valid),
		.i_write_rd_idx(memory_inst_rd),
		.i_rd(memory_rd)
	);

	//====================================================
	// FETCH

	wire fetch_valid;
	wire [`TAG_SIZE] fetch_tag;
	wire [31:0] fetch_instruction;
	wire [31:0] fetch_pc;
	
	CPU_Fetch fetch(
		.i_reset(i_reset),
		.i_clock(i_clock),

		// Control
		.i_decode_busy(decode_busy),
		.o_valid(fetch_valid),
		.i_jump(execute_jump),
		.i_jump_pc(execute_jump_pc),

		// Debug
		.o_tag(fetch_tag),

		// Interrupt
		.i_irq_pending(csr_irq_pending),
		.i_irq_pc(csr_irq_pc),
		.o_irq_dispatched(csr_irq_dispatched),
		.o_irq_epc(csr_irq_epc),

		// Bus
		.o_bus_request(o_ibus_request),
		.i_bus_ready(i_ibus_ready),
		.o_bus_address(o_ibus_address),
		.i_bus_rdata(i_ibus_rdata),

		// Output
		.o_instruction(fetch_instruction),
		.o_pc(fetch_pc)
	);

	//====================================================
	// DECODE

	wire decode_busy;
	wire decode_valid;
	wire decode_fault;
	wire [`TAG_SIZE] decode_tag;
	wire [31:0] decode_instruction;
	wire [31:0] decode_pc;
	wire [4:0] decode_inst_rs1;
	wire [4:0] decode_inst_rs2;
	wire [4:0] decode_inst_rd;
	wire [31:0] decode_imm;
	
	wire decode_arithmetic;
	wire decode_compare;
	wire decode_complex;
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

		// Control
		.i_execute_busy(execute_busy),
		.i_fetch_valid(fetch_valid),
		.o_busy(decode_busy),
		.o_valid(decode_valid),
		.o_fault(decode_fault),

		// Debug
		.i_tag(fetch_tag),
		.o_tag(decode_tag),

		// Input
		.i_instruction(fetch_instruction),
		.i_pc(fetch_pc),

		// Output
		.o_instruction(decode_instruction),
		.o_pc(decode_pc),
		.o_inst_rs1(decode_inst_rs1),
		.o_inst_rs2(decode_inst_rs2),
		.o_inst_rd(decode_inst_rd),
		.o_imm(decode_imm),
		
		.o_arithmetic(decode_arithmetic),
		.o_compare(decode_compare),
		.o_complex(decode_complex),
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
	// HAZARD

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

	//====================================================
	// EXECUTE

	wire execute_busy;
	wire execute_valid;
	wire execute_jump;
	wire [31:0] execute_jump_pc;
	wire execute_fault;
	wire [`TAG_SIZE] execute_tag;
	wire [4:0] execute_inst_rd;
	wire [31:0] execute_rd;
	wire execute_mem_read;
	wire execute_mem_write;
	wire execute_mem_flush;
	wire [2:0] execute_mem_width;
	wire execute_mem_signed;
	wire [31:0] execute_mem_address;
	wire [31:0] execute_mem_wdata;
	
	CPU_Execute execute(
		.i_reset(i_reset),
		.i_clock(i_clock),

		// CSR
		.o_csr_index(csr_index),
		.i_csr_rdata(csr_rdata),
		.o_csr_wdata_wr(csr_wdata_wr),
		.o_csr_wdata(csr_wdata),
		.i_epc(csr_epc),

		// Control
		.i_memory_busy(memory_busy),
		.i_decode_valid(decode_valid),
		.o_busy(execute_busy),
		.o_valid(execute_valid),
		.o_jump(execute_jump),
		.o_jump_pc(execute_jump_pc),
		.o_fault(execute_fault),

		// Debug
		.i_tag(decode_tag),
		.o_tag(execute_tag),

		// Input from decode.
		.i_pc(decode_pc),
		.i_instruction(decode_instruction),
		.i_rs1(fwd_rs1),
		.i_rs2(fwd_rs2),
		.i_inst_rd(decode_inst_rd),
		.i_imm(decode_imm),
		.i_arithmetic(decode_arithmetic),
		.i_compare(decode_compare),
		.i_complex(decode_complex),
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
		.o_inst_rd(execute_inst_rd),
		.o_rd(execute_rd),
		.o_mem_read(execute_mem_read),
		.o_mem_write(execute_mem_write),
		.o_mem_flush(execute_mem_flush),
		.o_mem_width(execute_mem_width),
		.o_mem_signed(execute_mem_signed),
		.o_mem_address(execute_mem_address)
	);

	//====================================================
	// MEMORY

	wire memory_busy;
	wire memory_valid;
	wire [`TAG_SIZE] memory_tag;
	wire [4:0] memory_inst_rd;
	wire [31:0] memory_rd;

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

		// Control
		.i_writeback_busy(writeback_busy),
		.i_execute_valid(execute_valid),
		.o_busy(memory_busy),
		.o_valid(memory_valid),

		// Debug
		.i_tag(execute_tag),
		.o_tag(memory_tag),

		// Input from execute.
		.i_inst_rd(execute_inst_rd),
		.i_rd(execute_rd),
		.i_mem_read(execute_mem_read),
		.i_mem_write(execute_mem_write),
		.i_mem_flush(execute_mem_flush),
		.i_mem_width(execute_mem_width),
		.i_mem_signed(execute_mem_signed),
		.i_mem_address(execute_mem_address),

		// Output from memory.
		.o_inst_rd(memory_inst_rd),
		.o_rd(memory_rd)
	);

	//====================================================
	// WRITEBACK

	wire writeback_busy;
	wire writeback_valid;
	wire [`TAG_SIZE] writeback_tag;
	wire [4:0] writeback_inst_rd;
	wire [31:0] writeback_rd;
	
	CPU_Writeback writeback(
		.i_reset(i_reset),
		.i_clock(i_clock),

		// Control
		.i_memory_valid(memory_valid),
		.o_busy(writeback_busy),
		.o_valid(writeback_valid),

		// Debug
		.i_tag(memory_tag),
		.o_tag(writeback_tag),
	
		// Input from memory.
		.i_inst_rd(memory_inst_rd),
		.i_rd(memory_rd),

		// Output from writeback.
		.o_inst_rd(writeback_inst_rd),
		.o_rd(writeback_rd)
	);

	//====================================================
	
	assign o_fault = decode_fault || execute_fault;

	reg [`TAG_SIZE] retire_tag = 0;

	initial o_retire_count = 0;

	always @(posedge i_clock) begin
		if (i_reset) begin
			retire_tag <= 0;
			o_retire_count <= 0;
		end
		else begin
			if (writeback_tag != retire_tag) begin
				o_retire_count <= o_retire_count + 1;
				retire_tag <= writeback_tag;
			end
		end
	end
	
endmodule
