if ((instruction & 32'hfe00707f) == 32'h00000033) begin
	$display("ADD");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
			r[inst_R_rd] <= r[inst_R_rs1] + r[inst_R_rs2];
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00000013) begin
	$display("ADDI");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
			r[inst_I_rd] <= r[inst_I_rs1] + $signed(inst_I_imm);
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h0000001b) begin
	$display("ADDIW");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
			r[inst_I_rd] <= r[inst_I_rs1] + inst_I_imm;
			`DECODE_DONE;
		
end
else if ((instruction & 32'hfe00707f) == 32'h0000003b) begin
	$display("ADDW");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
			r[inst_R_rd] <= r[inst_R_rs1] + r[inst_R_rs2];
			`DECODE_DONE;
		
end
else if ((instruction & 32'hf800707f) == 32'h0000302f) begin
	$display("AMOADD.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h0000202f) begin
	$display("AMOADD.W");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h6000302f) begin
	$display("AMOAND.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h6000202f) begin
	$display("AMOAND.W");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'he000302f) begin
	$display("AMOMAXU.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'he000202f) begin
	$display("AMOMAXU.W");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h4000302f) begin
	$display("AMOOR.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h4000202f) begin
	$display("AMOOR.W");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h0800302f) begin
	$display("AMOSWAP.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h0800202f) begin
	$display("AMOSWAP.W");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h00007033) begin
	$display("AND");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
			r[inst_R_rd] <= r[inst_R_rs1] & r[inst_R_rs2];
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00007013) begin
	$display("ANDI");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
			r[inst_I_rd] <= r[inst_I_rs1] & inst_I_imm;
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000007f) == 32'h00000017) begin
	$display("AUIPC");
			$display("U: R(%d), imm %d (%d)", inst_U_rd, inst_U_imm, $signed(inst_U_imm));
		
			r[inst_U_rd] <= pc + $signed(inst_U_imm);
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00000063) begin
	$display("BEQ");
			$display("B: R(%d), R(%d), %d (%d)", inst_B_rs1, inst_B_rs2, inst_B_imm, $signed(inst_B_imm));
		
			if (r[inst_B_rs1] == r[inst_B_rs2])
				pc_next <= pc + $signed(inst_B_imm);
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00005063) begin
	$display("BGE");
			$display("B: R(%d), R(%d), %d (%d)", inst_B_rs1, inst_B_rs2, inst_B_imm, $signed(inst_B_imm));
		
			if ($signed(r[inst_B_rs1]) >= $signed(r[inst_B_rs2]))
				pc_next <= pc + $signed(inst_B_imm);
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00007063) begin
	$display("BGEU");
			$display("B: R(%d), R(%d), %d (%d)", inst_B_rs1, inst_B_rs2, inst_B_imm, $signed(inst_B_imm));
		
			if (r[inst_B_rs1] >= r[inst_B_rs2])
				pc_next <= pc + $signed(inst_B_imm);
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00004063) begin
	$display("BLT");
			$display("B: R(%d), R(%d), %d (%d)", inst_B_rs1, inst_B_rs2, inst_B_imm, $signed(inst_B_imm));
		
			if ($signed(r[inst_B_rs1]) < $signed(r[inst_B_rs2]))
				pc_next <= pc + $signed(inst_B_imm);
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00006063) begin
	$display("BLTU");
			$display("B: R(%d), R(%d), %d (%d)", inst_B_rs1, inst_B_rs2, inst_B_imm, $signed(inst_B_imm));
		
			if (r[inst_B_rs1] >= r[inst_B_rs2])
				pc_next <= pc + $signed(inst_B_imm);
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00001063) begin
	$display("BNE");
			$display("B: R(%d), R(%d), %d (%d)", inst_B_rs1, inst_B_rs2, inst_B_imm, $signed(inst_B_imm));
		
			if (r[inst_B_rs1] != r[inst_B_rs2])
				pc_next <= pc + $signed(inst_B_imm);
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00003073) begin
	$display("CSRRC");
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00007073) begin
	$display("CSRRCI");
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00002073) begin
	$display("CSRRS");
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00006073) begin
	$display("CSRRSI");
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00001073) begin
	$display("CSRRW");
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00005073) begin
	$display("CSRRWI");
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h02004033) begin
	$display("DIV");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h02005033) begin
	$display("DIVU");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0200503b) begin
	$display("DIVUW");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0200403b) begin
	$display("DIVW");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hffffffff) == 32'h00100073) begin
	$display("EBREAK");
	$display("Not implemented.");
