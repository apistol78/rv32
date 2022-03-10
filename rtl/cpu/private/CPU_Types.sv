`ifndef _CPU_TYPES_SV
`define _CPU_TYPES_SV

`include "CPU_Defines.sv"

typedef enum bit [`REG_ID_SIZE]
{
	ZERO = 6'd0,
	RA = 6'd1,
	SP = 6'd2,
	GP = 6'd3,
	TP = 6'd4,
	T0 = 6'd5,
	T1 = 6'd6,
	T2 = 6'd7,
	S0 = 6'd8,
	S1 = 6'd9,
	A0 = 6'd10,
	A1 = 6'd11,
	A2 = 6'd12,
	A3 = 6'd13,
	A4 = 6'd14,
	A5 = 6'd15,
	A6 = 6'd16,
	A7 = 6'd17,
	S2 = 6'd18,
	S3 = 6'd19,
	S4 = 6'd20,
	S5 = 6'd21,
	S6 = 6'd22,
	S7 = 6'd23,
	S8 = 6'd24,
	S9 = 6'd25,
	S10 = 6'd26,
	S11 = 6'd27,
	T3 = 6'd28,
	T4 = 6'd29,
	T5 = 6'd30,
	T6 = 6'd31
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
	register_t inst_rs3;			//!< Index source register 3.
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
