module CPU_v2 (
	input wire i_reset,
	input wire i_clock,				// CPU clock
	output reg o_rw,				// Data read/write
	output wire o_request,			// IO request.
	input wire i_ready,				// IO request ready.
	output wire [31:0] o_address,	// Address
	input wire [31:0] i_data,		// Read data
	output reg [31:0] o_data		// Write data,
);

    // Register files,
    // RS1 and RS2 are read from file
    // simultaneously as decode stage.

    wire [31:0] rs1;
    wire [31:0] rs2;

	wire [4:0] fetch_inst_rs1 = fetch_instruction[19:15];
	wire [4:0] fetch_inst_rs2 = fetch_instruction[24:20];

    CPU_Registers registers(
        .i_reset(i_reset),
        .i_clock(i_clock),

        .i_read_rs1_idx(fetch_inst_rs1),
        .i_read_rs2_idx(fetch_inst_rs2),
        .i_read(fetch_ready),
        .o_rs1(rs1),
        .o_rs2(rs2),

        .i_rd(writeback_rd),
        .i_write_rd_idx(writeback_inst_rd),
        .i_write(writeback_ready)
    );


    //====================================================
    // FETCH

    wire [31:0] fetch_instruction;
    wire [31:0] fetch_pc;
    wire fetch_ready;

    CPU_Fetch fetch(
        .i_reset(i_reset),
        .i_clock(i_clock),

        // Memory
        .o_request(o_request),
        .i_ready(i_ready),
        .o_address(o_address),
        .i_data(i_data),

        // Input
         .i_branch(writeback_branch),
        .i_pc_next(writeback_pc_next),

        // Output
        .o_instruction(fetch_instruction),
        .o_pc(fetch_pc),
        .o_ready(fetch_ready)
    );


    //====================================================
    // DECODE

    wire [31:0] decode_instruction;
    wire [31:0] decode_pc;
    wire [4:0] decode_inst_rs1;
    wire [4:0] decode_inst_rs2;
    wire [4:0] decode_inst_rd;
    wire [31:0] decode_imm;
    wire decode_branch;
    wire decode_ready;

    CPU_Decode decode(
        .i_reset(i_reset),
        .i_clock(i_clock),
        .i_execute(fetch_ready),

        .i_instruction(fetch_instruction),
        .i_pc(fetch_pc),

        .o_instruction(decode_instruction),
        .o_pc(decode_pc),
        .o_inst_rs1(decode_inst_rs1),
        .o_inst_rs2(decode_inst_rs2),
        .o_inst_rd(decode_inst_rd),
        .o_imm(decode_imm),
        .o_branch(decode_branch),
        .o_ready(decode_ready)
    );


    //====================================================
    // EXECUTE

    // Select register from file or pipeline due to register hazard.
    wire [31:0] resolved_rs1 = (decode_inst_rs1 == memory_inst_rd) ? memory_rd : rs1;
    wire [31:0] resolved_rs2 = (decode_inst_rs2 == memory_inst_rd) ? memory_rd : rs2;

    wire [4:0] execute_inst_rd;
    wire [31:0] execute_rd;
    wire execute_branch;
    wire [31:0] execute_pc_next;
    wire execute_ready;

    CPU_Execute execute(
        .i_reset(i_reset),
        .i_clock(i_clock),
        .i_execute(decode_ready),

        // Input from decode.
        .i_pc(decode_pc),
        .i_instruction(decode_instruction),
        .i_rs1(resolved_rs1),
        .i_rs2(resolved_rs2),
        .i_inst_rd(decode_inst_rd),
        .i_imm(decode_imm),
        .i_branch(decode_branch),

        // Output from execute.
        .o_inst_rd(execute_inst_rd),
        .o_rd(execute_rd),
        .o_branch(execute_branch),
        .o_pc_next(execute_pc_next),
        .o_ready(execute_ready)
    );



    //====================================================
    // MEMORY

    wire [4:0] memory_inst_rd;
    wire [31:0] memory_rd;
    wire memory_branch;
    wire [31:0] memory_pc_next;
    wire memory_ready;

    CPU_Memory memory(
        .i_reset(i_reset),
        .i_clock(i_clock),
        .i_execute(execute_ready),

        // Input from execute.
        .i_inst_rd(execute_inst_rd),
        .i_rd(execute_rd),
        .i_branch(execute_branch),
        .i_pc_next(execute_pc_next),

        // Output from memory.
        .o_inst_rd(memory_inst_rd),
        .o_rd(memory_rd),
        .o_branch(memory_branch),
        .o_pc_next(memory_pc_next),
        .o_ready(memory_ready)
    );


    //====================================================
    // WRITEBACK

    wire [4:0] writeback_inst_rd;
    wire [31:0] writeback_rd;
    wire writeback_branch;
    wire [31:0] writeback_pc_next;
    wire writeback_ready;

    CPU_Writeback writeback(
        .i_reset(i_reset),
        .i_clock(i_clock),
        .i_execute(memory_ready),

        // Input from memory.
        .i_instruction(memory_instruction),
        .i_inst_rd(memory_inst_rd),
        .i_rd(memory_rd),
        .i_branch(memory_branch),
        .i_pc_next(memory_pc_next),

        // Output from writeback.
        .o_inst_rd(writeback_inst_rd),
        .o_rd(writeback_rd),
        .o_branch(writeback_branch),
        .o_pc_next(writeback_pc_next),
        .o_ready(writeback_ready)
    );


endmodule
