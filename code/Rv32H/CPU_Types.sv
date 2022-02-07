`ifndef _CPU_TYPES_SV
`define _CPU_TYPES_SV

`include "CPU_Defines.sv"

typedef struct packed
{
	logic [`TAG_SIZE] tag;
	logic [31:0] instruction;
	logic [31:0] pc;
}
fetch_data_t;

typedef struct packed
{
	logic [`TAG_SIZE] tag;
	logic [31:0] pc;			//!< Program counter.
	logic [4:0] inst_rs1;		//!< Index source register 1.
	logic [4:0] inst_rs2;		//!< Index source register 2.
	logic [4:0] inst_rd;		//!< Index destination register.
	logic [31:0] imm;			//!< Immediate number.
	logic arithmetic;			//!< Arithmetic instruction.
	logic compare;				//!< Compare instruction.
	logic complx;				//!< Complex instruction.
	logic jump;					//!< Jump instruction.
	logic jump_conditional;		//!< Conditional jump instruction.
	logic [3:0] alu_operation;	//!< ALU operation code.
	logic [2:0] alu_operand1;	//!< ALU operand 1 index.
	logic [2:0] alu_operand2;	//!< ALU operand 2 index.
	logic memory_read;			//!< Memory read instruction.
	logic memory_write;			//!< Memory write instruction.
	logic [2:0] memory_width;	//!< Memory access width (1, 2, or 4) in bytes.
	logic memory_signed;		//!< Signed extended memory access.
	logic [4:0] op;				//!< Complex instruction operation code.
}
decode_data_t;

typedef struct packed
{
	logic [`TAG_SIZE] tag;
	logic [4:0] inst_rd;
	logic [31:0] rd;
	logic mem_read;
	logic mem_write;
	logic mem_flush;
	logic [2:0] mem_width;
	logic mem_signed;
	logic [31:0] mem_address;
}
execute_data_t;

typedef struct packed
{
	logic [`TAG_SIZE] tag;
	logic [4:0] inst_rd;
	logic [31:0] rd;
}
memory_data_t;

typedef struct packed
{
	logic [`TAG_SIZE] tag;
	logic [4:0] inst_rd;
	logic [31:0] rd;
}
writeback_data_t;

`endif  // _CPU_TYPES_SV
