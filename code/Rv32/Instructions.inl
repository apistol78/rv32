if ((word & 0xfe00707f) == 0x00000033)	// ADD
{
	auto f = parseFormatR(word);
	R_s(f.rd) = R_s(f.rs1) + R_s(f.rs2);
}
else if ((word & 0x0000707f) == 0x00000013)	// ADDI
{
	auto f = parseFormatI(word);
	R_s(f.rd) = R_s(f.rs1) + f.imm;
}
else if ((word & 0xfe00707f) == 0x00007033)	// AND
{
	auto f = parseFormatR(word);
	R_u(f.rd) = R_u(f.rs1) & R_u(f.rs2);
}
else if ((word & 0x0000707f) == 0x00007013)	// ANDI
{
	auto f = parseFormatI(word);
	R_u(f.rd) = R_u(f.rs1) & f.imm;
}
else if ((word & 0x0000007f) == 0x00000017)	// AUIPC
{
	auto f = parseFormatU(word);
	R_u(f.rd) = PC + f.imm;
}
else if ((word & 0x0000707f) == 0x00000063)	// BEQ
{
	auto f = parseFormatB(word);
	if (R_s(f.rs1) == R_s(f.rs2))
		PC_NEXT = PC + f.imm;
}
else if ((word & 0x0000707f) == 0x00005063)	// BGE
{
	auto f = parseFormatB(word);
	if (R_s(f.rs1) >= R_s(f.rs2))
		PC_NEXT = PC + f.imm;
}
else if ((word & 0x0000707f) == 0x00007063)	// BGEU
{
	auto f = parseFormatB(word);
	if (R_u(f.rs1) >= R_u(f.rs2))
		PC_NEXT = PC + f.imm;
}
else if ((word & 0x0000707f) == 0x00004063)	// BLT
{
	auto f = parseFormatB(word);
	if (R_s(f.rs1) < R_s(f.rs2))
		PC_NEXT = PC + f.imm;
}
else if ((word & 0x0000707f) == 0x00006063)	// BLTU
{
	auto f = parseFormatB(word);
	if (R_u(f.rs1) < R_u(f.rs2))
		PC_NEXT = PC + f.imm;
}
else if ((word & 0x0000707f) == 0x00001063)	// BNE
{
	auto f = parseFormatB(word);
	if (R_s(f.rs1) != R_s(f.rs2))
		PC_NEXT = PC + f.imm;
}
else if ((word & 0x0000707f) == 0x00003073)	// CSRRC
{
	auto f = parseFormatCSR(word);
	uint32_t data = readCSR(f.csr);
	uint32_t tmp = R_u(f.rs1);
	R_u(f.rd) = data;
	writeCSR(f.csr, R_u(f.rd) & ~tmp);
}
else if ((word & 0x0000707f) == 0x00007073)	// CSRRCI
{
	log::info << L"CSRRCI" << Endl;
}
else if ((word & 0x0000707f) == 0x00002073)	// CSRRS
{
	auto f = parseFormatCSR(word);
	uint32_t data = readCSR(f.csr);
	uint32_t tmp = R_u(f.rs1);
	R_u(f.rd) = data;
	writeCSR(f.csr, R_u(f.rd) | tmp);
}
else if ((word & 0x0000707f) == 0x00006073)	// CSRRSI
{
	log::info << L"CSRRSI" << Endl;
}
else if ((word & 0x0000707f) == 0x00001073)	// CSRRW
{
	auto f = parseFormatCSR(word);
	R_s(f.rd) = readCSR(f.csr);
	writeCSR(f.csr, R_s(f.rs1));
}
else if ((word & 0x0000707f) == 0x00005073)	// CSRRWI
{
	log::info << L"CSRRWI" << Endl;
}
else if ((word & 0xfe00707f) == 0x02004033)	// DIV
{
	auto f = parseFormatR(word);
	int32_t dividend = R_s(f.rs1);
	int32_t divisor = R_s(f.rs2);
	if (divisor == 0)
		R_s(f.rd) = -1;
	else if (dividend == std::numeric_limits< int32_t >::lowest() && divisor == -1)
		R_s(f.rd) = std::numeric_limits< int32_t >::lowest();
	else
		R_s(f.rd) = dividend / divisor;
}
else if ((word & 0xfe00707f) == 0x02005033)	// DIVU
{
	auto f = parseFormatR(word);
	uint32_t dividend = R_u(f.rs1);
	uint32_t divisor = R_u(f.rs2);
	if (divisor ==  0)
		R_u(f.rd) = 0xffffffff;
	else		
		R_u(f.rd) = dividend / divisor;
}
else if ((word & 0xffffffff) == 0x00100073)	// EBREAK
{
	return false;
}
else if ((word & 0xffffffff) == 0x00000073)	// ECALL
{
}
else if ((word & 0xfe00007f) == 0x00000053)	// FADD
{
	auto f = parseFormatR(word);
	FR(f.rd) = FR(f.rs1) + FR(f.rs2);
}
else if ((word & 0xfff0007f) == 0xc0000053)	// FCVT_W_S
{
	auto f = parseFormatR(word);
	R(f.rd) = (int32_t)FR(f.rs1);
}
else if ((word & 0xfff0007f) == 0xd0000053)	// FCVT_S_W
{
	auto f = parseFormatR(word);
	FR(f.rd) = (float)R(f.rs1);
}
else if ((word & 0xfff0007f) == 0xd0100053)	// FCVT_S_WU
{
	auto f = parseFormatR(word);
	FR(f.rd) = (float)R(f.rs1);
}
else if ((word & 0xfe00007f) == 0x18000053)	// FDIV
{
	auto f = parseFormatR(word);
	FR(f.rd) = FR(f.rs1) / FR(f.rs2);
}
else if ((word & 0x0000707f) == 0x0000000f)	// FENCE
{
	flushCaches();
}
else if ((word & 0xfe00707f) == 0xa0002053)	// FEQ
{
	auto f = parseFormatR(word);
	R(f.rd) = (FR(f.rs1) == FR(f.rs2) ? 1 : 0);
}
else if ((word & 0xfe00707f) == 0xa0000053)	// FLE
{
	auto f = parseFormatR(word);
	R(f.rd) = (FR(f.rs1) <= FR(f.rs2) ? 1 : 0);
}
else if ((word & 0xfe00707f) == 0xa0001053)	// FLT
{
	auto f = parseFormatR(word);
	R(f.rd) = (FR(f.rs1) < FR(f.rs2) ? 1 : 0);
}
else if ((word & 0x0000707f) == 0x00002007)	// FLW
{
	auto f = parseFormatI(word);
	FR_u(f.rd) = (uint32_t)MEM_RD(R_u(f.rs1) + f.imm);
}
else if ((word & 0x0600007f) == 0x00000043)	// FMADD
{
	auto f = parseFormatR4(word);
	FR(f.rd) = FR(f.rs1) * FR(f.rs2) + FR(f.rs3);
}
else if ((word & 0x0600007f) == 0x00000047)	// FMSUB
{
	auto f = parseFormatR4(word);
	FR(f.rd) = FR(f.rs1) * FR(f.rs2) - FR(f.rs3);
}
else if ((word & 0x0600007f) == 0x0000004f)	// FNMADD
{
	auto f = parseFormatR4(word);
	FR(f.rd) = -(FR(f.rs1) * FR(f.rs2)) - FR(f.rs3);
}
else if ((word & 0x0600007f) == 0x0000004b)	// FNMSUB
{
	auto f = parseFormatR4(word);
	FR(f.rd) = -(FR(f.rs1) * FR(f.rs2)) + FR(f.rs3);
}
else if ((word & 0xfe00707f) == 0x28000053)	// FMIN
{
	auto f = parseFormatR(word);
	FR(f.rd) = std::min(FR(f.rs1), FR(f.rs2));
}
else if ((word & 0xfe00707f) == 0x28001053)	// FMAX
{
	auto f = parseFormatR(word);
	FR(f.rd) = std::max(FR(f.rs1), FR(f.rs2));
}
else if ((word & 0xfe00007f) == 0x10000053)	// FMUL
{
	auto f = parseFormatR(word);
	FR(f.rd) = FR(f.rs1) * FR(f.rs2);
}
else if ((word & 0xfff0707f) == 0xe0000053)	// FMV_X_W
{
	auto f = parseFormatR(word);
	R(f.rd) = FR_u(f.rs1);
}
else if ((word & 0xfff0707f) == 0xf0000053)	// FMV_W_X
{
	auto f = parseFormatR(word);
	FR_u(f.rd) = R(f.rs1);
}
else if ((word & 0xfe00707f) == 0x20000053)	// FSGNJ
{
	auto f = parseFormatR(word);
	uint32_t tmp1 = FR_u(f.rs1);
	uint32_t tmp2 = FR_u(f.rs2);
	FR_u(f.rd) = (tmp1 & 0x7fffffff) | (tmp2 & 0x80000000);
}
else if ((word & 0xfe00707f) == 0x20001053)	// FSGNJN
{
	auto f = parseFormatR(word);
	uint32_t tmp1 = FR_u(f.rs1);
	uint32_t tmp2 = FR_u(f.rs2);
	FR_u(f.rd) = (tmp1 & 0x7fffffff) | ((tmp2 & 0x80000000) ^ 0x80000000);
}
else if ((word & 0xfe00707f) == 0x20002053)	// FSGNJX
{
	auto f = parseFormatR(word);
	uint32_t tmp1 = FR_u(f.rs1);
	uint32_t tmp2 = FR_u(f.rs2);
	FR_u(f.rd) = (tmp1 & 0x7fffffff) | ((tmp2 & 0x80000000) ^ (tmp1 & 0x80000000));
}
else if ((word & 0xfe00007f) == 0x08000053)	// FSUB
{
	auto f = parseFormatR(word);
	FR(f.rd) = FR(f.rs1) - FR(f.rs2);
}
else if ((word & 0x0000707f) == 0x00002027)	// FSW
{
	auto f = parseFormatS(word);
	MEM_WR(R_u(f.rs1) + f.imm, FR_u(f.rs2));
}
else if ((word & 0x0000007f) == 0x0000006f)	// JAL
{
	auto f = parseFormatJ(word);
	R_u(f.rd) = PC_NEXT;
	PC_NEXT = PC + f.imm;
}
else if ((word & 0x0000707f) == 0x00000067)	// JALR
{
	auto f = parseFormatI(word);
	R_u(f.rd) = PC_NEXT;
	PC_NEXT = (R(f.rs1) + f.imm) & ~1U;
}
else if ((word & 0x0000707f) == 0x00000003)	// LB
{
	auto f = parseFormatI(word);
	R_s(f.rd) = (int8_t)MEM_RD_U8(R_u(f.rs1) + f.imm);
}
else if ((word & 0x0000707f) == 0x00004003)	// LBU
{
	auto f = parseFormatI(word);
	R_u(f.rd) = MEM_RD_U8(R_u(f.rs1) + f.imm);
}
else if ((word & 0x0000707f) == 0x00001003)	// LH
{
	auto f = parseFormatI(word);
	R_s(f.rd) = (int16_t)MEM_RD_U16(R_u(f.rs1) + f.imm);
}
else if ((word & 0x0000707f) == 0x00005003)	// LHU
{
	auto f = parseFormatI(word);
	R(f.rd) = MEM_RD_U16(R(f.rs1) + f.imm);			
}
else if ((word & 0x0000007f) == 0x00000037)	// LUI
{
	auto f = parseFormatU(word);
	R_s(f.rd) = f.imm;
}
else if ((word & 0x0000707f) == 0x00002003)	// LW
{
	auto f = parseFormatI(word);
	R_s(f.rd) = (int32_t)MEM_RD(R_u(f.rs1) + f.imm);
}
else if ((word & 0xfe00707f) == 0x02000033)	// MUL
{
	auto f = parseFormatR(word);
	R_s(f.rd) = R_s(f.rs1) * R_s(f.rs2);
}
else if ((word & 0xfe00707f) == 0x02001033)	// MULH
{
	auto f = parseFormatR(word);
	int64_t lh = (int64_t)R_s(f.rs1);
	int64_t rh = (int64_t)R_s(f.rs2);
	R_s(f.rd) = (lh * rh) >> 32;
}
else if ((word & 0xfe00707f) == 0x02003033)	// MULHU
{
	auto f = parseFormatR(word);
	uint64_t lh = (uint64_t)R(f.rs1);
	uint64_t rh = (uint64_t)R(f.rs2);
	R(f.rd) = (lh * rh) >> 32;
}
else if ((word & 0xffffffff) == 0x30200073)	// MRET
{
	PC_NEXT = readCSR(CSR::MEPC);
	log::info << L"MRET " << str(L"%08x", PC_NEXT) << Endl;
}
else if ((word & 0xfe00707f) == 0x00006033)	// OR
{
	auto f = parseFormatR(word);
	R_u(f.rd) = R_u(f.rs1) | R_u(f.rs2);
}
else if ((word & 0x0000707f) == 0x00006013)	// ORI
{
	auto f = parseFormatI(word);
	R_u(f.rd) = R_u(f.rs1) | f.imm;
}
else if ((word & 0xfe00707f) == 0x02006033)	// REM
{
	auto f = parseFormatR(word);
	int32_t dividend = R_s(f.rs1);
	int32_t divisor = R_s(f.rs2);
	if (divisor == 0)
		R_s(f.rd) = dividend;
	else if (dividend == std::numeric_limits< int32_t >::lowest() && divisor == -1)
		R_s(f.rd) = 0;
	else
		R_s(f.rd) = dividend % divisor;
}
else if ((word & 0xfe00707f) == 0x02007033)	// REMU
{
	auto f = parseFormatR(word);
	uint32_t dividend = R_u(f.rs1);
	uint32_t divisor = R_u(f.rs2);
	if (divisor == 0)
		R_u(f.rd) = dividend;
	else
		R_u(f.rd) = dividend % divisor;
}
else if ((word & 0x0000707f) == 0x00000023)	// SB
{
	auto f = parseFormatS(word);
	MEM_WR_U8(R_u(f.rs1) + f.imm, (uint8_t)R_u(f.rs2));
}
else if ((word & 0x0000707f) == 0x00001023)	// SH
{
	auto f = parseFormatS(word);
	MEM_WR_U16(R_u(f.rs1) + f.imm, R_u(f.rs2));
}
else if ((word & 0xfe00707f) == 0x00001033)	// SLL
{
	auto f = parseFormatR(word);
	R_s(f.rd) = R_s(f.rs1) << R_u(f.rs2);
}
else if ((word & 0xfc00707f) == 0x00001013)	// SLLI
{
	auto f = parseFormatR(word);
	R_s(f.rd) = R_s(f.rs1) << ((word >> 20) & 0x1f);
}
else if ((word & 0xfe00707f) == 0x00002033)	// SLT
{
	auto f = parseFormatR(word);
	R_u(f.rd) = (R_s(f.rs1) < R_s(f.rs2)) ? 1 : 0;
}
else if ((word & 0x0000707f) == 0x00002013)	// SLTI
{
	auto f = parseFormatI(word);
	R_u(f.rd) = (R_s(f.rs1) < f.imm) ? 1 : 0;
}
else if ((word & 0x0000707f) == 0x00003013)	// SLTIU
{
	auto f = parseFormatI(word);
	R_u(f.rd) = (R_u(f.rs1) < f.imm) ? 1 : 0;
}
else if ((word & 0xfe00707f) == 0x00003033)	// SLTU
{
	auto f = parseFormatR(word);
	R_u(f.rd) = (R_u(f.rs1) < R_u(f.rs2)) ? 1 : 0;
}
else if ((word & 0xfe00707f) == 0x40005033)	// SRA
{
	auto f = parseFormatR(word);
	const int32_t sh = R_s(f.rs2);
	if (sh >= 0)
		R_s(f.rd) = R_s(f.rs1) >> sh;
	else
		log::error << L"SRA, shift by negative not supported!" << Endl;
}
else if ((word & 0xfc00707f) == 0x40005013)	// SRAI
{
	auto f = parseFormatR(word);
	const int32_t sh = (int32_t)((word >> 20) & 0x1f);
	R_s(f.rd) = R_s(f.rs1) >> sh;
}
else if ((word & 0xfe00707f) == 0x00005033)	// SRL
{
	auto f = parseFormatR(word);
	R_u(f.rd) = R_u(f.rs1) >> R_u(f.rs2);
}
else if ((word & 0xfc00707f) == 0x00005013)	// SRLI
{
	auto f = parseFormatR(word);
	R_u(f.rd) = R_u(f.rs1) >> ((word >> 20) & 0x1f);
}
else if ((word & 0xfe00707f) == 0x40000033)	// SUB
{
	auto f = parseFormatR(word);
	R_s(f.rd) = R_s(f.rs1) - R_s(f.rs2);
}
else if ((word & 0x0000707f) == 0x00002023)	// SW
{
	auto f = parseFormatS(word);
	MEM_WR(R_u(f.rs1) + f.imm, R_u(f.rs2));
}
else if ((word & 0xffffffff) == 0x10500073)	// WFI
{
	m_waitForInterrupt = true;
}
else if ((word & 0xfe00707f) == 0x00004033)	// XOR
{
	auto f = parseFormatR(word);
	R_u(f.rd) = R_u(f.rs1) ^ R_u(f.rs2);
}
else if ((word & 0x0000707f) == 0x00004013)	// XORI
{
	auto f = parseFormatI(word);
	R_u(f.rd) = R_u(f.rs1) ^ f.imm;
}
else
{
	log::error << L"Unknown instruction, " << str(L"%08x", word) << Endl;
	return false;
}
return true;
