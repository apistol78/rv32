`ifndef _CPU_TYPES_SV
`define _CPU_TYPES_SV

`include "CPU_Defines.sv"

typedef enum bit [`REG_ID_SIZE]
{
	ZERO = 5'd0,
	RA = 5'd1,
	SP = 5'd2,
	GP = 5'd3,
	TP = 5'd4,
	T0 = 5'd5,
	T1 = 5'd6,
	T2 = 5'd7,
	S0 = 5'd8,
	S1 = 5'd9,
	A0 = 5'd10,
	A1 = 5'd11,
	A2 = 5'd12,
	A3 = 5'd13,
	A4 = 5'd14,
	A5 = 5'd15,
	A6 = 5'd16,
	A7 = 5'd17,
	S2 = 5'd18,
	S3 = 5'd19,
	S4 = 5'd20,
	S5 = 5'd21,
	S6 = 5'd22,
	S7 = 5'd23,
	S8 = 5'd24,
	S9 = 5'd25,
	S10 = 5'd26,
	S11 = 5'd27,
	T3 = 5'd28,
	T4 = 5'd29,
	T5 = 5'd30,
	T6 = 5'd31
}
register_t;

typedef struct packed
{
	logic [`TAG_SIZE] tag;
	logic [31:0] instruction;
	logic [31:0] pc;
	register_t inst_rs1;
	register_t inst_rs2;
	register_t inst_rs3;
	register_t inst_rd;
}
fetch_data_t;

typedef struct packed
{
	logic [`TAG_SIZE] tag;
	logic [31:0] pc;				//!< Program counter.
	logic [2:0] have_rs;
	register_t inst_rs1;			//!< Index source register 1.
	register_t inst_rs2;			//!< Index source register 2.
	register_t inst_rs3;			//!< Index source register 3 (FP only).
	register_t inst_rd;				//!< Index destination register.
	logic [31:0] imm;				//!< Immediate number.
	logic arithmetic;				//!< Arithmetic instruction.
	logic shift;					//!< Shift instruction.
	logic compare;					//!< Compare instruction.
	logic complx;					//!< Complex instruction.
	logic jump;						//!< Jump instruction.
	logic jump_conditional;			//!< Conditional jump instruction.
	logic fpu;						//!< FPU instruction.
	logic [3:0] alu_operation;		//!< ALU operation code.
	logic [2:0] alu_operand1;		//!< ALU operand 1 index.
	logic [2:0] alu_operand2;		//!< ALU operand 2 index.
	logic memory_read;				//!< Memory read instruction.
	logic memory_write;				//!< Memory write instruction.
	logic [2:0] memory_width;		//!< Memory access width (1, 2, or 4) in bytes.
	logic memory_signed;			//!< Signed extended memory access.
	logic [4:0] op;					//!< Complex instruction operation code.
	logic [4:0] fpu_operation;		//!< FPU operation code.
}
decode_data_t;

typedef struct packed
{
	logic [`TAG_SIZE] tag;
	register_t inst_rd;
	logic [31:0] rd;
	logic mem_read;
	logic mem_write;
	logic mem_flush;
	logic [2:0] mem_width;
	logic mem_signed;
	logic [31:0] mem_address;
	register_t mem_inst_rd;			//!< Memory load into register, separate from inst_rd since we cannot forward from execute on load.
}
execute_data_t;

typedef struct packed
{
	logic [`TAG_SIZE] tag;
	register_t inst_rd;
	logic [31:0] rd;
}
memory_data_t;

typedef struct packed
{
	logic [`TAG_SIZE] tag;
	register_t inst_rd;
	logic [31:0] rd;
}
writeback_data_t;

`endif  // _CPU_TYPES_SV