end
else if ((instruction & 32'hffffffff) == 32'h00000073) begin
	$display("ECALL");
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00007f) == 32'h02000053) begin
	$display("FADD.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfff0007f) == 32'hd2200053) begin
	$display("FCVT.D.L");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfff0007f) == 32'h42000053) begin
	$display("FCVT.D.S");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfff0007f) == 32'hd2000053) begin
	$display("FCVT.D.W");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfff0007f) == 32'hd2100053) begin
	$display("FCVT.D.WU");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfff0007f) == 32'h40100053) begin
	$display("FCVT.S.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfff0007f) == 32'hc2000053) begin
	$display("FCVT.W.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00007f) == 32'h1a000053) begin
	$display("FDIV.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h0000000f) begin
	$display("FENCE");
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h0000100f) begin
	$display("FENCE.I");
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'ha2002053) begin
	$display("FEQ.D");
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00003007) begin
	$display("FLD");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'ha2000053) begin
	$display("FLE.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'ha2001053) begin
	$display("FLT.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00002007) begin
	$display("FLW");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
	$display("Not implemented.");
end
else if ((instruction & 32'h0600007f) == 32'h02000043) begin
	$display("FMADD.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00007f) == 32'h12000053) begin
	$display("FMUL.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfff0707f) == 32'hf2000053) begin
	$display("FMV.D.X");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfff0707f) == 32'he2000053) begin
	$display("FMV.X.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfff0707f) == 32'he0000053) begin
	$display("FMV.X.W");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfff0707f) == 32'hf0000053) begin
	$display("FMV.W.X");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'h0600007f) == 32'h0200004b) begin
	$display("FNMSUB.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00003027) begin
	$display("FSD");
			$display("S: R(%d), R(%d), %d (%d)", inst_S_rs1, inst_S_rs2, inst_S_imm, $signed(inst_S_imm));
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h22000053) begin
	$display("FSGNJ.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h22002053) begin
	$display("FSGNJX.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00007f) == 32'h0a000053) begin
	$display("FSUB.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00002027) begin
	$display("FSW");
			$display("S: R(%d), R(%d), %d (%d)", inst_S_rs1, inst_S_rs2, inst_S_imm, $signed(inst_S_imm));
		
	$display("Not implemented.");
end
else if ((instruction & 32'h0000007f) == 32'h0000006f) begin
	$display("JAL");
			$display("J: R(%d), imm %d (%d)", inst_J_rd, inst_J_imm, $signed(inst_J_imm));
		
			r[inst_J_rd] <= pc_next;
			pc_next <= pc + $signed(inst_J_imm);
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00000067) begin
	$display("JALR");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
			r[inst_I_rd] <= pc_next;
			pc_next <= r[inst_I_rs1] + $signed(inst_I_imm);
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00000003) begin
	$display("LB");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
			if (decode_step == 0) begin
				o_address <= r[inst_I_rs1] + inst_I_imm;
				o_rw <= 0;
				o_request <= 1;				
				decode_step <= 1;
			end
			else if (decode_step == 1) begin
				if (i_ready) begin
					r[inst_I_rd] <= { { 24{ i_data[7] } }, i_data[6:0] };
					o_request <= 0;
					`DECODE_DONE;
				end
			end
		
end
else if ((instruction & 32'h0000707f) == 32'h00004003) begin
	$display("LBU");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
			if (decode_step == 0) begin
				o_address <= r[inst_I_rs1] + inst_I_imm;
				o_rw <= 0;
				o_request <= 1;				
				decode_step <= 1;
			end
			else if (decode_step == 1) begin
				if (i_ready) begin
					r[inst_I_rd] <= i_data[7:0];
					o_request <= 0;
					`DECODE_DONE;
				end
			end
		
end
else if ((instruction & 32'h0000707f) == 32'h00003003) begin
	$display("LD");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00001003) begin
	$display("LH");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
			if (decode_step == 0) begin
				o_address <= r[inst_I_rs1] + inst_I_imm;
				o_rw <= 0;
				o_request <= 1;				
				decode_step <= 1;
			end
			else if (decode_step == 1) begin
				if (i_ready) begin
					r[inst_I_rd] <= { { 16{ i_data[15] } }, i_data[14:0] };
					o_request <= 0;
					`DECODE_DONE;
				end
			end
		
end
else if ((instruction & 32'h0000707f) == 32'h00005003) begin
	$display("LHU");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
			if (decode_step == 0) begin
				o_address <= r[inst_I_rs1] + inst_I_imm;
				o_rw <= 0;
				o_request <= 1;				
				decode_step <= 1;
			end
			else if (decode_step == 1) begin
				if (i_ready) begin
					r[inst_I_rd] <= i_data[15:0];
					o_request <= 0;
					`DECODE_DONE;
				end
			end
		
end
else if ((instruction & 32'hf9f0707f) == 32'h1000302f) begin
	$display("LR.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hf9f0707f) == 32'h1000202f) begin
	$display("LR.W");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'h0000007f) == 32'h00000037) begin
	$display("LUI");
			$display("U: R(%d), imm %d (%d)", inst_U_rd, inst_U_imm, $signed(inst_U_imm));
		
			r[inst_U_rd] <= inst_U_imm;
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00002003) begin
	$display("LW");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
			if (decode_step == 0) begin
				$display("LOAD WORD, addr %x", r[inst_I_rs1] + $signed(inst_I_imm));
				o_address <= r[inst_I_rs1] + $signed(inst_I_imm);
				o_rw <= 0;
				o_request <= 1;				
				decode_step <= 1;
			end
			else if (decode_step == 1) begin
				if (i_ready) begin
					$display("LOAD WORD, data %x", i_data);
					r[inst_I_rd] <= i_data;
					o_request <= 0;
					`DECODE_DONE;
				end
			end
		
end
else if ((instruction & 32'h0000707f) == 32'h00006003) begin
	$display("LWU");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
			if (decode_step == 0) begin
				o_address <= r[inst_I_rs1] + inst_I_imm;
				o_rw <= 0;
				o_request <= 1;				
				decode_step <= 1;
			end
			else if (decode_step == 1) begin
				if (i_ready) begin
					r[inst_I_rd] <= i_data[31:0];
					o_request <= 0;
					`DECODE_DONE;
				end
			end
		
end
else if ((instruction & 32'hfe00707f) == 32'h02000033) begin
	$display("MUL");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
			r[inst_R_rd] <= $signed(r[inst_R_rs1]) * $signed(r[inst_R_rs2]);
			`DECODE_DONE;
		
end
else if ((instruction & 32'hfe00707f) == 32'h02001033) begin
	$display("MULH");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h02003033) begin
	$display("MULHU");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h02002033) begin
	$display("MULHSU");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0200003b) begin
	$display("MULW");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hffffffff) == 32'h30200073) begin
	$display("MRET");
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h00006033) begin
	$display("OR");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
			r[inst_R_rd] <= r[inst_R_rs1] | r[inst_R_rs2];
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00006013) begin
	$display("ORI");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
			r[inst_I_rd] <= r[inst_I_rs1] | inst_I_imm;
			`DECODE_DONE;
		
end
else if ((instruction & 32'hfe00707f) == 32'h02006033) begin
	$display("REM");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h02007033) begin
	$display("REMU");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0200703b) begin
	$display("REMUW");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0200603b) begin
	$display("REMW");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00000023) begin
	$display("SB");
			$display("S: R(%d), R(%d), %d (%d)", inst_S_rs1, inst_S_rs2, inst_S_imm, $signed(inst_S_imm));
		
			if (decode_step == 0) begin
				o_address <= r[inst_S_rs1] + $signed(inst_S_imm);
				o_rw <= 0;
				o_request <= 1;				
				decode_step <= 1;
			end
			else if (decode_step == 1) begin
				if (i_ready) begin
					o_data <= { i_data[31:8], r[inst_S_rs2][7:0] };
					o_rw <= 1;
					o_request <= 1;
					decode_step <= 2;
				end
			end
			else if (decode_step == 2) begin
				if (i_ready) begin
					o_request <= 0;
					`DECODE_DONE;
				end
			end
		
end
else if ((instruction & 32'hf800707f) == 32'h1800302f) begin
	$display("SC.D");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hf800707f) == 32'h1800202f) begin
	$display("SC.W");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00003023) begin
	$display("SD");
			$display("S: R(%d), R(%d), %d (%d)", inst_S_rs1, inst_S_rs2, inst_S_imm, $signed(inst_S_imm));
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe007fff) == 32'h12000073) begin
	$display("SFENCE.VMA");
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00001023) begin
	$display("SH");
			$display("S: R(%d), R(%d), %d (%d)", inst_S_rs1, inst_S_rs2, inst_S_imm, $signed(inst_S_imm));
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h00001033) begin
	$display("SLL");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
			r[inst_R_rd] <= r[inst_R_rs1] << r[inst_R_rs2];
			`DECODE_DONE;
		
end
else if ((instruction & 32'hfc00707f) == 32'h00001013) begin
	$display("SLLI");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
			r[inst_R_rd] <= r[inst_R_rs1] << instruction[25:20];
			`DECODE_DONE;
		
end
else if ((instruction & 32'hfe00707f) == 32'h0000101b) begin
	$display("SLLIW");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0000103b) begin
	$display("SLLW");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h00002033) begin
	$display("SLT");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
			r[inst_R_rd] <= ($signed(r[inst_R_rs1]) < $signed(r[inst_R_rs2])) ? 1 : 0;
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00002013) begin
	$display("SLTI");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
			r[inst_I_rd] <= ($signed(r[inst_I_rs1]) < $signed(inst_I_imm)) ? 1 : 0;
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00003013) begin
	$display("SLTIU");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
			r[inst_I_rd] <= (r[inst_I_rs1] < inst_I_imm) ? 1 : 0;
			`DECODE_DONE;
		
end
else if ((instruction & 32'hfe00707f) == 32'h00003033) begin
	$display("SLTU");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
			r[inst_R_rd] <= (r[inst_R_rs1] < r[inst_R_rs2]) ? 1 : 0;
			`DECODE_DONE;
		
end
else if ((instruction & 32'hfe00707f) == 32'h40005033) begin
	$display("SRA");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfc00707f) == 32'h40005013) begin
	$display("SRAI");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
			r[inst_R_rd] <= r[inst_R_rs1] >> instruction[25:20];
			`DECODE_DONE;
		
end
else if ((instruction & 32'hfc00707f) == 32'h4000501b) begin
	$display("SRAIW");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h4000503b) begin
	$display("SRAW");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hffffffff) == 32'h10200073) begin
	$display("SRET");
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h00005033) begin
	$display("SRL");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfc00707f) == 32'h00005013) begin
	$display("SRLI");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfc00707f) == 32'h0000501b) begin
	$display("SRLIW");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h0000503b) begin
	$display("SRLW");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h40000033) begin
	$display("SUB");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
			r[inst_R_rd] <= $signed(r[inst_R_rs1]) - $signed(r[inst_R_rs2]);
			`DECODE_DONE;
		
end
else if ((instruction & 32'hfe00707f) == 32'h4000003b) begin
	$display("SUBW");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
	$display("Not implemented.");
end
else if ((instruction & 32'h0000707f) == 32'h00002023) begin
	$display("SW");
			$display("S: R(%d), R(%d), %d (%d)", inst_S_rs1, inst_S_rs2, inst_S_imm, $signed(inst_S_imm));
		
			if (decode_step == 0) begin
				o_address <= r[inst_S_rs1] + $signed(inst_S_imm);
				o_data <= r[inst_S_rs2];
				o_rw <= 0;
				o_request <= 1;				
				decode_step <= 1;
			end
			else if (decode_step == 1) begin
				if (i_ready) begin
					o_request <= 0;
					`DECODE_DONE;
				end
			end			
		
end
else if ((instruction & 32'hffffffff) == 32'h00200073) begin
	$display("URET");
	$display("Not implemented.");
end
else if ((instruction & 32'hffffffff) == 32'h10500073) begin
	$display("WFI");
	$display("Not implemented.");
end
else if ((instruction & 32'hfe00707f) == 32'h00004033) begin
	$display("XOR");
			$display("R: R(%d) = R(%d) op R(%d)", inst_R_rd, inst_R_rs1, inst_R_rs2);
		
			r[inst_R_rd] <= r[inst_R_rs1] ^ r[inst_R_rs2];
			`DECODE_DONE;
		
end
else if ((instruction & 32'h0000707f) == 32'h00004013) begin
	$display("XORI");
			$display("I: R(%d) = R(%d) op %d (%d)", inst_I_rd, inst_I_rs1, inst_I_imm, $signed(inst_I_imm));
		
			r[inst_I_rd] <= r[inst_I_rs1] ^ inst_I_imm;
			`DECODE_DONE;
		
end
else begin
	$display("Unknown instruction.");
end
