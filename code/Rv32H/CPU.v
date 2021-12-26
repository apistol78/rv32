module CPU (
	input wire i_reset,
	input wire i_clock,				// CPU clock
	output reg o_rw,				// Data read/write
	output reg o_request,			// IO request.
	input wire i_ready,				// IO request ready.
	output reg [31:0] o_address,	// Address
	input wire [31:0] i_data,		// Read data
	output reg [31:0] o_data,		// Write data,
	output wire [31:0] o_pc
);
	`define DECODE_DONE \
		state <= STATE_RETIRE;
	
	`define GOTO(ADDR) \
		pc_next <= ADDR;

	`define MEM_READ_REQ(ADDR) \
		o_address <= ADDR; \
		o_rw <= 0; \
		o_request <= 1;

	`define MEM_WRITE_REQ(ADDR, DATA) \
		o_address <= ADDR; \
		o_data <= DATA; \
		o_rw <= 1; \
		o_request <= 1;

	`define BUS_READ \
		o_rw <= 0;
	`define BUS_WRITE \
		o_rw <= 1;
	`define BUS_REQUEST_START \
		o_request <= 1;
	`define BUS_REQUEST_END \
		o_request <= 0;
	`define BUS_READY \
		i_ready
		
	`define ERROR
		//state <= STATE_ERROR;

	localparam STATE_FETCH_ISSUE	= 3'd0;
	localparam STATE_FETCH_READ     = 3'd1;
	localparam STATE_DECODE			= 3'd2;
	localparam STATE_RETIRE			= 3'd3;
	localparam STATE_ERROR			= 3'd4;

	reg [2:0] state;
	reg [2:0] decode_step;
	reg [31:0] pc;
	reg [31:0] pc_next;
	reg [31:0] instruction;

	/*
	// mulhu
	reg [31:0] m64_al;
	reg [31:0] m64_ah;
	reg [31:0] m64_bl;
	reg [31:0] m64_bh;
	reg [31:0] m64_p0;
	reg [31:0] m64_p1;
	reg [31:0] m64_p2;
	reg [31:0] m64_p3;
	*/
	
	// Common format
	wire [4:0] inst_rs1 = instruction[19:15];
	wire [4:0] inst_rs2 = instruction[24:20];
	wire [4:0] inst_rd  = instruction[11:7];

	// Instruction formats
	wire [31:0] inst_B_imm = { { 20{ instruction[31] } }, instruction[7], instruction[30:25], instruction[11:8], 1'b0 };
	wire [31:0] inst_I_imm = { { 21{ instruction[31] } }, instruction[30:20] };
	wire [31:0] inst_J_imm = { { 12{ instruction[31] } }, instruction[19:12], instruction[20], instruction[30:21], 1'b0 };
	wire [31:0] inst_S_imm = { { 21{ instruction[31] } }, instruction[30:25], instruction[11:7] };
	wire [31:0] inst_U_imm = { instruction[31:12], 12'b0 };

	`include "Instructions_i.v"

	wire need_rs1 = is_B | is_I | is_R | is_S;
	wire need_rs2 = is_B | is_R | is_S;
	wire need_rd = is_I | is_J | is_R | is_U;

	wire [31:0] rs1;
	wire [31:0] rs2;
	reg [31:0] rd;
	reg registers_wr_request;
	Registers registers(
		.i_reset(i_reset),
		.i_clock(i_clock),
		.i_rs1_idx(inst_rs1),
		.i_rs2_idx(inst_rs2),
		.i_rd_idx(inst_rd),
		.i_need_rs1(need_rs1),
		.i_need_rs2(need_rs2),
		.i_need_rd(need_rd),
		.o_rs1(rs1),
		.o_rs2(rs2),
		.i_rd(rd),
		.i_wr_request(registers_wr_request)
	);

	assign o_pc = { 29'b0, state };
	
	wire [1:0] address_byte = o_address[1:0];

	// https://en.wikipedia.org/wiki/RISC-V#ISA_base_and_extensions
	
	/*
	initial begin
		state <= STATE_FETCH_ISSUE;
		decode_step <= 0;
		pc <= 32'h0000_0000;
		pc_next <= 32'h0000_0000;
		instruction <= 32'h0000_0000;
		registers_wr_request <= 0;
		o_rw <= 0;		
		o_request <= 0;
		o_address <= 0;
		o_data <= 0;
	end
	*/

	always @ (posedge i_clock, posedge i_reset)
	begin
		if (i_reset) begin
			state <= STATE_FETCH_ISSUE;
			decode_step <= 0;
			pc <= 32'h0000_0000;
			pc_next <= 32'h0000_0000;
			instruction <= 32'h0000_0000;
			registers_wr_request <= 0;
			o_rw <= 0;		
			o_request <= 0;
			o_address <= 0;
			o_data <= 0;
		end
		else begin
			case (state)
				STATE_FETCH_ISSUE: begin
					`MEM_READ_REQ(pc);
					state <= STATE_FETCH_READ;
					registers_wr_request <= 0;
				end
				
				STATE_FETCH_READ: begin
					if (`BUS_READY) begin
						instruction <= i_data;
						o_request <= 0;
						state <= STATE_DECODE;
						decode_step <= 0;
						pc_next <= pc + 4;
					end
				end

				STATE_DECODE: begin
					$display("STATE_DECODE[%d], PC: %x", decode_step, pc);
					`include "Instructions_d.v"
				end

				STATE_RETIRE: begin
					$display("STATE_RETIRE, PC: %x, PC_NEXT: %x", pc, pc_next);
					pc <= pc_next;
					state <= STATE_FETCH_ISSUE;
					registers_wr_request <= 1;
				end
			endcase
		end
	end

endmodule
