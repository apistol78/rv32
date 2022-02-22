`include "private/CPU_Types.sv"

// https://zipcpu.com/blog/2017/08/14/strategies-for-pipelining.html

`timescale 1ns/1ns

module CPU #(
	parameter RESET_VECTOR = 32'h00000000,
	parameter STACK_POINTER = 32'h10000400,
	parameter VENDORID = 32'h0,
	parameter ARCHID = 32'h0,
	parameter IMPID = 32'h0,
	parameter HARTID = 32'h0	
)(
	input i_reset,
	input i_clock,					// CPU clock

	// Control
	input i_timer_interrupt,
	input i_external_interrupt,

	// Instruction bus
	output o_ibus_request,			// IO request.
	input i_ibus_ready,			// IO request ready.
	output [31:0] o_ibus_address,	// Address
	input [31:0] i_ibus_rdata,		// Read data
	
	// Data bus
	output o_dbus_rw,				// Data read/write
	output o_dbus_request,			// IO request.
	input i_dbus_ready,			// IO request ready.
	output [31:0] o_dbus_address,	// Address
	input [31:0] i_dbus_rdata,		// Read data
	output [31:0] o_dbus_wdata,	// Write data
	
	// Debug
	output o_fault
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

	CPU_CSR #(
		.VENDORID(VENDORID),
		.ARCHID(ARCHID),
		.IMPID(IMPID),
		.HARTID(HARTID)
	) csr(
		.i_reset(i_reset),
		.i_clock(i_clock),

		.i_timer_interrupt(i_timer_interrupt),
		.i_external_interrupt(i_external_interrupt),
		.i_ecall(execute_ecall),

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

	wire [31:0] rs1;
	wire [31:0] rs2;
	wire [31:0] rs3;

	CPU_Registers #(
		.STACK_POINTER(STACK_POINTER)
	) registers(
		.i_reset(i_reset),
		.i_clock(i_clock),

		.i_fetch_data(fetch_data),
		.o_rs1(rs1),
		.o_rs2(rs2),
		.o_rs3(rs3),

		.i_memory_data(memory_data)
	);

	//====================================================
	// FETCH

	fetch_data_t fetch_data;
	
	CPU_Fetch #(
		.RESET_VECTOR(RESET_VECTOR)
	) fetch(
		.i_reset(i_reset),
		.i_clock(i_clock),

		// Control
		.i_jump(execute_jump),
		.i_jump_pc(execute_jump_pc),

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
		.i_decode_busy(decode_busy),
		.o_data(fetch_data)
	);

	//====================================================
	// DECODE

	wire decode_fault;
	wire decode_busy;
	decode_data_t decode_data;

	CPU_Decode decode(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.o_fault(decode_fault),

		// Input
		.o_busy(decode_busy),
		.i_data(fetch_data),

		// Output
		.i_execute_busy(execute_busy),
		.o_data(decode_data)
	);

	//====================================================
	// HAZARD

	wire [31:0] forward_rs1;
	wire [31:0] forward_rs2;
	wire [31:0] forward_rs3;
	wire forward_memory_raw;

	CPU_Forward forward(
		.i_decode_data(decode_data),
		.i_execute_data(execute_data),
		.i_memory_data(memory_data),
		.i_writeback_data(writeback_data),

		.i_rs1(rs1),
		.i_rs2(rs2),
		.i_rs3(rs3),

		.o_rs1(forward_rs1),
		.o_rs2(forward_rs2),
		.o_rs3(forward_rs3),

		.o_memory_raw(forward_memory_raw)
	);

	//====================================================
	// EXECUTE

	wire execute_fault;
	wire execute_jump;
	wire [31:0] execute_jump_pc;
	wire execute_ecall;
	wire execute_busy;
	execute_data_t execute_data;
	
	CPU_Execute execute(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.o_fault(execute_fault),

		// CSR
		.o_csr_index(csr_index),
		.i_csr_rdata(csr_rdata),
		.o_csr_wdata_wr(csr_wdata_wr),
		.o_csr_wdata(csr_wdata),
		.i_epc(csr_epc),

		// Control
		.o_jump(execute_jump),
		.o_jump_pc(execute_jump_pc),
		.o_ecall(execute_ecall),

		// Input
		.o_busy(execute_busy),
		.i_memory_raw(forward_memory_raw),
		.i_data(decode_data),
		.i_rs1(forward_rs1),
		.i_rs2(forward_rs2),
		.i_rs3(forward_rs3),
	
		// Output
		.i_memory_busy(memory_busy),
		.o_data(execute_data)
	);

	//====================================================
	// MEMORY

	wire memory_busy;
	memory_data_t memory_data;

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

		// Input
		.o_busy(memory_busy),
		.i_data(execute_data),

		// Output
		.o_data(memory_data)
	);

	//====================================================
	// WRITEBACK

	writeback_data_t writeback_data;
	
	CPU_Writeback writeback(
		.i_reset(i_reset),
		.i_clock(i_clock),

		// Input
		.i_data(memory_data),

		// Output
		.o_data(writeback_data),
		.o_retired()
	);

	//====================================================
	
	assign o_fault = decode_fault || execute_fault;

endmodule
