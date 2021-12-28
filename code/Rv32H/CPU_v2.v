module CPU_v2 (
	input wire i_reset,
	input wire i_clock,				// CPU clock

	// Bus
	output wire o_bus_rw,				// Data read/write
	output wire o_bus_request,			// IO request.
	input wire i_bus_ready,				// IO request ready.
	output wire [31:0] o_bus_address,	// Address
	input wire [31:0] i_bus_rdata,		// Read data
	output wire [31:0] o_bus_wdata		// Write data,
);

	//====================================================
	// BUS ACCESS

	wire bus_pa_request;
	wire bus_pa_ready;
	wire [31:0] bus_pa_address;
	wire [31:0] bus_pa_rdata;

	wire bus_pb_rw;
	wire bus_pb_request;
	wire bus_pb_ready;
	wire [31:0] bus_pb_address;
	wire [31:0] bus_pb_rdata;
	wire [31:0] bus_pb_wdata;

	CPU_BusAccess bus(
		.i_reset(i_reset),
		.i_clock(i_clock),

		// Bus
		.o_bus_rw(o_bus_rw),
		.o_bus_request(o_bus_request),
		.i_bus_ready(i_bus_ready),
		.o_bus_address(o_bus_address),
		.i_bus_rdata(i_bus_rdata),
		.o_bus_wdata(o_bus_wdata),

		// Port A (FETCH)
		.i_pa_rw(1'b0),
		.i_pa_request(bus_pa_request),
		.o_pa_ready(bus_pa_ready),
		.i_pa_address(bus_pa_address),
		.o_pa_rdata(bus_pa_rdata),
		//.i_pa_wdata(bus_pa_wdata),

		// Port B (MEMORY)
		.i_pb_rw(bus_pb_rw),
		.i_pb_request(bus_pb_request),
		.o_pb_ready(bus_pb_ready),
		.i_pb_address(bus_pb_address),
		.o_pb_rdata(bus_pb_rdata),
		.i_pb_wdata(bus_pb_wdata)
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

		.i_write_tag(writeback_tag),
		.i_write_rd_idx(writeback_inst_rd),
		.i_rd(writeback_rd)
	);

	//====================================================
	// CONTROL

	wire stall = memory_stall;

	//====================================================
	// FETCH

	wire [7:0] fetch_tag;
	wire [31:0] fetch_instruction;
	wire [31:0] fetch_pc;
	
	CPU_Fetch fetch(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_stall(stall),

		// Bus
		.o_bus_request(bus_pa_request),
		.i_bus_ready(bus_pa_ready),
		.o_bus_address(bus_pa_address),
		.i_bus_rdata(bus_pa_rdata),

		// Input
		.i_branch(writeback_branch),
		.i_pc_next(writeback_pc_next),

		// Output
		.o_tag(fetch_tag),
		.o_instruction(fetch_instruction),
		.o_pc(fetch_pc)
	);

	//====================================================
	// DECODE

	wire [7:0] decode_tag;
	wire [31:0] decode_instruction;
	wire [31:0] decode_pc;
	wire [4:0] decode_inst_rs1;
	wire [4:0] decode_inst_rs2;
	wire [4:0] decode_inst_rd;
	wire [31:0] decode_imm;
	wire decode_branch;
	
	CPU_Decode decode(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_stall(stall),
	
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
		.o_branch(decode_branch)
	);

	//====================================================
	// EXECUTE

	// Forward register values from pipeline if already in flight.
	wire [31:0] fwd_rs1 = 
		(decode_inst_rs1 == 0) ? 32'h0 :
		(decode_inst_rs1 == execute_inst_rd) ? execute_rd :
		(decode_inst_rs1 == memory_inst_rd) ? memory_rd :
		(decode_inst_rs1 == writeback_inst_rd) ? writeback_rd :
		rs1;

	wire [31:0] fwd_rs2 =
		(decode_inst_rs2 == 0) ? 32'h0 :
		(decode_inst_rs2 == execute_inst_rd) ? execute_rd :
		(decode_inst_rs2 == memory_inst_rd) ? memory_rd :
		(decode_inst_rs2 == writeback_inst_rd) ? writeback_rd :
		rs2;

	wire [7:0] execute_tag;
	wire [4:0] execute_inst_rd;
	wire [31:0] execute_rd;
	wire execute_branch;
	wire [31:0] execute_pc_next;
	wire execute_mem_read;
	wire execute_mem_write;
	wire [31:0] execute_mem_address;
	
	CPU_Execute execute(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_stall(stall),

		// Input from decode.
		.i_tag(decode_tag),
		.i_pc(decode_pc),
		.i_instruction(decode_instruction),
		.i_rs1(fwd_rs1),
		.i_rs2(fwd_rs2),
		.i_inst_rd(decode_inst_rd),
		.i_imm(decode_imm),
		.i_branch(decode_branch),

		// Output from execute.
		.o_tag(execute_tag),
		.o_inst_rd(execute_inst_rd),
		.o_rd(execute_rd),
		.o_branch(execute_branch),
		.o_pc_next(execute_pc_next),
		.o_mem_read(execute_mem_read),
		.o_mem_write(execute_mem_write),
		.o_mem_address(execute_mem_address)
	);

	//====================================================
	// MEMORY

	wire [7:0] memory_tag;
	wire [4:0] memory_inst_rd;
	wire [31:0] memory_rd;
	wire memory_branch;
	wire [31:0] memory_pc_next;
	wire memory_stall;

	CPU_Memory memory(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_stall(stall),

		// Bus
		.o_bus_rw(bus_pb_rw),
		.o_bus_request(bus_pb_request),
		.i_bus_ready(bus_pb_ready),
		.o_bus_address(bus_pb_address),
		.i_bus_rdata(bus_pb_rdata),
		.o_bus_wdata(bus_pb_wdata),

		// Input from execute.
		.i_tag(execute_tag),
		.i_inst_rd(execute_inst_rd),
		.i_rd(execute_rd),
		.i_branch(execute_branch),
		.i_pc_next(execute_pc_next),
		.i_mem_read(execute_mem_read),
		.i_mem_write(execute_mem_write),
		.i_mem_address(execute_mem_address),

		// Output from memory.
		.o_tag(memory_tag),
		.o_inst_rd(memory_inst_rd),
		.o_rd(memory_rd),
		.o_branch(memory_branch),
		.o_pc_next(memory_pc_next),
		.o_stall(memory_stall)
	);


	//====================================================
	// WRITEBACK

	wire [7:0] writeback_tag;
	wire [4:0] writeback_inst_rd;
	wire [31:0] writeback_rd;
	wire writeback_branch;
	wire [31:0] writeback_pc_next;
	
	CPU_Writeback writeback(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_stall(stall),
	
		// Input from memory.
		.i_tag(memory_tag),
		.i_inst_rd(memory_inst_rd),
		.i_rd(memory_rd),
		.i_branch(memory_branch),
		.i_pc_next(memory_pc_next),

		// Output from writeback.
		.o_tag(writeback_tag),
		.o_inst_rd(writeback_inst_rd),
		.o_rd(writeback_rd),
		.o_branch(writeback_branch),
		.o_pc_next(writeback_pc_next)
	);


endmodule
