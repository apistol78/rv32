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
	`define DECODE_DONE state <= STATE_DECODE_FINISH;

	`define MEM_READ_REQ(ADDR) \
		o_address <= ADDR; \
		o_rw <= 0; \
		request <= 1;

	`define MEM_WRITE_REQ(ADDR, DATA) \
		o_address <= ADDR; \
		o_data <= DATA; \
		o_rw <= 1; \
		request <= 1;

	localparam STATE_FETCH_ISSUE		= 3'b001;
	localparam STATE_FETCH_READ     	= 3'b010;
	localparam STATE_DECODE				= 3'b011;
	localparam STATE_DECODE_FINISH	= 3'b100;

	reg [2:0] state;
	reg [2:0] decode_step;
	reg [31:0] pc;
	reg [31:0] pc_next;
	reg [31:0] r[31:0];
	reg [31:0] instruction;
	reg request;

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

	integer i;
	
	initial begin
		state <= STATE_FETCH_ISSUE;
		decode_step <= 0;
		
		pc <= 32'h0000_0000;
		pc_next <= 32'h0000_0000;

      for (i = 0; i < 32; i = i + 1)
        	r[i] <= 0;

		r[2] <= 32'h0001_2000;	// sp

		instruction <= 0;
		o_rw <= 0;
		o_request <= 0;	
	end
	
	// Defer request so address is stable.
	always @ (negedge i_clock)
	begin
		o_request <= request;
	end

	always @ (posedge i_clock)
	begin
		if (state == STATE_FETCH_ISSUE) begin
			// $display("STATE_FETCH_ISSUE, pc = %x", pc);
			`MEM_READ_REQ(pc);
			pc_next <= pc + 4;
			r[0] <= 0;
			state <= STATE_FETCH_READ;
		end
		else if (state == STATE_FETCH_READ) begin
			if (i_ready) begin
				// $display("STATE_FETCH_READ, i_data = %x", i_data);
				instruction <= i_data;
				request <= 0;
				state <= STATE_DECODE;
				decode_step <= 0;
			end
		end
		else if (state == STATE_DECODE) begin
			$display("STATE_DECODE[%d], PC: %x, SP: %x", decode_step, pc, r[2]);
			`include "Instructions_d.v"
		end
		else if (state == STATE_DECODE_FINISH) begin
			$display("STATE_DECODE_FINISH, PC: %x, PC_NEXT: %x", pc, pc_next);
			pc <= pc_next;
			state <= STATE_FETCH_ISSUE;
		end
	end

endmodule
