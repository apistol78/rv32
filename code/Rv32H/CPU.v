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
	`define DECODE_DONE state <= STATE_RETIRE;
	
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

	localparam STATE_FETCH_ISSUE	= 3'b001;
	localparam STATE_FETCH_READ     = 3'b010;
	localparam STATE_DECODE			= 3'b011;
	localparam STATE_RETIRE			= 3'b100;

	reg [2:0] state;
	reg [2:0] decode_step;
	reg [31:0] pc;
	reg [31:0] pc_next;
	reg [31:0] r[31:0];
	reg [31:0] instruction;

	assign o_pc = { 29'b0, state };
	
	wire [1:0] address_byte = o_address[1:0];

	// https://en.wikipedia.org/wiki/RISC-V#ISA_base_and_extensions

	// B format
	wire [4:0] inst_B_rs1 = instruction[19:15];
	wire [4:0] inst_B_rs2 = instruction[24:20];
	wire [31:0] inst_B_imm = { { 20{ instruction[31] } }, instruction[7], instruction[30:25], instruction[11:8], 1'b0 };

	// I format
	wire [4:0] inst_I_rd = instruction[11:7];
	wire [4:0] inst_I_rs1 = instruction[19:15];
	wire [31:0] inst_I_imm = { { 21{ instruction[31] } }, instruction[30:20] };

	// J format
	wire [4:0] inst_J_rd = instruction[11:7];
	wire [31:0] inst_J_imm = { { 12{ instruction[31] } }, instruction[19:12], instruction[20], instruction[30:21], 1'b0 };

	// R format
	wire [4:0] inst_R_rd = instruction[11:7];
	wire [4:0] inst_R_rs1 = instruction[19:15];
	wire [4:0] inst_R_rs2 = instruction[24:20];

	// S format
	wire [4:0] inst_S_rs1 = instruction[19:15];
	wire [4:0] inst_S_rs2 = instruction[24:20];
	wire [31:0] inst_S_imm = { { 21{ instruction[31] } }, instruction[30:25], instruction[11:7] };

	// U format
	wire [4:0] inst_U_rd = instruction[11:7];
	wire [31:0] inst_U_imm = { instruction[31:12], 12'b0 };

	`include "Instructions_i.v"

	//integer i;
	
	initial begin
		state <= STATE_FETCH_ISSUE;
		decode_step <= 0;
		pc <= 32'h0000_0000;
		pc_next <= 32'h0000_0000;
		//for (i = 0; i < 32; i = i + 1)
		//	r[i] <= 0;
		r[2] <= 32'h0001_0400;	// sp
		instruction <= 0;
		o_rw <= 0;		
		o_request <= 0;
		o_address <= 0;
		o_data <= 0;
	end
	
	always @ (posedge i_clock, posedge i_reset)
	begin
		if (i_reset) begin
			state <= STATE_FETCH_ISSUE;
			decode_step <= 0;
			pc <= 32'h0000_0000;
			pc_next <= 32'h0000_0000;
			//for (i = 0; i < 32; i = i + 1)
			//	r[i] <= 0;
			r[2] <= 32'h0001_0400;	// sp
			instruction <= 0;
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
				end
				
				STATE_FETCH_READ: begin
					if (`BUS_READY) begin
						instruction <= i_data;
						o_request <= 0;
						state <= STATE_DECODE;
						decode_step <= 0;
						pc_next <= pc + 4;
						r[0] <= 0;
					end
				end

				STATE_DECODE: begin
					// $display("STATE_DECODE[%d], PC: %x, SP: %x", decode_step, pc, r[2]);
					`include "Instructions_d.v"
				end

				STATE_RETIRE: begin
					// $display("STATE_RETIRE, PC: %x, PC_NEXT: %x", pc, pc_next);
					`MEM_READ_REQ(pc_next);
					pc <= pc_next;
					state <= STATE_FETCH_READ;					
				end
			endcase
		end
	end

endmodule
