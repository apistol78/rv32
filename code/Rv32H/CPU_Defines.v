
// Tag size, wider is useful for debugging but no more than a
// single bit should be necessary.
`define TAG_SIZE                    3:0

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
