// This file is automatically generated, DO NOT EDIT!
// ==================================================

wire [3:0] alu_operation = 
	is_ADD   ? `OP_SIGNED_ADD             :
	is_ADDI  ? `OP_SIGNED_ADD             :
	is_AND   ? `OP_AND                    :
	is_ANDI  ? `OP_AND                    :
	is_AUIPC ? `OP_SIGNED_ADD             :
	is_BEQ   ? `OP_EQUAL                  :
	is_BGE   ? `OP_SIGNED_GREATER_EQUAL   :
	is_BGEU  ? `OP_UNSIGNED_GREATER_EQUAL :
	is_BLT   ? `OP_SIGNED_LESS_THAN       :
	is_BLTU  ? `OP_UNSIGNED_LESS_THAN     :
	is_BNE   ? `OP_NOT_EQUAL              :
	is_JAL   ? `OP_SIGNED_ADD             :
	is_JALR  ? `OP_SIGNED_ADD             :
	is_LB    ? `OP_SIGNED_ADD             :
	is_LBU   ? `OP_SIGNED_ADD             :
	is_LH    ? `OP_SIGNED_ADD             :
	is_LHU   ? `OP_SIGNED_ADD             :
	is_LUI   ? `OP_UNSIGNED_ADD           :
	is_LW    ? `OP_SIGNED_ADD             :
	is_OR    ? `OP_OR                     :
	is_ORI   ? `OP_OR                     :
	is_SB    ? `OP_SIGNED_ADD             :
	is_SH    ? `OP_SIGNED_ADD             :
	is_SLL   ? `OP_SHIFT_LEFT             :
	is_SLLI  ? `OP_SHIFT_LEFT             :
	is_SLT   ? `OP_SIGNED_LESS_THAN       :
	is_SLTI  ? `OP_SIGNED_LESS_THAN       :
	is_SLTIU ? `OP_UNSIGNED_LESS_THAN     :
	is_SLTU  ? `OP_UNSIGNED_LESS_THAN     :
	is_SRA   ? `OP_ARITHMETIC_SHIFT_RIGHT :
	is_SRAI  ? `OP_ARITHMETIC_SHIFT_RIGHT :
	is_SRL   ? `OP_SHIFT_RIGHT            :
	is_SRLI  ? `OP_SHIFT_RIGHT            :
	is_SUB   ? `OP_SIGNED_SUB             :
	is_SW    ? `OP_SIGNED_ADD             :
	is_XOR   ? `OP_XOR                    :
	is_XORI  ? `OP_XOR                    :
	0;

wire [2:0] alu_operand1 = 
	is_ADD   ? `RS1  :
	is_ADDI  ? `RS1  :
	is_AND   ? `RS1  :
	is_ANDI  ? `RS1  :
	is_AUIPC ? `PC   :
	is_BEQ   ? `RS1  :
	is_BGE   ? `RS1  :
	is_BGEU  ? `RS1  :
	is_BLT   ? `RS1  :
	is_BLTU  ? `RS1  :
	is_BNE   ? `RS1  :
	is_JAL   ? `PC   :
	is_JALR  ? `RS1  :
	is_LB    ? `RS1  :
	is_LBU   ? `RS1  :
	is_LH    ? `RS1  :
	is_LHU   ? `RS1  :
	is_LUI   ? `IMM  :
	is_LW    ? `RS1  :
	is_OR    ? `RS1  :
	is_ORI   ? `RS1  :
	is_SB    ? `RS1  :
	is_SH    ? `RS1  :
	is_SLL   ? `RS1  :
	is_SLLI  ? `RS1  :
	is_SLT   ? `RS1  :
	is_SLTI  ? `RS1  :
	is_SLTIU ? `RS1  :
	is_SLTU  ? `RS1  :
	is_SRA   ? `RS1  :
	is_SRAI  ? `RS1  :
	is_SRL   ? `RS1  :
	is_SRLI  ? `RS1  :
	is_SUB   ? `RS1  :
	is_SW    ? `RS1  :
	is_XOR   ? `RS1  :
	is_XORI  ? `RS1  :
	0;

wire [2:0] alu_operand2 = 
	is_ADD   ? `RS2  :
	is_ADDI  ? `IMM  :
	is_AND   ? `RS2  :
	is_ANDI  ? `IMM  :
	is_AUIPC ? `IMM  :
	is_BEQ   ? `RS2  :
	is_BGE   ? `RS2  :
	is_BGEU  ? `RS2  :
	is_BLT   ? `RS2  :
	is_BLTU  ? `RS2  :
	is_BNE   ? `RS2  :
	is_JAL   ? `IMM  :
	is_JALR  ? `IMM  :
	is_LB    ? `IMM  :
	is_LBU   ? `IMM  :
	is_LH    ? `IMM  :
	is_LHU   ? `IMM  :
	is_LUI   ? `ZERO :
	is_LW    ? `IMM  :
	is_OR    ? `RS2  :
	is_ORI   ? `IMM  :
	is_SB    ? `IMM  :
	is_SH    ? `IMM  :
	is_SLL   ? `RS2  :
	is_SLLI  ? `IMM  :
	is_SLT   ? `RS2  :
	is_SLTI  ? `IMM  :
	is_SLTIU ? `IMM  :
	is_SLTU  ? `RS2  :
	is_SRA   ? `RS2  :
	is_SRAI  ? `IMM  :
	is_SRL   ? `RS2  :
	is_SRLI  ? `IMM  :
	is_SUB   ? `RS2  :
	is_SW    ? `IMM  :
	is_XOR   ? `RS2  :
	is_XORI  ? `IMM  :
	0;
