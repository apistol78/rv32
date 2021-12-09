/*

	Simple 32-bit CPU

	16 registers, some with special purpose:
		R14 - stack pointer
		R15 - program counter

*/
module ap_cpu(
	input wire iCLK,			// CPU clock
	output reg oRW,				// Data read/write
	output reg oREQ,			// IO request.
	input wire iRDY,			// IO request ready.
	output reg [31:0] oADDR,	// Address
	input wire [31:0] iDATA,	// Read data
	output reg [31:0] oDATA		// Write data
);

	`define DISPLAY_OP(op) $display(``op``)
	`define DISPLAY_OP_1(op, arg) $display(``op``, ``arg``)
	`define DISPLAY_OP_2(op, arg1, arg2) $display(``op``, ``arg1``, ``arg2``)

	// States
	localparam STATE_FETCH_A_ISSUE		= 0;
	localparam STATE_FETCH_A_COMPLETE	= 1;
	localparam STATE_FETCH_B_ISSUE		= 2;
	localparam STATE_FETCH_B_COMPLETE	= 3;
	localparam STATE_DECODE				= 4;
	localparam STATE_LOAD_ISSUE			= 5;
	localparam STATE_LOAD_COMPLETE		= 6;
	localparam STATE_STORE_ISSUE		= 7;
	localparam STATE_STORE_COMPLETE		= 8;

	// Opcodes
	localparam OP_NOP     = 8'h00;
	localparam OP_LOAD    = 8'h01;
	localparam OP_LOAD_i  = 8'h81;
	localparam OP_STORE   = 8'h02;
	localparam OP_STORE_i = 8'h82;
	localparam OP_MOVE    = 8'h03;
	localparam OP_MOVE_i  = 8'h83;
	localparam OP_INC     = 8'h04;
	localparam OP_DEC     = 8'h05;
	localparam OP_ADD     = 8'h06;
	localparam OP_ADD_i   = 8'h86;
	localparam OP_SUB     = 8'h07;
	localparam OP_SUB_i   = 8'h87;
	localparam OP_MUL     = 8'h08;
	localparam OP_MUL_i   = 8'h88;
	localparam OP_DIV     = 8'h09;
	localparam OP_DIV_i   = 8'h89;
	localparam OP_SHL     = 8'h0a;
	localparam OP_SHL_i   = 8'h8a;
	localparam OP_SHR     = 8'h0b;
	localparam OP_SHR_i   = 8'h8b;
	localparam OP_NOT     = 8'h0c;
	localparam OP_AND     = 8'h0d;
	localparam OP_AND_i   = 8'h8d;
	localparam OP_OR      = 8'h0e;
	localparam OP_OR_i    = 8'h8e;
	localparam OP_XOR     = 8'h0f;
	localparam OP_XOR_i   = 8'h8f;
	localparam OP_CMPEQ   = 8'h10;
	localparam OP_CMPEQ_i = 8'h90;
	localparam OP_CMPNE   = 8'h11;
	localparam OP_CMPNE_i = 8'h91;
	localparam OP_CMPLT   = 8'h12;
	localparam OP_CMPLT_i = 8'h92;
	localparam OP_CMPGT   = 8'h13;
	localparam OP_CMPGT_i = 8'h93;
	localparam OP_CMPLE   = 8'h14;
	localparam OP_CMPLE_i = 8'h94;
	localparam OP_CMPGE   = 8'h15;
	localparam OP_CMPGE_i = 8'h95;
	localparam OP_JMP     = 8'h96;
	localparam OP_JMPZ    = 8'h97;
	localparam OP_JMPNZ   = 8'h98;
	localparam OP_PUSH    = 8'h19;
	localparam OP_PUSH_i  = 8'h99;
	localparam OP_POP     = 8'h1a;
	localparam OP_CALL    = 8'h9b;
	localparam OP_RET     = 8'h1c;
		
	// Internal
	reg [3:0] state = STATE_FETCH_A_ISSUE;
	reg [31:0] r[15:0];
	reg request = 0;
	reg [3:0] index = 0;

	// |6|6|6|6|5|5|5|5|5|5|5|5|5|5|4|4|4|4|4|4|4|4|4|4|3|3|3|3|3|3|3|3|3|3|2|2|2|2|2|2|2|2|2|2|1|1|1|1|1|1|1|1|1|1| | | | | | | | | | |
	// |3|2|1|0|9|8|7|6|5|4|3|2|1|0|9|8|7|6|5|4|3|2|1|0|9|8|7|6|5|4|3|2|1|0|9|8|7|6|5|4|3|2|1|0|9|8|7|6|5|4|3|2|1|0|9|8|7|6|5|4|3|2|1|0|
	// |      op       |  ra   |  rb   |                               |                            data                               |

	reg [63:0] inst;
	wire [7:0] inst_op;
	wire [3:0] inst_regA;
	wire [3:0] inst_regB;
	wire [31:0] inst_data;

	assign inst_op = inst[63:56];
	assign inst_regA = inst[55:52];
	assign inst_regB = inst[51:48];
	assign inst_data = inst[31:0];

	integer i;
	initial begin
		oRW = 1'b0;
		oREQ = 1'b0;
		oADDR = 32'h0;
		oDATA = 32'h0;

		// Initialize all registers to zero.
		for (i = 0; i < 16; i = i + 1) begin
			r[i] = 32'h0;
		end
	end

	// Defer oREQ to negative edge since we want
	// to make sure oADDR/oDATA is latched before
	// signaling the request.
	always @ (negedge iCLK) begin
		oREQ <= request;
	end

	always @ (posedge iCLK) begin

		// Issue fetch instruction, opcode.
		if (state == STATE_FETCH_A_ISSUE) begin
			//$display("STATE_FETCH_A_ISSUE");
			oADDR <= r[15];
			oRW <= 1'b0;
			state <= STATE_FETCH_A_COMPLETE;
			request <= 1'b1;
		end

		// Complete fetch instruction, opcode.
		else if (state == STATE_FETCH_A_COMPLETE) begin
			//$display("STATE_FETCH_A_COMPLETE");
			if (iRDY) begin
				inst[63:32] <= iDATA;
				request <= 1'b0;
				r[15] <= r[15] + 32'h1;
				if (iDATA[31]) begin
					// Long instruction, need to fetch data.
					state <= STATE_FETCH_B_ISSUE;
				end else begin
					// Short instruction, no data associated.
					state <= STATE_DECODE;
				end
			end
		end

		// Issue fetch instruction, data high word.
		else if (state == STATE_FETCH_B_ISSUE) begin
			//$display("STATE_FETCH_B_ISSUE");
			oADDR <= r[15];
			oRW <= 1'b0;
			state <= STATE_FETCH_B_COMPLETE;
			request <= 1'b1;
		end

		// Complete fetch instruction, data high word.
		else if (state == STATE_FETCH_B_COMPLETE) begin
			//$display("STATE_FETCH_B_COMPLETE");
			if (iRDY) begin
				inst[31:0] <= iDATA;
				request <= 1'b0;
				r[15] <= r[15] + 32'h1;
				state <= STATE_DECODE;
			end
		end

		// Decode instruction.
		else if (state == STATE_DECODE) begin
//`ifdef CPU_DISPLAY_REGISTERS
			$display("\n=========");
			$display(" %h", inst);
			$display(" r0=%h,  r1=%h,  r2=%h,  r3=%h",  r[0],  r[1],  r[2],  r[3]);
			$display(" r4=%h,  r5=%h,  r6=%h,  r7=%h",  r[4],  r[5],  r[6],  r[7]);
			$display(" r8=%h,  r9=%h, r10=%h, r11=%h",  r[8],  r[9], r[10], r[11]);
			$display("r12=%h, r13=%h, r14=%h, r15=%h", r[12], r[13], r[14], r[15]);
