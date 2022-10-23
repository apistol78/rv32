`ifndef _CPU_TYPES_SV
`define _CPU_TYPES_SV

`include "CPU_Defines.sv"

typedef enum bit [`REG_ID_SIZE-1:0]
{
	ZERO = `REG_ID_SIZE'd0,
	RA =   `REG_ID_SIZE'd1,
	SP =   `REG_ID_SIZE'd2,
	GP =   `REG_ID_SIZE'd3,
	TP =   `REG_ID_SIZE'd4,
	T0 =   `REG_ID_SIZE'd5,
	T1 =   `REG_ID_SIZE'd6,
	T2 =   `REG_ID_SIZE'd7,
	S0 =   `REG_ID_SIZE'd8,
	S1 =   `REG_ID_SIZE'd9,
	A0 =   `REG_ID_SIZE'd10,
	A1 =   `REG_ID_SIZE'd11,
	A2 =   `REG_ID_SIZE'd12,
	A3 =   `REG_ID_SIZE'd13,
	A4 =   `REG_ID_SIZE'd14,
	A5 =   `REG_ID_SIZE'd15,
	A6 =   `REG_ID_SIZE'd16,
	A7 =   `REG_ID_SIZE'd17,
	S2 =   `REG_ID_SIZE'd18,
	S3 =   `REG_ID_SIZE'd19,
	S4 =   `REG_ID_SIZE'd20,
	S5 =   `REG_ID_SIZE'd21,
	S6 =   `REG_ID_SIZE'd22,
	S7 =   `REG_ID_SIZE'd23,
	S8 =   `REG_ID_SIZE'd24,
	S9 =   `REG_ID_SIZE'd25,
	S10 =  `REG_ID_SIZE'd26,
	S11 =  `REG_ID_SIZE'd27,
	T3 =   `REG_ID_SIZE'd28,
	T4 =   `REG_ID_SIZE'd29,
	T5 =   `REG_ID_SIZE'd30,
	T6 =   `REG_ID_SIZE'd31
}
register_t;

/**! Output data from FETCH stage. */
typedef struct packed
{
	bit strobe;				//!< Toggled each time data is updated.
	bit [31:0] instruction;
	bit [31:0] pc;
	register_t inst_rs1;
	register_t inst_rs2;
	register_t inst_rs3;
	register_t inst_rd;
	bit [31:0] imm;			//!< Immediate number.
}
fetch_data_t;

/**! Output data from DECODE stage. */
typedef struct packed
{
	bit strobe;					//!< Toggled each time data is updated.
	bit [31:0] pc;				//!< Program counter.
	bit [2:0] have_rs;
	register_t inst_rs1;		//!< Index source register 1.
	register_t inst_rs2;		//!< Index source register 2.
	register_t inst_rs3;		//!< Index source register 3.
	register_t inst_rd;			//!< Index destination register.
	bit [31:0] imm;				//!< Immediate number.
	bit arithmetic;				//!< Arithmetic instruction.
	bit shift;					//!< Shift instruction.
	bit compare;				//!< Compare instruction.
	bit complx;					//!< Complex instruction.
	bit jump;					//!< Jump instruction.
	bit jump_conditional;		//!< Conditional jump instruction.
	bit [3:0] alu_operation;	//!< ALU operation code.
	bit [4:0] alu_operand1;		//!< ALU operand 1, one hot.
	bit [4:0] alu_operand2;		//!< ALU operand 2, one hot.
	bit memory_read;			//!< Memory read instruction.
	bit memory_write;			//!< Memory write instruction.
	bit [1:0] memory_width;		//!< Memory access width (00 = 1, 01 = 2, 10 = 4, 11 = invalid) in bytes.
	bit memory_signed;			//!< Signed extended memory access.
	bit [4:0] op;				//!< Complex instruction operation code.
`ifdef FPU_ENABLE
	bit fpu;					//!< FPU instruction.
	bit [4:0] fpu_operation;	//!< FPU operation code.
`endif
}
decode_data_t;

/**! Output data from EXECUTE stage. */
typedef struct packed
{
	bit strobe;					//!< Toggled each time data is updated.
	register_t inst_rd;
	bit [31:0] rd;
	bit mem_read;
	bit mem_write;
	bit mem_flush;
	bit [1:0] mem_width;
	bit mem_signed;
	bit [31:0] mem_address;
	register_t mem_inst_rd;		//!< Memory load into register, separate from inst_rd since we cannot forward from execute on load.
}
execute_data_t;

/**! Output data from MEMORY stage. */
typedef struct packed
{
	bit strobe;					//!< Toggled each time data is updated.
	register_t inst_rd;
	bit [31:0] rd;
}
memory_data_t;

/**! Output data from WRITEBACK stage. */
typedef struct packed
{
	register_t inst_rd;
	bit [31:0] rd;
}
writeback_data_t;

`endif  // _CPU_TYPES_SV
