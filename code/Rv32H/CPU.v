module CPU (
	input wire i_reset,
	input wire i_clock,			// CPU clock
	output reg o_rw,			// Data read/write
	output reg o_req,			// IO request.
	input wire i_ready,			// IO request ready.
	output reg [31:0] o_addr,	// Address
	input wire [31:0] i_data,	// Read data
	output reg [31:0] o_data	// Write data
);
	`define DISPLAY_OP(op) $display(``op``)

	localparam STATE_FETCH_ISSUE	= 0;
	localparam STATE_FETCH_READ     = 1;
	localparam STATE_DECODE			= 2;

	reg [3:0] state;
	reg [31:0] pc;
	reg [31:0] r[31:0];

	reg [31:0] instruction;

	// B format
	assign inst_B_rs1 = instruction[19:15];
	assign inst_B_rs1 = instruction[24:20];
	assign inst_B_imm = { instruction[11:8] };

	// R format
	assign inst_R_rd = { instruction[11:7] };
	assign inst_R_rs1 = instruction[19:15];
	assign inst_R_rs1 = instruction[24:20];

	always @ (posedge i_clock)
	begin
		if (i_reset == 1'b1) begin
			state <= STATE_FETCH_ISSUE;
			pc <= 0;
			r[0] <= 0;
		end
		else begin

			if (state == STATE_FETCH_ISSUE) begin
				o_addr <= pc;
				o_rw <= 0;
				o_req <= 1;
				state <= STATE_FETCH_READ;
			end

			else if (state == STATE_FETCH_READ) begin
				if (i_ready) begin
					instruction <= i_data;
					o_req <= 0;
					state <= STATE_DECODE;
				end
			end

			else if (state == STATE_DECODE) begin
				pc <= pc + 4;
				state <= STATE_FETCH_ISSUE;
				`include "Instructions.v"
			end

		end
	end

endmodule
