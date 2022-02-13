`ifndef _CPU_DEFINES_SV
`define _CPU_DEFINES_SV

// Tag size, wider is useful for debugging but no more than a
// single bit should be necessary.
`define TAG_SIZE                    1:0

// Size of register index.
`define REG_ID_SIZE					5:0

// Multiply and divide
`ifndef __VERILATOR__
	`define MUL_CYCLE_LATENCY       3 // 3+1
	`define DIV_CYCLE_LATENCY       15 // 15+1
`else
	`define MUL_CYCLE_LATENCY       2
	`define DIV_CYCLE_LATENCY       2
`endif

// ALU operations
`define OP_SIGNED_ADD               1
`define OP_UNSIGNED_ADD             2
`define OP_SIGNED_SUB               3
`define OP_AND                      4
`define OP_OR                       5
`define OP_XOR                      6
`define OP_SHIFT_LEFT               7
`define OP_SHIFT_RIGHT              8
`define OP_ARITHMETIC_SHIFT_RIGHT   9
`define OP_SIGNED_LESS_THAN         10
`define OP_UNSIGNED_LESS_THAN       11
`define OP_EQUAL                    12
`define OP_NOT_EQUAL                13
`define OP_SIGNED_GREATER_EQUAL     14
`define OP_UNSIGNED_GREATER_EQUAL   15

// FPU operations
`define FPU_OP_ADD		1
`define FPU_OP_SUB		2
`define FPU_OP_MUL		3
`define FPU_OP_DIV		4
`define FPU_OP_SQRT		5
`define FPU_OP_MIN		6
`define FPU_OP_MAX		7
`define FPU_OP_CMP		8
`define FPU_OP_CLASS	9

// CSR registers
`define CSR_MIE			12'h304
`define CSR_MTVEC		12'h305
`define CSR_MEPC		12'h341
`define CSR_MCAUSE		12'h342
`define CSR_MIP			12'h343

`define CSR_MVENDORID	12'hf11
`define CSR_MARCHID		12'hf12
`define CSR_MIMPID		12'hf13
`define CSR_MHARTID		12'hf14

`endif	// _CPU_DEFINES_SV
