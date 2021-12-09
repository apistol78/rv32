if ((instruction & 32'hfe00707f) == 32'h00000033) begin
	`DISPLAY_OP("ADD");
			r[inst_B_rd] <= r[inst_B_rs1] + r[inst_B_rs2];
		
end
else if ((instruction & 32'h0000707f) == 32'h00000013) begin
	`DISPLAY_OP("ADDI not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h0000001b) begin
	`DISPLAY_OP("ADDIW not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0000003b) begin
	`DISPLAY_OP("ADDW not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h0000302f) begin
	`DISPLAY_OP("AMOADD.D not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h0000202f) begin
	`DISPLAY_OP("AMOADD.W not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h6000302f) begin
	`DISPLAY_OP("AMOAND.D not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h6000202f) begin
	`DISPLAY_OP("AMOAND.W not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'he000302f) begin
	`DISPLAY_OP("AMOMAXU.D not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'he000202f) begin
	`DISPLAY_OP("AMOMAXU.W not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h4000302f) begin
	`DISPLAY_OP("AMOOR.D not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h4000202f) begin
	`DISPLAY_OP("AMOOR.W not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h0800302f) begin
	`DISPLAY_OP("AMOSWAP.D not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h0800202f) begin
	`DISPLAY_OP("AMOSWAP.W not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h00007033) begin
	`DISPLAY_OP("AND not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00007013) begin
	`DISPLAY_OP("ANDI not implemented.");
end
else if ((instruction & 32'h0000007f) == 32'h00000017) begin
	`DISPLAY_OP("AUIPC not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00000063) begin
	`DISPLAY_OP("BEQ not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00005063) begin
	`DISPLAY_OP("BGE not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00007063) begin
	`DISPLAY_OP("BGEU not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00004063) begin
	`DISPLAY_OP("BLT not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00006063) begin
	`DISPLAY_OP("BLTU not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00001063) begin
	`DISPLAY_OP("BNE not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00003073) begin
	`DISPLAY_OP("CSRRC not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00007073) begin
	`DISPLAY_OP("CSRRCI not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00002073) begin
	`DISPLAY_OP("CSRRS not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00006073) begin
	`DISPLAY_OP("CSRRSI not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00001073) begin
	`DISPLAY_OP("CSRRW not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00005073) begin
	`DISPLAY_OP("CSRRWI not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h02004033) begin
	`DISPLAY_OP("DIV not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h02005033) begin
	`DISPLAY_OP("DIVU not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0200503b) begin
	`DISPLAY_OP("DIVUW not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0200403b) begin
	`DISPLAY_OP("DIVW not implemented.");
end
else if ((instruction & 32'hffffffff) == 32'h00100073) begin
	`DISPLAY_OP("EBREAK not implemented.");
end
else if ((instruction & 32'hffffffff) == 32'h00000073) begin
	`DISPLAY_OP("ECALL not implemented.");
end
else if ((instruction & 32'hfe00007f) == 32'h02000053) begin
	`DISPLAY_OP("FADD.D not implemented.");
end
else if ((instruction & 32'hfff0007f) == 32'hd2200053) begin
	`DISPLAY_OP("FCVT.D.L not implemented.");
end
else if ((instruction & 32'hfff0007f) == 32'h42000053) begin
	`DISPLAY_OP("FCVT.D.S not implemented.");
end
else if ((instruction & 32'hfff0007f) == 32'hd2000053) begin
	`DISPLAY_OP("FCVT.D.W not implemented.");
end
else if ((instruction & 32'hfff0007f) == 32'hd2100053) begin
	`DISPLAY_OP("FCVT.D.WU not implemented.");
end
else if ((instruction & 32'hfff0007f) == 32'h40100053) begin
	`DISPLAY_OP("FCVT.S.D not implemented.");
end
else if ((instruction & 32'hfff0007f) == 32'hc2000053) begin
	`DISPLAY_OP("FCVT.W.D not implemented.");
end
else if ((instruction & 32'hfe00007f) == 32'h1a000053) begin
	`DISPLAY_OP("FDIV.D not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h0000000f) begin
	`DISPLAY_OP("FENCE not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h0000100f) begin
	`DISPLAY_OP("FENCE.I not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'ha2002053) begin
	`DISPLAY_OP("FEQ.D not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00003007) begin
	`DISPLAY_OP("FLD not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'ha2000053) begin
	`DISPLAY_OP("FLE.D not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'ha2001053) begin
	`DISPLAY_OP("FLT.D not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00002007) begin
	`DISPLAY_OP("FLW not implemented.");
end
else if ((instruction & 32'h0600007f) == 32'h02000043) begin
	`DISPLAY_OP("FMADD.D not implemented.");
end
else if ((instruction & 32'hfe00007f) == 32'h12000053) begin
	`DISPLAY_OP("FMUL.D not implemented.");
end
else if ((instruction & 32'hfff0707f) == 32'hf2000053) begin
	`DISPLAY_OP("FMV.D.X not implemented.");
end
else if ((instruction & 32'hfff0707f) == 32'he2000053) begin
	`DISPLAY_OP("FMV.X.D not implemented.");
end
else if ((instruction & 32'hfff0707f) == 32'he0000053) begin
	`DISPLAY_OP("FMV.X.W not implemented.");
end
else if ((instruction & 32'hfff0707f) == 32'hf0000053) begin
	`DISPLAY_OP("FMV.W.X not implemented.");
end
else if ((instruction & 32'h0600007f) == 32'h0200004b) begin
	`DISPLAY_OP("FNMSUB.D not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00003027) begin
	`DISPLAY_OP("FSD not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h22000053) begin
	`DISPLAY_OP("FSGNJ.D not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h22002053) begin
	`DISPLAY_OP("FSGNJX.D not implemented.");
end
else if ((instruction & 32'hfe00007f) == 32'h0a000053) begin
	`DISPLAY_OP("FSUB.D not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00002027) begin
	`DISPLAY_OP("FSW not implemented.");
end
else if ((instruction & 32'h0000007f) == 32'h0000006f) begin
	`DISPLAY_OP("JAL not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00000067) begin
	`DISPLAY_OP("JALR not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00000003) begin
	`DISPLAY_OP("LB not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00004003) begin
	`DISPLAY_OP("LBU not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00003003) begin
	`DISPLAY_OP("LD not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00001003) begin
	`DISPLAY_OP("LH not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00005003) begin
	`DISPLAY_OP("LHU not implemented.");
end
else if ((instruction & 32'hf9f0707f) == 32'h1000302f) begin
	`DISPLAY_OP("LR.D not implemented.");
end
else if ((instruction & 32'hf9f0707f) == 32'h1000202f) begin
	`DISPLAY_OP("LR.W not implemented.");
end
else if ((instruction & 32'h0000007f) == 32'h00000037) begin
	`DISPLAY_OP("LUI not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00002003) begin
	`DISPLAY_OP("LW not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00006003) begin
	`DISPLAY_OP("LWU not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h02000033) begin
	`DISPLAY_OP("MUL not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h02001033) begin
	`DISPLAY_OP("MULH not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h02003033) begin
	`DISPLAY_OP("MULHU not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h02002033) begin
	`DISPLAY_OP("MULHSU not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0200003b) begin
	`DISPLAY_OP("MULW not implemented.");
end
else if ((instruction & 32'hffffffff) == 32'h30200073) begin
	`DISPLAY_OP("MRET not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h00006033) begin
	`DISPLAY_OP("OR not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00006013) begin
	`DISPLAY_OP("ORI not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h02006033) begin
	`DISPLAY_OP("REM not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h02007033) begin
	`DISPLAY_OP("REMU not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0200703b) begin
	`DISPLAY_OP("REMUW not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0200603b) begin
	`DISPLAY_OP("REMW not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00000023) begin
	`DISPLAY_OP("SB not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h1800302f) begin
	`DISPLAY_OP("SC.D not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h1800202f) begin
	`DISPLAY_OP("SC.W not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00003023) begin
	`DISPLAY_OP("SD not implemented.");
end
else if ((instruction & 32'hfe007fff) == 32'h12000073) begin
	`DISPLAY_OP("SFENCE.VMA not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00001023) begin
	`DISPLAY_OP("SH not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h00001033) begin
	`DISPLAY_OP("SLL not implemented.");
end
else if ((instruction & 32'hfc00707f) == 32'h00001013) begin
	`DISPLAY_OP("SLLI not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0000101b) begin
	`DISPLAY_OP("SLLIW not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0000103b) begin
	`DISPLAY_OP("SLLW not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h00002033) begin
	`DISPLAY_OP("SLT not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00002013) begin
	`DISPLAY_OP("SLTI not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00003013) begin
	`DISPLAY_OP("SLTIU not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h00003033) begin
	`DISPLAY_OP("SLTU not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h40005033) begin
	`DISPLAY_OP("SRA not implemented.");
end
else if ((instruction & 32'hfc00707f) == 32'h40005013) begin
	`DISPLAY_OP("SRAI not implemented.");
end
else if ((instruction & 32'hfc00707f) == 32'h4000501b) begin
	`DISPLAY_OP("SRAIW not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h4000503b) begin
	`DISPLAY_OP("SRAW not implemented.");
end
else if ((instruction & 32'hffffffff) == 32'h10200073) begin
	`DISPLAY_OP("SRET not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h00005033) begin
	`DISPLAY_OP("SRL not implemented.");
end
else if ((instruction & 32'hfc00707f) == 32'h00005013) begin
	`DISPLAY_OP("SRLI not implemented.");
end
else if ((instruction & 32'hfc00707f) == 32'h0000501b) begin
	`DISPLAY_OP("SRLIW not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0000503b) begin
	`DISPLAY_OP("SRLW not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h40000033) begin
	`DISPLAY_OP("SUB not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h4000003b) begin
	`DISPLAY_OP("SUBW not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00002023) begin
	`DISPLAY_OP("SW not implemented.");
end
else if ((instruction & 32'hffffffff) == 32'h00200073) begin
	`DISPLAY_OP("URET not implemented.");
end
else if ((instruction & 32'hffffffff) == 32'h10500073) begin
	`DISPLAY_OP("WFI not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h00004033) begin
	`DISPLAY_OP("XOR not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00004013) begin
	`DISPLAY_OP("XORI not implemented.");
end


