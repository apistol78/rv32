if ((word & 0xfe00707f) == 0x00000033)
{
	TRACE(L"ADD");
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) + R_s(f.rs2);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00000013)
{
	TRACE(L"ADDI");
			auto f = parseFormatI(word);
			R_s(f.rd) = R_s(f.rs1) + f.imm;
        
	return true;
}
else if ((word & 0x0000707f) == 0x0000001b)
{
	TRACE(L"ADDIW");
			auto f = parseFormatI(word);
			R(f.rd) = R(f.rs1) + f.imm;
        
	return true;
}
else if ((word & 0xfe00707f) == 0x0000003b)
{
	TRACE(L"ADDW");
			auto f = parseFormatR(word);
			R(f.rd) = R(f.rs1) + R(f.rs2);
		
	return true;
}
else if ((word & 0xf800707f) == 0x0000302f)
{
	log::error << L"AMOADD.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x0000202f)
{
	log::error << L"AMOADD.W (not implemented)" << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x6000302f)
{
	log::error << L"AMOAND.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x6000202f)
{
	log::error << L"AMOAND.W (not implemented)" << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0xe000302f)
{
	log::error << L"AMOMAXU.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0xe000202f)
{
	log::error << L"AMOMAXU.W (not implemented)" << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x4000302f)
{
	log::error << L"AMOOR.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x4000202f)
{
	log::error << L"AMOOR.W (not implemented)" << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x0800302f)
{
	log::error << L"AMOSWAP.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x0800202f)
{
	log::error << L"AMOSWAP.W (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x00007033)
{
	TRACE(L"AND");
			auto f = parseFormatR(word);
			R_u(f.rd) = R_u(f.rs1) & R_u(f.rs2);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00007013)
{
	TRACE(L"ANDI");
			auto f = parseFormatI(word);
			R_u(f.rd) = R_u(f.rs1) & f.imm;
        
	return true;
}
else if ((word & 0x0000007f) == 0x00000017)
{
	TRACE(L"AUIPC");
			auto f = parseFormatU(word);
			R_u(f.rd) = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00000063)
{
	TRACE(L"BEQ");
			auto f = parseFormatB(word);
			if (R_s(f.rs1) == R_s(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00005063)
{
	TRACE(L"BGE");
			auto f = parseFormatB(word);
			if (R_s(f.rs1) >= R_s(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00007063)
{
	TRACE(L"BGEU");
			auto f = parseFormatB(word);
			if (R_u(f.rs1) >= R_u(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00004063)
{
	TRACE(L"BLT");
			auto f = parseFormatB(word);
			if (R_s(f.rs1) < R_s(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00006063)
{
	TRACE(L"BLTU");
			auto f = parseFormatB(word);
			if (R_u(f.rs1) < R_u(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00001063)
{
	TRACE(L"BNE");
			auto f = parseFormatB(word);
			if (R_s(f.rs1) != R_s(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00003073)
{
	log::error << L"CSRRC (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00007073)
{
	log::error << L"CSRRCI (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00002073)
{
	log::error << L"CSRRS (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00006073)
{
	log::error << L"CSRRSI (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00001073)
{
	log::error << L"CSRRW (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00005073)
{
	log::error << L"CSRRWI (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x02004033)
{
	TRACE(L"DIV");
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
	TRACE(L"DIVU");
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
	TRACE(L"DIVUW");
			auto f = parseFormatR(word);
			uint32_t dividend = R_u(f.rs1);
			uint32_t divisor = R_u(f.rs2);
			if (divisor ==  0)
				R_s(f.rd) = -1;
			else		
				R_u(f.rd) = dividend / divisor;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x0200403b)
{
	TRACE(L"DIVW");
			auto f = parseFormatR(word);
			R(f.rd) = R(f.rs1) / R(f.rs2);
		
	return true;
}
else if ((word & 0xffffffff) == 0x00100073)
{
	log::error << L"EBREAK (not implemented)" << Endl;
	return false;
}
else if ((word & 0xffffffff) == 0x00000073)
{
	TRACE(L"ECALL");
		
	return true;
}
else if ((word & 0xfe00007f) == 0x02000053)
{
	log::error << L"FADD.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfff0007f) == 0xd2200053)
{
	log::error << L"FCVT.D.L (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfff0007f) == 0x42000053)
{
	log::error << L"FCVT.D.S (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfff0007f) == 0xd2000053)
{
	log::error << L"FCVT.D.W (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfff0007f) == 0xd2100053)
{
	log::error << L"FCVT.D.WU (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfff0007f) == 0x40100053)
{
	log::error << L"FCVT.S.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfff0007f) == 0xc2000053)
{
	log::error << L"FCVT.W.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00007f) == 0x1a000053)
{
	log::error << L"FDIV.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x0000000f)
{
	log::error << L"FENCE (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x0000100f)
{
	log::error << L"FENCE.I (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0xa2002053)
{
	log::error << L"FEQ.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00003007)
{
	log::error << L"FLD (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0xa2000053)
{
	log::error << L"FLE.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0xa2001053)
{
	log::error << L"FLT.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00002007)
{
	log::error << L"FLW (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0600007f) == 0x02000043)
{
	log::error << L"FMADD.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00007f) == 0x12000053)
{
	log::error << L"FMUL.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfff0707f) == 0xf2000053)
{
	log::error << L"FMV.D.X (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfff0707f) == 0xe2000053)
{
	log::error << L"FMV.X.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfff0707f) == 0xe0000053)
{
	log::error << L"FMV.X.W (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfff0707f) == 0xf0000053)
{
	log::error << L"FMV.W.X (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0600007f) == 0x0200004b)
{
	log::error << L"FNMSUB.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00003027)
{
	log::error << L"FSD (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x22000053)
{
	log::error << L"FSGNJ.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x22002053)
{
	log::error << L"FSGNJX.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00007f) == 0x0a000053)
{
	log::error << L"FSUB.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00002027)
{
	log::error << L"FSW (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000007f) == 0x0000006f)
{
	TRACE(L"JAL");
			auto f = parseFormatJ(word);
			R_u(f.rd) = PC_NEXT;
			PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00000067)
{
	TRACE(L"JALR");
			auto f = parseFormatI(word);
			R_u(f.rd) = PC_NEXT;
			PC_NEXT = R(f.rs1) + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00000003)
{
	TRACE(L"LB");
			auto f = parseFormatI(word);
			R_s(f.rd) = (int8_t)MEM_RD_U8(R_u(f.rs1) + f.imm);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00004003)
{
	TRACE(L"LBU");
			auto f = parseFormatI(word);
			R_u(f.rd) = MEM_RD_U8(R_u(f.rs1) + f.imm);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00003003)
{
	log::error << L"LD (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00001003)
{
	TRACE(L"LH");
			auto f = parseFormatI(word);
			R_s(f.rd) = (int16_t)MEM_RD_U16(R_u(f.rs1) + f.imm);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00005003)
{
	TRACE(L"LHU");
			auto f = parseFormatI(word);
			R(f.rd) = MEM_RD_U16(R(f.rs1) + f.imm);			
		
	return true;
}
else if ((word & 0xf9f0707f) == 0x1000302f)
{
	log::error << L"LR.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xf9f0707f) == 0x1000202f)
{
	log::error << L"LR.W (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000007f) == 0x00000037)
{
	TRACE(L"LUI");
			auto f = parseFormatU(word);
			R_s(f.rd) = f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00002003)
{
	TRACE(L"LW");
			auto f = parseFormatI(word);
			R_s(f.rd) = (int32_t)MEM_RD(R_u(f.rs1) + f.imm);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00006003)
{
	TRACE(L"LWU");
			auto f = parseFormatI(word);
			R_u(f.rd) = MEM_RD(R_u(f.rs1) + f.imm);
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02000033)
{
	TRACE(L"MUL");
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) * R_s(f.rs2);
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02001033)
{
	TRACE(L"MULH");
			auto f = parseFormatR(word);
			int64_t lh = (int64_t)R_s(f.rs1);
			int64_t rh = (int64_t)R_s(f.rs2);
			R_s(f.rd) = (lh * rh) >> 32;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02003033)
{
	TRACE(L"MULHU");
			auto f = parseFormatR(word);
			uint64_t lh = (uint64_t)R(f.rs1);
			uint64_t rh = (uint64_t)R(f.rs2);
			R(f.rd) = (lh * rh) >> 32;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02002033)
{
	log::error << L"MULHSU (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x0200003b)
{
	log::error << L"MULW (not implemented)" << Endl;
	return false;
}
else if ((word & 0xffffffff) == 0x30200073)
{
	log::error << L"MRET (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x00006033)
{
	TRACE(L"OR");
			auto f = parseFormatR(word);
			R_u(f.rd) = R_u(f.rs1) | R_u(f.rs2);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00006013)
{
	TRACE(L"ORI");
			auto f = parseFormatI(word);
			R_u(f.rd) = R_u(f.rs1) | f.imm;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02006033)
{
	TRACE(L"REM");
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
	TRACE(L"REMU");
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
	log::error << L"REMUW (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x0200603b)
{
	log::error << L"REMW (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00000023)
{
	TRACE(L"SB");
			auto f = parseFormatS(word);
			MEM_WR_U8(R_u(f.rs1) + f.imm, (uint8_t)R_u(f.rs2));
		
	return true;
}
else if ((word & 0xf800707f) == 0x1800302f)
{
	log::error << L"SC.D (not implemented)" << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x1800202f)
{
	log::error << L"SC.W (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00003023)
{
	log::error << L"SD (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe007fff) == 0x12000073)
{
	log::error << L"SFENCE.VMA (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00001023)
{
	TRACE(L"SH");
			auto f = parseFormatS(word);
			MEM_WR_U16(R_u(f.rs1) + f.imm, R_u(f.rs2));
		
	return true;
}
else if ((word & 0xfe00707f) == 0x00001033)
{
	TRACE(L"SLL");
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) << R_u(f.rs2);
		
	return true;
}
else if ((word & 0xfc00707f) == 0x00001013)
{
	TRACE(L"SLLI");
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) << ((word >> 20) & 0x1f);
		
	return true;
}
else if ((word & 0xfe00707f) == 0x0000101b)
{
	log::error << L"SLLIW (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x0000103b)
{
	log::error << L"SLLW (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x00002033)
{
	TRACE(L"SLT");
			auto f = parseFormatR(word);
			R_u(f.rd) = (R_s(f.rs1) < R_s(f.rs2)) ? 1 : 0;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00002013)
{
	TRACE(L"SLTI");
			auto f = parseFormatI(word);
			R_u(f.rd) = (R_s(f.rs1) < f.imm) ? 1 : 0;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00003013)
{
	TRACE(L"SLTIU");
			auto f = parseFormatI(word);
			R_u(f.rd) = (R_u(f.rs1) < f.imm) ? 1 : 0;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x00003033)
{
	TRACE(L"SLTU");
			auto f = parseFormatR(word);
			R_u(f.rd) = (R_u(f.rs1) < R_u(f.rs2)) ? 1 : 0;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x40005033)
{
	TRACE(L"SRA");
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) >> R_u(f.rs2);
		
	return true;
}
else if ((word & 0xfc00707f) == 0x40005013)
{
	TRACE(L"SRAI");
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) >> ((word >> 20) & 0x1f);
		
	return true;
}
else if ((word & 0xfc00707f) == 0x4000501b)
{
	log::error << L"SRAIW (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x4000503b)
{
	log::error << L"SRAW (not implemented)" << Endl;
	return false;
}
else if ((word & 0xffffffff) == 0x10200073)
{
	log::error << L"SRET (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x00005033)
{
	TRACE(L"SRL");
			auto f = parseFormatR(word);
			R_u(f.rd) = R_u(f.rs1) >> R_u(f.rs2);
		
	return true;
}
else if ((word & 0xfc00707f) == 0x00005013)
{
	TRACE(L"SRLI");
			auto f = parseFormatR(word);
			R_u(f.rd) = R_u(f.rs1) >> ((word >> 20) & 0x1f);
		
	return true;
}
else if ((word & 0xfc00707f) == 0x0000501b)
{
	log::error << L"SRLIW (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x0000503b)
{
	log::error << L"SRLW (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x40000033)
{
	TRACE(L"SUB");
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) - R_s(f.rs2);
		
	return true;
}
else if ((word & 0xfe00707f) == 0x4000003b)
{
	log::error << L"SUBW (not implemented)" << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00002023)
{
	TRACE(L"SW");
			auto f = parseFormatS(word);
			MEM_WR(R_u(f.rs1) + f.imm, R_u(f.rs2));
		
	return true;
}
else if ((word & 0xffffffff) == 0x00200073)
{
	log::error << L"URET (not implemented)" << Endl;
	return false;
}
else if ((word & 0xffffffff) == 0x10500073)
{
	log::error << L"WFI (not implemented)" << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x00004033)
{
	TRACE(L"XOR");
			auto f = parseFormatR(word);
			R_u(f.rd) = R_u(f.rs1) ^ R_u(f.rs2);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00004013)
{
	TRACE(L"XORI");
			auto f = parseFormatI(word);
			R_u(f.rd) = R_u(f.rs1) ^ f.imm;
		
	return true;
}
else
{
	log::error << L"Unknown instruction." << Endl;
	return false;
}