//`endif
			case (inst_op)
				OP_NOP:
					begin
						`DISPLAY_OP("nop");
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_LOAD:
					begin
						`DISPLAY_OP_2("load, r%d=[r%d]", inst_regA, inst_regB);
						oRW <= 0;
						oADDR <= inst_regB;
						index <= inst_regA;
						state <= STATE_LOAD_COMPLETE;
						request <= 1'b1;
					end

				OP_LOAD_i:
					begin
						`DISPLAY_OP_2("load, r%d=[0x%h]", inst_regA, inst_data);
						oRW <= 0;
						oADDR <= inst_data;
						index <= inst_regA;
						state <= STATE_LOAD_COMPLETE;
						request <= 1'b1;
					end

				OP_STORE:
					begin
						`DISPLAY_OP_2("store [r%d]=0x%h", inst_regA, r[inst_regB]); //, address=%h, value=%h", inst_data, r[0]));
						oRW <= 1;
						oADDR <= inst_regA;
						oDATA <= r[inst_regB];
						state <= STATE_STORE_COMPLETE;
						request <= 1'b1;
					end

				OP_STORE_i:
					begin
						`DISPLAY_OP_2("store [0x%h]=0x%h", inst_data, r[inst_regA]); //, address=%h, value=%h", inst_data, r[0]));
						oRW <= 1;
						oADDR <= inst_data;
						oDATA <= r[inst_regA];
						state <= STATE_STORE_COMPLETE;
						request <= 1'b1;
					end

				OP_MOVE:
					begin
						`DISPLAY_OP("move");
						r[inst_regA] <= r[inst_regB];
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_MOVE_i:
					begin
						`DISPLAY_OP("move_i");
						r[inst_regA] <= inst_data;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_INC:
					begin
						`DISPLAY_OP("inc");
						r[inst_regA] <= r[inst_regA] + 32'h1;
						state <= STATE_FETCH_A_ISSUE;
					end
				
				OP_DEC:
					begin
						`DISPLAY_OP("dec");
						r[inst_regA] <= r[inst_regA] - 32'h1;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_ADD:
					begin
						`DISPLAY_OP("add");
						r[inst_regA] <= r[inst_regA] + r[inst_regB];
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_ADD_i:
					begin
						`DISPLAY_OP("add_i");
						r[inst_regA] <= r[inst_regA] + inst_data;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_SUB:
					begin
						`DISPLAY_OP("sub");
						r[inst_regA] <= r[inst_regA] - r[inst_regB];
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_SUB_i:
					begin
						`DISPLAY_OP("sub_i");
						r[inst_regA] <= r[inst_regA] - inst_data;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_MUL:
					begin
						`DISPLAY_OP("mul");
						r[inst_regA] <= r[inst_regA] * r[inst_regB];
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_MUL_i:
					begin
						`DISPLAY_OP("mul_i");
						r[inst_regA] <= r[inst_regA] * inst_data;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_DIV:
					begin
						`DISPLAY_OP("div");
						r[inst_regA] <= r[inst_regA] / r[inst_regB];
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_DIV_i:
					begin
						`DISPLAY_OP("div_i");
						r[inst_regA] <= r[inst_regA] / inst_data;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_SHL:
					begin
						`DISPLAY_OP("shl");
						r[inst_regA] <= r[inst_regA] << r[inst_regB];
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_SHL_i:
					begin
						`DISPLAY_OP("shl_i");
						r[inst_regA] <= r[inst_regA] << inst_data;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_SHR:
					begin
						`DISPLAY_OP("shr");
						r[inst_regA] <= r[inst_regA] >> r[inst_regB];
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_SHR_i:
					begin
						`DISPLAY_OP("shr_i");
						r[inst_regA] <= r[inst_regA] >> inst_data;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_NOT:
					begin
						`DISPLAY_OP("not");
						r[inst_regA] <= ~r[inst_regA];
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_AND:
					begin
						`DISPLAY_OP_2("and, r%h &= r%h", inst_regA, inst_regB);
						r[inst_regA] <= r[inst_regA] & r[inst_regB];
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_AND_i:
					begin
						`DISPLAY_OP("and_i");
						r[inst_regA] <= r[inst_regA] & inst_data;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_OR:
					begin
						`DISPLAY_OP("or");
						r[inst_regA] <= r[inst_regA] | r[inst_regB];
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_OR_i:
					begin
						`DISPLAY_OP("or_i");
						r[inst_regA] <= r[inst_regA] | inst_data;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_XOR:
					begin
						`DISPLAY_OP("xor");
						r[inst_regA] <= r[inst_regA] ^ r[inst_regB];
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_XOR_i:
					begin
						`DISPLAY_OP("xor_i");
						r[inst_regA] <= r[inst_regA] ^ inst_data;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_CMPEQ:
					begin
						`DISPLAY_OP("cmpeq");
						r[0] <= (r[inst_regA] == r[inst_regB]) ? 32'hffffffff : 32'h0;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_CMPEQ_i:
					begin
						`DISPLAY_OP("cmpeq_i");
						r[0] <= (r[inst_regA] == inst_data) ? 32'hffffffff : 32'h0;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_CMPNE:
					begin
						`DISPLAY_OP("cmpne");
						r[0] <= (r[inst_regA] != r[inst_regB]) ? 32'hffffffff : 32'h0;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_CMPNE_i:
					begin
						`DISPLAY_OP("cmpne_i");
						r[0] <= (r[inst_regA] != inst_data) ? 32'hffffffff : 32'h0;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_CMPLT:
					begin
						`DISPLAY_OP("cmplt");
						r[0] <= (r[inst_regA] < r[inst_regB]) ? 32'hffffffff : 32'h0;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_CMPLT_i:
					begin
						`DISPLAY_OP("cmplt_i");
						r[0] <= (r[inst_regA] < inst_data) ? 32'hffffffff : 32'h0;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_CMPGT:
					begin
						`DISPLAY_OP("cmpgt");
						r[0] <= (r[inst_regA] > r[inst_regB]) ? 32'hffffffff : 32'h0;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_CMPGT_i:
					begin
						`DISPLAY_OP("cmpgt_i");
						r[0] <= (r[inst_regA] > inst_data) ? 32'hffffffff : 32'h0;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_CMPLE:
					begin
						`DISPLAY_OP("cmple");
						r[0] <= (r[inst_regA] <= r[inst_regB]) ? 32'hffffffff : 32'h0;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_CMPLE_i:
					begin
						`DISPLAY_OP("cmple_i");
						r[0] <= (r[inst_regA] <= inst_data) ? 32'hffffffff : 32'h0;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_CMPGE:
					begin
						`DISPLAY_OP("cmpge");
						r[0] <= (r[inst_regA] >= r[inst_regB]) ? 32'hffffffff : 32'h0;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_CMPGE_i:
					begin
						`DISPLAY_OP("cmpge_i");
						r[0] <= (r[inst_regA] >= inst_data) ? 32'hffffffff : 32'h0;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_JMP:
					begin
						`DISPLAY_OP("jump");
						r[15] <= inst_data;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_JMPZ:
					begin
						`DISPLAY_OP("jmpz");
						if (r[0] == 32'h0)
							r[15] <= inst_data;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_JMPNZ:
					begin
						`DISPLAY_OP("jmpnz");
						if (r[0] != 32'h0)
							r[15] <= inst_data;
						state <= STATE_FETCH_A_ISSUE;
					end

				OP_PUSH:
					begin
						`DISPLAY_OP("push");
						oRW <= 1;
						oADDR <= r[14];
						oDATA <= r[inst_regB];
						r[14] <= r[14] - 32'h1;
						state <= STATE_STORE_COMPLETE;
						request <= 1'b1;
					end

				OP_PUSH_i:
					begin
						`DISPLAY_OP("push_i");
					end

				OP_POP:
					begin
						`DISPLAY_OP("pop");
						oRW <= 0;
						oADDR <= r[14] + 32'h1;
						r[14] <= r[14] + 32'h1;
						index <= inst_regA;
						state <= STATE_LOAD_COMPLETE;
						request <= 1'b1;
					end

				OP_CALL:
					begin
						`DISPLAY_OP("call");
						oRW <= 1;
						oADDR <= r[14];
						oDATA <= r[15];
						r[14] <= r[14] - 32'h1;
						r[15] <= inst_data;
						state <= STATE_STORE_COMPLETE;
						request <= 1'b1;
					end

				OP_RET:
					begin
						`DISPLAY_OP("ret");
						oRW <= 0;
						oADDR <= r[14] + 32'h1;
						r[14] <= r[14] + 32'h1;
						index <= 15;
						state <= STATE_LOAD_COMPLETE;
						request <= 1'b1;					
					end

				default:
					begin
						`DISPLAY_OP_1("** invalid instruction %h **", inst_op);
						state <= STATE_FETCH_A_ISSUE;
					end
			endcase
		end

		else if (state == STATE_LOAD_COMPLETE) begin
			if (iRDY) begin
				r[index] <= iDATA;
				request <= 1'b0;
				state <= STATE_FETCH_A_ISSUE;
			end
		end

		else if (state == STATE_STORE_COMPLETE) begin
			if (iRDY) begin
				request <= 1'b0;
				state <= STATE_FETCH_A_ISSUE;
			end
		end
	end

endmodule