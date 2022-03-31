//Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2021.2 (lin64) Build 3367213 Tue Oct 19 02:47:39 MDT 2021
//Date        : Thu Mar 31 18:41:59 2022
//Host        : pn-conan-ws running 64-bit Pop!_OS 21.10
//Command     : generate_target IP_Multiply.bd
//Design      : IP_Multiply
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CORE_GENERATION_INFO = "IP_Multiply,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=IP_Multiply,x_ipVersion=1.00.a,x_ipLanguage=VERILOG,numBlks=1,numReposBlks=1,numNonXlnxBlks=0,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=0,numPkgbdBlks=0,bdsource=USER,synth_mode=OOC_per_IP}" *) (* HW_HANDOFF = "IP_Multiply.hwdef" *) 
module IP_Multiply
   (clock,
    dataa,
    datab,
    result);
  input clock;
  input [31:0]dataa;
  input [31:0]datab;
  output [63:0]result;

  wire clock_1;
  wire [31:0]dataa_1;
  wire [31:0]datab_1;
  wire [63:0]mult_gen_0_P;

  assign clock_1 = clock;
  assign dataa_1 = dataa[31:0];
  assign datab_1 = datab[31:0];
  assign result[63:0] = mult_gen_0_P;
  IP_Multiply_mult_gen_0_0 mult_gen_0
       (.A(dataa_1),
        .B(datab_1),
        .CLK(clock_1),
        .P(mult_gen_0_P));
endmodule
