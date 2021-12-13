module CPU (
	input wire i_reset,
	input wire i_clock,				// CPU clock
	output reg o_rw,				// Data read/write
	output reg o_request,			// IO request.
	input wire i_ready,				// IO request ready.
	output reg [31:0] o_address,	// Address
	input wire [31:0] i_data,		// Read data
	output reg [31:0] o_data		// Write data
);
	`define DECODE_DONE				state <= STATE_DECODE_FINISH;

	localparam STATE_FETCH_ISSUE	= 0;
	localparam STATE_FETCH_READ     = 1;
	localparam STATE_DECODE			= 2;
	localparam STATE_DECODE_FINISH	= 3;

	reg [3:0] state;
	reg [3:0] decode_step;
	reg [31:0] pc;
	reg [31:0] pc_next;
	reg [31:0] r[31:0];
	reg [31:0] instruction;

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

	integer i;

	always @ (posedge i_clock)
	begin
		if (i_reset == 1'b1) begin
			// $display("RESET");
			state <= STATE_FETCH_ISSUE;
			decode_step <= 0;
			pc <= 32'h0200;
			pc_next <= 0;

        	for (i = 0; i < 32; i++)
            	r[i] = 0;
			r[2] <= 32'h00030000 - 4;	// sp

			instruction <= 0;
			o_rw <= 0;
			o_request <= 0;
		end
		else begin
			if (state == STATE_FETCH_ISSUE) begin
				//$display("STATE_FETCH_ISSUE, pc = %x", pc);
				o_address <= pc;
				o_rw <= 0;
				o_request <= 1;
				pc_next <= pc + 4;
				r[0] <= 0;
				state <= STATE_FETCH_READ;
			end
			else if (state == STATE_FETCH_READ) begin
				if (i_ready) begin
					//$display("STATE_FETCH_READ, i_data = %x", i_data);
					instruction <= i_data;
					o_request <= 0;
					state <= STATE_DECODE;
					decode_step <= 0;
				end
			end
			else if (state == STATE_DECODE) begin
				//$display("STATE_DECODE[%d], PC: %x, SP: %x", decode_step, pc, r[2]);
				`include "Instructions.v"
			end
			else if (state == STATE_DECODE_FINISH) begin
				//$display("STATE_DECODE_FINISH, PC: %x, PC_NEXT: %x", pc, pc_next);
				//$display("");
				pc <= pc_next;
				state <= STATE_FETCH_ISSUE;
			end
		end
	end

endmodule
