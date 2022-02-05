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
	logic [31:0] instruction;
	logic [31:0] pc;
	logic [4:0] inst_rs1;
	logic [4:0] inst_rs2;
	logic [4:0] inst_rd;
	logic [31:0] imm;

	logic arithmetic;
	logic compare;
	logic complx;
	logic jump;
	logic jump_conditional;

	logic [3:0] alu_operation;
	logic [2:0] alu_operand1;
	logic [2:0] alu_operand2;

	logic memory_read;
	logic memory_write;
	logic [2:0] memory_width;
	logic memory_signed;

	logic [4:0] op;
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
