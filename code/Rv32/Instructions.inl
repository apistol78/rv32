if ((word & 0xfe00707f) == 0x00000033)
{
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) + R_s(f.rs2);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00000013)
{
			auto f = parseFormatI(word);
			R_s(f.rd) = R_s(f.rs1) + f.imm;
        
	return true;
}
else if ((word & 0x0000707f) == 0x0000001b)
{
	log::error << L"ADDIW not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x0000003b)
{
	log::error << L"ADDW not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x0000302f)
{
	log::error << L"AMOADD.D not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x0000202f)
{
	log::error << L"AMOADD.W not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x6000302f)
{
	log::error << L"AMOAND.D not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x6000202f)
{
	log::error << L"AMOAND.W not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0xe000302f)
{
	log::error << L"AMOMAXU.D not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0xe000202f)
{
	log::error << L"AMOMAXU.W not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x4000302f)
{
	log::error << L"AMOOR.D not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x4000202f)
{
	log::error << L"AMOOR.W not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x0800302f)
{
	log::error << L"AMOSWAP.D not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x0800202f)
{
	log::error << L"AMOSWAP.W not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x00007033)
{
			auto f = parseFormatR(word);
			R_u(f.rd) = R_u(f.rs1) & R_u(f.rs2);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00007013)
{
			auto f = parseFormatI(word);
			R_u(f.rd) = R_u(f.rs1) & f.imm;
        
	return true;
}
else if ((word & 0x0000007f) == 0x00000017)
{
			auto f = parseFormatU(word);
			R_u(f.rd) = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00000063)
{
			auto f = parseFormatB(word);
			if (R_s(f.rs1) == R_s(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00005063)
{
			auto f = parseFormatB(word);
			if (R_s(f.rs1) >= R_s(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00007063)
{
			auto f = parseFormatB(word);
			if (R_u(f.rs1) >= R_u(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00004063)
{
			auto f = parseFormatB(word);
			if (R_s(f.rs1) < R_s(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00006063)
{
			auto f = parseFormatB(word);
			if (R_u(f.rs1) < R_u(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00001063)
{
			auto f = parseFormatB(word);
			if (R_s(f.rs1) != R_s(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00003073)
{
			auto f = parseFormatCSR(word);
			uint32_t data = readCSR(f.csr);
			uint32_t tmp = R_u(f.rs1);
			R_u(f.rd) = data;
			writeCSR(f.csr, R_u(f.rd) & ~tmp);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00007073)
{
			log::info << L"CSRRCI" << Endl;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00002073)
{
			auto f = parseFormatCSR(word);
			uint32_t data = readCSR(f.csr);
			uint32_t tmp = R_u(f.rs1);
			R_u(f.rd) = data;
			writeCSR(f.csr, R_u(f.rd) | tmp);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00006073)
{
			log::info << L"CSRRSI" << Endl;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00001073)
{
			auto f = parseFormatCSR(word);
			R_s(f.rd) = readCSR(f.csr);
			writeCSR(f.csr, R_s(f.rs1));
		
	return true;
}
else if ((word & 0x0000707f) == 0x00005073)
{
			log::info << L"CSRRWI" << Endl;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02004033)
{
			auto f = parseFormatR(word);
			int32_t dividend = R_s(f.rs1);
			int32_t divisor = R_s(f.rs2);
			if (divisor == 0)
				R_s(f.rd) = -1;
			else if (dividend == -std::numeric_limits< int32_t >::max() && divisor == -1)
				R_s(f.rd) = dividend;
			else
				R_s(f.rd) = dividend / divisor;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02005033)
{
			auto f = parseFormatR(word);
			uint32_t dividend = R_u(f.rs1);
			uint32_t divisor = R_u(f.rs2);
			if (divisor ==  0)
				R_s(f.rd) = -1;
			else		
				R_u(f.rd) = dividend / divisor;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x0200503b)
{
	log::error << L"DIVUW not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x0200403b)
{
	log::error << L"DIVW not implemented." << Endl;
	return false;
}
else if ((word & 0xffffffff) == 0x00100073)
{
			return false;
		
	return true;
}
else if ((word & 0xffffffff) == 0x00000073)
{
		
	return true;
}
else if ((word & 0xfe00007f) == 0x00000053)
{
	log::error << L"FADD not implemented." << Endl;
	return false;
}
else if ((word & 0xfff0007f) == 0xc0000053)
{
	log::error << L"FCVT_W_S not implemented." << Endl;
	return false;
}
else if ((word & 0xfff0007f) == 0xd0000053)
{
	log::error << L"FCVT_S_W not implemented." << Endl;
	return false;
}
else if ((word & 0xfff0007f) == 0xd0100053)
{
	log::error << L"FCVT_S_WU not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00007f) == 0x18000053)
{
	log::error << L"FDIV not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x0000000f)
{
	log::error << L"FENCE not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x0000100f)
{
	log::error << L"FENCE.I not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0xa0002053)
{
	log::error << L"FEQ not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00003007)
{
	log::error << L"FLD not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0xa0000053)
{
	log::error << L"FLE not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0xa0001053)
{
	log::error << L"FLT not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00002007)
{
			auto f = parseFormatI(word);
			FR_u(f.rd) = (uint32_t)MEM_RD(R_u(f.rs1) + f.imm);
		
	return true;
}
else if ((word & 0x0600007f) == 0x00000043)
{
	log::error << L"FMADD not implemented." << Endl;
	return false;
}
else if ((word & 0x0600007f) == 0x00000047)
{
	log::error << L"FMSUB not implemented." << Endl;
	return false;
}
else if ((word & 0x0600007f) == 0x0000004f)
{
	log::error << L"FNMADD not implemented." << Endl;
	return false;
}
else if ((word & 0x0600007f) == 0x0000004b)
{
	log::error << L"FNMSUB not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x28000053)
{
	log::error << L"FMIN not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x28001053)
{
	log::error << L"FMAX not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00007f) == 0x10000053)
{
	log::error << L"FMUL not implemented." << Endl;
	return false;
}
else if ((word & 0xfff0707f) == 0xe0000053)
{
	log::error << L"FMV_X_W not implemented." << Endl;
	return false;
}
else if ((word & 0xfff0707f) == 0xf0000053)
{
	log::error << L"FMV_W_X not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00003027)
{
	log::error << L"FSD not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x20000053)
{
	log::error << L"FSGNJ not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x20001053)
{
	log::error << L"FSGNJN not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x20002053)
{
	log::error << L"FSGNJX not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00007f) == 0x08000053)
{
	log::error << L"FSUB not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00002027)
{
			auto f = parseFormatS(word);
			MEM_WR(R_u(f.rs1) + f.imm, FR_u(f.rs2));
		
	return true;
}
else if ((word & 0x0000007f) == 0x0000006f)
{
			auto f = parseFormatJ(word);
			R_u(f.rd) = PC_NEXT;
			PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00000067)
{
			auto f = parseFormatI(word);
			R_u(f.rd) = PC_NEXT;
			PC_NEXT = R(f.rs1) + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00000003)
{
			auto f = parseFormatI(word);
			R_s(f.rd) = (int8_t)MEM_RD_U8(R_u(f.rs1) + f.imm);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00004003)
{
			auto f = parseFormatI(word);
			R_u(f.rd) = MEM_RD_U8(R_u(f.rs1) + f.imm);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00003003)
{
	log::error << L"LD not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00001003)
{
			auto f = parseFormatI(word);
			R_s(f.rd) = (int16_t)MEM_RD_U16(R_u(f.rs1) + f.imm);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00005003)
{
			auto f = parseFormatI(word);
			R(f.rd) = MEM_RD_U16(R(f.rs1) + f.imm);			
		
	return true;
}
else if ((word & 0xf9f0707f) == 0x1000302f)
{
	log::error << L"LR.D not implemented." << Endl;
	return false;
}
else if ((word & 0xf9f0707f) == 0x1000202f)
{
	log::error << L"LR.W not implemented." << Endl;
	return false;
}
else if ((word & 0x0000007f) == 0x00000037)
{
			auto f = parseFormatU(word);
			R_s(f.rd) = f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00002003)
{
			auto f = parseFormatI(word);
			R_s(f.rd) = (int32_t)MEM_RD(R_u(f.rs1) + f.imm);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00006003)
{
	log::error << L"LWU not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x02000033)
{
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) * R_s(f.rs2);
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02001033)
{
			auto f = parseFormatR(word);
			int64_t lh = (int64_t)R_s(f.rs1);
			int64_t rh = (int64_t)R_s(f.rs2);
			R_s(f.rd) = (lh * rh) >> 32;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02003033)
{
			auto f = parseFormatR(word);
			uint64_t lh = (uint64_t)R(f.rs1);
			uint64_t rh = (uint64_t)R(f.rs2);
			R(f.rd) = (lh * rh) >> 32;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02002033)
{
	log::error << L"MULHSU not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x0200003b)
{
	log::error << L"MULW not implemented." << Endl;
	return false;
}
else if ((word & 0xffffffff) == 0x30200073)
{
			PC_NEXT = readCSR(CSR::MEPC);
			log::info << L"MRET " << str(L"%08x", PC_NEXT) << Endl;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x00006033)
{
			auto f = parseFormatR(word);
			R_u(f.rd) = R_u(f.rs1) | R_u(f.rs2);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00006013)
{
			auto f = parseFormatI(word);
			R_u(f.rd) = R_u(f.rs1) | f.imm;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02006033)
{
			auto f = parseFormatR(word);
			int32_t dividend = R_s(f.rs1);
			int32_t divisor = R_s(f.rs2);
			if (divisor == 0)
				R_s(f.rd) = dividend;
			else
				R_s(f.rd) = dividend % divisor;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02007033)
{
			auto f = parseFormatR(word);
			uint32_t dividend = R_u(f.rs1);
			uint32_t divisor = R_u(f.rs2);
			if (divisor == 0)
				R_u(f.rd) = dividend;
			else
				R_u(f.rd) = dividend % divisor;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x0200703b)
{
	log::error << L"REMUW not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x0200603b)
{
	log::error << L"REMW not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00000023)
{
			auto f = parseFormatS(word);
			MEM_WR_U8(R_u(f.rs1) + f.imm, (uint8_t)R_u(f.rs2));
		
	return true;
}
else if ((word & 0xf800707f) == 0x1800302f)
{
	log::error << L"SC.D not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x1800202f)
{
	log::error << L"SC.W not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00003023)
{
	log::error << L"SD not implemented." << Endl;
	return false;
}
else if ((word & 0xfe007fff) == 0x12000073)
{
	log::error << L"SFENCE.VMA not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00001023)
{
			auto f = parseFormatS(word);
			MEM_WR_U16(R_u(f.rs1) + f.imm, R_u(f.rs2));
		
	return true;
}
else if ((word & 0xfe00707f) == 0x00001033)
{
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) << R_u(f.rs2);
		
	return true;
}
else if ((word & 0xfc00707f) == 0x00001013)
{
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) << ((word >> 20) & 0x1f);
		
	return true;
}
else if ((word & 0xfe00707f) == 0x0000101b)
{
	log::error << L"SLLIW not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x0000103b)
{
	log::error << L"SLLW not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x00002033)
{
			auto f = parseFormatR(word);
			R_u(f.rd) = (R_s(f.rs1) < R_s(f.rs2)) ? 1 : 0;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00002013)
{
			auto f = parseFormatI(word);
			R_u(f.rd) = (R_s(f.rs1) < f.imm) ? 1 : 0;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00003013)
{
			auto f = parseFormatI(word);
			R_u(f.rd) = (R_u(f.rs1) < f.imm) ? 1 : 0;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x00003033)
{
			auto f = parseFormatR(word);
			R_u(f.rd) = (R_u(f.rs1) < R_u(f.rs2)) ? 1 : 0;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x40005033)
{
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) >> R_u(f.rs2);
		
	return true;
}
else if ((word & 0xfc00707f) == 0x40005013)
{
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) >> ((word >> 20) & 0x1f);
		
	return true;
}
else if ((word & 0xfc00707f) == 0x4000501b)
{
	log::error << L"SRAIW not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x4000503b)
{
	log::error << L"SRAW not implemented." << Endl;
	return false;
}
else if ((word & 0xffffffff) == 0x10200073)
{
	log::error << L"SRET not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x00005033)
{
			auto f = parseFormatR(word);
			R_u(f.rd) = R_u(f.rs1) >> R_u(f.rs2);
		
	return true;
}
else if ((word & 0xfc00707f) == 0x00005013)
{
			auto f = parseFormatR(word);
			R_u(f.rd) = R_u(f.rs1) >> ((word >> 20) & 0x1f);
		
	return true;
}
else if ((word & 0xfc00707f) == 0x0000501b)
{
	log::error << L"SRLIW not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x0000503b)
{
	log::error << L"SRLW not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x40000033)
{
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) - R_s(f.rs2);
		
	return true;
}
else if ((word & 0xfe00707f) == 0x4000003b)
{
	log::error << L"SUBW not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00002023)
{
			auto f = parseFormatS(word);
			MEM_WR(R_u(f.rs1) + f.imm, R_u(f.rs2));
		
	return true;
}
else if ((word & 0xffffffff) == 0x00200073)
{
	log::error << L"URET not implemented." << Endl;
	return false;
}
else if ((word & 0xffffffff) == 0x10500073)
{
			m_waitForInterrupt = true;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x00004033)
{
			auto f = parseFormatR(word);
			R_u(f.rd) = R_u(f.rs1) ^ R_u(f.rs2);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00004013)
{
			auto f = parseFormatI(word);
			R_u(f.rd) = R_u(f.rs1) ^ f.imm;
		
	return true;
}
else
{
	log::error << L"Unknown instruction." << Endl;
	return false;
}
