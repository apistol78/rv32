if ((word & 0xfe00707f) == 0x00000033)
{
	TRACE(L"	ADD");
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) + R_s(f.rs2);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00000013)
{
	TRACE(L"	ADDI");
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
			auto f = parseFormatI(word);
			R_s(f.rd) = R_s(f.rs1) + f.imm;
        
	return true;
}
else if ((word & 0x0000707f) == 0x0000001b)
{
	TRACE(L"	ADDIW");
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
			auto f = parseFormatI(word);
			R(f.rd) = R(f.rs1) + f.imm;
        
	return true;
}
else if ((word & 0xfe00707f) == 0x0000003b)
{
	TRACE(L"	ADDW");
			auto f = parseFormatR(word);
			R(f.rd) = R(f.rs1) + R(f.rs2);
		
	return true;
}
else if ((word & 0xf800707f) == 0x0000302f)
{
	TRACE(L"	AMOADD.D");
	log::info << L"No verilog implementation of AMOADD.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x0000202f)
{
	TRACE(L"	AMOADD.W");
	log::info << L"No verilog implementation of AMOADD.W" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x6000302f)
{
	TRACE(L"	AMOAND.D");
	log::info << L"No verilog implementation of AMOAND.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x6000202f)
{
	TRACE(L"	AMOAND.W");
	log::info << L"No verilog implementation of AMOAND.W" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0xe000302f)
{
	TRACE(L"	AMOMAXU.D");
	log::info << L"No verilog implementation of AMOMAXU.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0xe000202f)
{
	TRACE(L"	AMOMAXU.W");
	log::info << L"No verilog implementation of AMOMAXU.W" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x4000302f)
{
	TRACE(L"	AMOOR.D");
	log::info << L"No verilog implementation of AMOOR.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x4000202f)
{
	TRACE(L"	AMOOR.W");
	log::info << L"No verilog implementation of AMOOR.W" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x0800302f)
{
	TRACE(L"	AMOSWAP.D");
	log::info << L"No verilog implementation of AMOSWAP.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x0800202f)
{
	TRACE(L"	AMOSWAP.W");
	log::info << L"No verilog implementation of AMOSWAP.W" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x00007033)
{
	TRACE(L"	AND");
			auto f = parseFormatR(word);
			R_u(f.rd) = R_u(f.rs1) & R_u(f.rs2);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00007013)
{
	TRACE(L"	ANDI");
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
			auto f = parseFormatI(word);
			R_u(f.rd) = R_u(f.rs1) & f.imm;
        
	return true;
}
else if ((word & 0x0000007f) == 0x00000017)
{
	TRACE(L"	AUIPC");
			auto f = parseFormatU(word);
			R_u(f.rd) = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00000063)
{
	TRACE(L"	BEQ");
			auto f = parseFormatB(word);
			if (R_s(f.rs1) == R_s(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00005063)
{
	TRACE(L"	BGE");
			auto f = parseFormatB(word);
			if (R_s(f.rs1) >= R_s(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00007063)
{
	TRACE(L"	BGEU");
			auto f = parseFormatB(word);
			if (R_u(f.rs1) >= R_u(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00004063)
{
	TRACE(L"	BLT");
			auto f = parseFormatB(word);
			if (R_s(f.rs1) < R_s(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00006063)
{
	TRACE(L"	BLTU");
			auto f = parseFormatB(word);
			if (R_u(f.rs1) < R_u(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00001063)
{
	TRACE(L"	BNE");
			auto f = parseFormatB(word);
			if (R_s(f.rs1) != R_s(f.rs2))
				PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00003073)
{
	TRACE(L"	CSRRC");
	log::info << L"No verilog implementation of CSRRC" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00007073)
{
	TRACE(L"	CSRRCI");
	log::info << L"No verilog implementation of CSRRCI" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00002073)
{
	TRACE(L"	CSRRS");
	log::info << L"No verilog implementation of CSRRS" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00006073)
{
	TRACE(L"	CSRRSI");
	log::info << L"No verilog implementation of CSRRSI" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00001073)
{
	TRACE(L"	CSRRW");
	log::info << L"No verilog implementation of CSRRW" << Endl;
			auto f = parseFormatCSR(word);
			R_s(f.rd) = readCSR(f.csr);
			writeCSR(f.csr, R_s(f.rs1));
		
	return true;
}
else if ((word & 0x0000707f) == 0x00005073)
{
	TRACE(L"	CSRRWI");
	log::info << L"No verilog implementation of CSRRWI" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x02004033)
{
	TRACE(L"	DIV");
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
	TRACE(L"	DIVU");
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
	TRACE(L"	DIVUW");
	log::info << L"No verilog implementation of DIVUW" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x0200403b)
{
	TRACE(L"	DIVW");
	log::info << L"No verilog implementation of DIVW" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xffffffff) == 0x00100073)
{
	TRACE(L"	EBREAK");
			return false;
		
	return true;
}
else if ((word & 0xffffffff) == 0x00000073)
{
	TRACE(L"	ECALL");
		
	return true;
}
else if ((word & 0xfe00007f) == 0x02000053)
{
	TRACE(L"	FADD.D");
	log::info << L"No verilog implementation of FADD.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfff0007f) == 0xd2200053)
{
	TRACE(L"	FCVT.D.L");
	log::info << L"No verilog implementation of FCVT.D.L" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfff0007f) == 0x42000053)
{
	TRACE(L"	FCVT.D.S");
	log::info << L"No verilog implementation of FCVT.D.S" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfff0007f) == 0xd2000053)
{
	TRACE(L"	FCVT.D.W");
	log::info << L"No verilog implementation of FCVT.D.W" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfff0007f) == 0xd2100053)
{
	TRACE(L"	FCVT.D.WU");
	log::info << L"No verilog implementation of FCVT.D.WU" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfff0007f) == 0x40100053)
{
	TRACE(L"	FCVT.S.D");
	log::info << L"No verilog implementation of FCVT.S.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfff0007f) == 0xc2000053)
{
	TRACE(L"	FCVT.W.D");
	log::info << L"No verilog implementation of FCVT.W.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00007f) == 0x1a000053)
{
	TRACE(L"	FDIV.D");
	log::info << L"No verilog implementation of FDIV.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x0000000f)
{
	TRACE(L"	FENCE");
	log::info << L"No verilog implementation of FENCE" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x0000100f)
{
	TRACE(L"	FENCE.I");
	log::info << L"No verilog implementation of FENCE.I" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0xa2002053)
{
	TRACE(L"	FEQ.D");
	log::info << L"No verilog implementation of FEQ.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00003007)
{
	TRACE(L"	FLD");
	log::info << L"No verilog implementation of FLD" << Endl;
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0xa2000053)
{
	TRACE(L"	FLE.D");
	log::info << L"No verilog implementation of FLE.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0xa2001053)
{
	TRACE(L"	FLT.D");
	log::info << L"No verilog implementation of FLT.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00002007)
{
	TRACE(L"	FLW");
	log::info << L"No verilog implementation of FLW" << Endl;
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0600007f) == 0x02000043)
{
	TRACE(L"	FMADD.D");
	log::info << L"No verilog implementation of FMADD.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00007f) == 0x12000053)
{
	TRACE(L"	FMUL.D");
	log::info << L"No verilog implementation of FMUL.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfff0707f) == 0xf2000053)
{
	TRACE(L"	FMV.D.X");
	log::info << L"No verilog implementation of FMV.D.X" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfff0707f) == 0xe2000053)
{
	TRACE(L"	FMV.X.D");
	log::info << L"No verilog implementation of FMV.X.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfff0707f) == 0xe0000053)
{
	TRACE(L"	FMV.X.W");
	log::info << L"No verilog implementation of FMV.X.W" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfff0707f) == 0xf0000053)
{
	TRACE(L"	FMV.W.X");
	log::info << L"No verilog implementation of FMV.W.X" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0600007f) == 0x0200004b)
{
	TRACE(L"	FNMSUB.D");
	log::info << L"No verilog implementation of FNMSUB.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00003027)
{
	TRACE(L"	FSD");
	log::info << L"No verilog implementation of FSD" << Endl;
	{
			auto f = parseFormatS(word);
			TRACE(str(L"\tS: R(%d), R(%d), %d", f.rs1, f.rs2, f.imm));
		}
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x22000053)
{
	TRACE(L"	FSGNJ.D");
	log::info << L"No verilog implementation of FSGNJ.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x22002053)
{
	TRACE(L"	FSGNJX.D");
	log::info << L"No verilog implementation of FSGNJX.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00007f) == 0x0a000053)
{
	TRACE(L"	FSUB.D");
	log::info << L"No verilog implementation of FSUB.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00002027)
{
	TRACE(L"	FSW");
	log::info << L"No verilog implementation of FSW" << Endl;
	{
			auto f = parseFormatS(word);
			TRACE(str(L"\tS: R(%d), R(%d), %d", f.rs1, f.rs2, f.imm));
		}
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000007f) == 0x0000006f)
{
	TRACE(L"	JAL");
	{
			auto f = parseFormatJ(word);
			TRACE(str(L"\tJ: R(%d), imm %d", f.rd, f.imm));
		}
			auto f = parseFormatJ(word);
			R_u(f.rd) = PC_NEXT;
			PC_NEXT = PC + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00000067)
{
	TRACE(L"	JALR");
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
			auto f = parseFormatI(word);
			R_u(f.rd) = PC_NEXT;
			PC_NEXT = R(f.rs1) + f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00000003)
{
	TRACE(L"	LB");
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
			auto f = parseFormatI(word);
			R_s(f.rd) = (int8_t)MEM_RD_U8(R_u(f.rs1) + f.imm);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00004003)
{
	TRACE(L"	LBU");
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
			auto f = parseFormatI(word);
			R_u(f.rd) = MEM_RD_U8(R_u(f.rs1) + f.imm);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00003003)
{
	TRACE(L"	LD");
	log::info << L"No verilog implementation of LD" << Endl;
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00001003)
{
	TRACE(L"	LH");
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
			auto f = parseFormatI(word);
			R_s(f.rd) = (int16_t)MEM_RD_U16(R_u(f.rs1) + f.imm);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00005003)
{
	TRACE(L"	LHU");
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
			auto f = parseFormatI(word);
			R(f.rd) = MEM_RD_U16(R(f.rs1) + f.imm);			
		
	return true;
}
else if ((word & 0xf9f0707f) == 0x1000302f)
{
	TRACE(L"	LR.D");
	log::info << L"No verilog implementation of LR.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xf9f0707f) == 0x1000202f)
{
	TRACE(L"	LR.W");
	log::info << L"No verilog implementation of LR.W" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000007f) == 0x00000037)
{
	TRACE(L"	LUI");
			auto f = parseFormatU(word);
			R_s(f.rd) = f.imm;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00002003)
{
	TRACE(L"	LW");
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
			auto f = parseFormatI(word);
			R_s(f.rd) = (int32_t)MEM_RD(R_u(f.rs1) + f.imm);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00006003)
{
	TRACE(L"	LWU");
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
			auto f = parseFormatI(word);
			R_u(f.rd) = MEM_RD(R_u(f.rs1) + f.imm);
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02000033)
{
	TRACE(L"	MUL");
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) * R_s(f.rs2);
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02001033)
{
	TRACE(L"	MULH");
			auto f = parseFormatR(word);
			int64_t lh = (int64_t)R_s(f.rs1);
			int64_t rh = (int64_t)R_s(f.rs2);
			R_s(f.rd) = (lh * rh) >> 32;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02003033)
{
	TRACE(L"	MULHU");
			auto f = parseFormatR(word);
			uint64_t lh = (uint64_t)R(f.rs1);
			uint64_t rh = (uint64_t)R(f.rs2);
			R(f.rd) = (lh * rh) >> 32;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02002033)
{
	TRACE(L"	MULHSU");
	log::info << L"No verilog implementation of MULHSU" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x0200003b)
{
	TRACE(L"	MULW");
	log::info << L"No verilog implementation of MULW" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xffffffff) == 0x30200073)
{
	TRACE(L"	MRET");
	log::info << L"No verilog implementation of MRET" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x00006033)
{
	TRACE(L"	OR");
			auto f = parseFormatR(word);
			R_u(f.rd) = R_u(f.rs1) | R_u(f.rs2);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00006013)
{
	TRACE(L"	ORI");
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
			auto f = parseFormatI(word);
			R_u(f.rd) = R_u(f.rs1) | f.imm;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x02006033)
{
	TRACE(L"	REM");
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
	TRACE(L"	REMU");
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
	TRACE(L"	REMUW");
	log::info << L"No verilog implementation of REMUW" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x0200603b)
{
	TRACE(L"	REMW");
	log::info << L"No verilog implementation of REMW" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00000023)
{
	TRACE(L"	SB");
	{
			auto f = parseFormatS(word);
			TRACE(str(L"\tS: R(%d), R(%d), %d", f.rs1, f.rs2, f.imm));
		}
			auto f = parseFormatS(word);
			MEM_WR_U8(R_u(f.rs1) + f.imm, (uint8_t)R_u(f.rs2));
		
	return true;
}
else if ((word & 0xf800707f) == 0x1800302f)
{
	TRACE(L"	SC.D");
	log::info << L"No verilog implementation of SC.D" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xf800707f) == 0x1800202f)
{
	TRACE(L"	SC.W");
	log::info << L"No verilog implementation of SC.W" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00003023)
{
	TRACE(L"	SD");
	log::info << L"No verilog implementation of SD" << Endl;
	{
			auto f = parseFormatS(word);
			TRACE(str(L"\tS: R(%d), R(%d), %d", f.rs1, f.rs2, f.imm));
		}
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe007fff) == 0x12000073)
{
	TRACE(L"	SFENCE.VMA");
	log::info << L"No verilog implementation of SFENCE.VMA" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00001023)
{
	TRACE(L"	SH");
	{
			auto f = parseFormatS(word);
			TRACE(str(L"\tS: R(%d), R(%d), %d", f.rs1, f.rs2, f.imm));
		}
			auto f = parseFormatS(word);
			MEM_WR_U16(R_u(f.rs1) + f.imm, R_u(f.rs2));
		
	return true;
}
else if ((word & 0xfe00707f) == 0x00001033)
{
	TRACE(L"	SLL");
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) << R_u(f.rs2);
		
	return true;
}
else if ((word & 0xfc00707f) == 0x00001013)
{
	TRACE(L"	SLLI");
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) << ((word >> 20) & 0x1f);
		
	return true;
}
else if ((word & 0xfe00707f) == 0x0000101b)
{
	TRACE(L"	SLLIW");
	log::info << L"No verilog implementation of SLLIW" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x0000103b)
{
	TRACE(L"	SLLW");
	log::info << L"No verilog implementation of SLLW" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x00002033)
{
	TRACE(L"	SLT");
			auto f = parseFormatR(word);
			R_u(f.rd) = (R_s(f.rs1) < R_s(f.rs2)) ? 1 : 0;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00002013)
{
	TRACE(L"	SLTI");
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
			auto f = parseFormatI(word);
			R_u(f.rd) = (R_s(f.rs1) < f.imm) ? 1 : 0;
		
	return true;
}
else if ((word & 0x0000707f) == 0x00003013)
{
	TRACE(L"	SLTIU");
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
			auto f = parseFormatI(word);
			R_u(f.rd) = (R_u(f.rs1) < f.imm) ? 1 : 0;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x00003033)
{
	TRACE(L"	SLTU");
			auto f = parseFormatR(word);
			R_u(f.rd) = (R_u(f.rs1) < R_u(f.rs2)) ? 1 : 0;
		
	return true;
}
else if ((word & 0xfe00707f) == 0x40005033)
{
	TRACE(L"	SRA");
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) >> R_u(f.rs2);
		
	return true;
}
else if ((word & 0xfc00707f) == 0x40005013)
{
	TRACE(L"	SRAI");
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) >> ((word >> 20) & 0x1f);
		
	return true;
}
else if ((word & 0xfc00707f) == 0x4000501b)
{
	TRACE(L"	SRAIW");
	log::info << L"No verilog implementation of SRAIW" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x4000503b)
{
	TRACE(L"	SRAW");
	log::info << L"No verilog implementation of SRAW" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xffffffff) == 0x10200073)
{
	TRACE(L"	SRET");
	log::info << L"No verilog implementation of SRET" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x00005033)
{
	TRACE(L"	SRL");
			auto f = parseFormatR(word);
			R_u(f.rd) = R_u(f.rs1) >> R_u(f.rs2);
		
	return true;
}
else if ((word & 0xfc00707f) == 0x00005013)
{
	TRACE(L"	SRLI");
			auto f = parseFormatR(word);
			R_u(f.rd) = R_u(f.rs1) >> ((word >> 20) & 0x1f);
		
	return true;
}
else if ((word & 0xfc00707f) == 0x0000501b)
{
	TRACE(L"	SRLIW");
	log::info << L"No verilog implementation of SRLIW" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x0000503b)
{
	TRACE(L"	SRLW");
	log::info << L"No verilog implementation of SRLW" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x40000033)
{
	TRACE(L"	SUB");
			auto f = parseFormatR(word);
			R_s(f.rd) = R_s(f.rs1) - R_s(f.rs2);
		
	return true;
}
else if ((word & 0xfe00707f) == 0x4000003b)
{
	TRACE(L"	SUBW");
	log::info << L"No verilog implementation of SUBW" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0x0000707f) == 0x00002023)
{
	TRACE(L"	SW");
	{
			auto f = parseFormatS(word);
			TRACE(str(L"\tS: R(%d), R(%d), %d", f.rs1, f.rs2, f.imm));
		}
			auto f = parseFormatS(word);
			MEM_WR(R_u(f.rs1) + f.imm, R_u(f.rs2));
		
	return true;
}
else if ((word & 0xffffffff) == 0x00200073)
{
	TRACE(L"	URET");
	log::info << L"No verilog implementation of URET" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xffffffff) == 0x10500073)
{
	TRACE(L"	WFI");
	log::info << L"No verilog implementation of WFI" << Endl;
	log::error << L"Not implemented." << Endl;
	return false;
}
else if ((word & 0xfe00707f) == 0x00004033)
{
	TRACE(L"	XOR");
			auto f = parseFormatR(word);
			R_u(f.rd) = R_u(f.rs1) ^ R_u(f.rs2);
		
	return true;
}
else if ((word & 0x0000707f) == 0x00004013)
{
	TRACE(L"	XORI");
	{
			auto f = parseFormatI(word);
			TRACE(str(L"\tI: R(%d) = R(%d) op %d", f.rd, f.rs1, f.imm));
			TRACE(str(L"\t   R(%d) = %08x", f.rs1, R_s(f.rs1)));
		}
			auto f = parseFormatI(word);
			R_u(f.rd) = R_u(f.rs1) ^ f.imm;
		
	return true;
}
else
{
	log::error << L"Unknown instruction." << Endl;
	return false;
}
