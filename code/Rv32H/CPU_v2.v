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

    wire [31:0] pc;


    wire [31:0] rs1;
    wire [31:0] rs2;

	wire [4:0] fetch_inst_rs1 = fetch_instruction[19:15];
	wire [4:0] fetch_inst_rs2 = fetch_instruction[24:20];

    CPU_Registers registers(
        .i_reset(i_reset),
        .i_clock(i_clock),

        .i_read_rs1_idx(fetch_inst_rs1),
        .i_read_rs2_idx(fetch_inst_rs2),
        .i_read(fetch_fetched),

        .o_rs1(rs1),
        .o_rs2(rs2)
    );

    // FETCH

    wire [31:0] fetch_instruction;
    wire fetch_fetched;

    CPU_Fetch fetch(
        .i_reset(i_reset),
        .i_clock(i_clock),

        .o_pc(pc),

        .o_request(o_request),
        .i_ready(i_ready),
        .o_address(o_address),
        .i_data(i_data),

        .o_instruction(fetch_instruction),
        .o_fetched(fetch_fetched)
    );

    // DECODE

    wire [31:0] decode_instruction;
    wire [4:0] decode_inst_rs1;
    wire [4:0] decode_inst_rs2;
    wire [4:0] decode_inst_rd;
    wire decode_decoded;

    CPU_Decode decode(
        .i_reset(i_reset),
        .i_clock(i_clock),

        .i_decode(fetch_fetched),
        .i_instruction(fetch_instruction),

        .o_instruction(decode_instruction),
        .o_inst_rs1(decode_inst_rs1),
        .o_inst_rs2(decode_inst_rs2),
        .o_inst_rd(decode_inst_rd),
        .o_decoded(decode_decoded)
    );

    // EXECUTE

    CPU_Execute execute(
        .i_reset(i_reset),
        .i_clock(i_clock),

        .i_execute(decode_decoded),

        .i_instruction(decode_instruction),
        .i_rs1(rs1),
        .i_rs2(rs2)
    );

endmodule
