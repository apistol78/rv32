//Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2021.2 (lin64) Build 3367213 Tue Oct 19 02:47:39 MDT 2021
//Date        : Sun Mar 27 13:39:09 2022
//Host        : pn-conan-ws running 64-bit Pop!_OS 21.10
//Command     : generate_target IP_UnsignedDivide.bd
//Design      : IP_UnsignedDivide
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CORE_GENERATION_INFO = "IP_UnsignedDivide,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=IP_UnsignedDivide,x_ipVersion=1.00.a,x_ipLanguage=VERILOG,numBlks=3,numReposBlks=3,numNonXlnxBlks=0,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=0,numPkgbdBlks=0,bdsource=USER,synth_mode=OOC_per_IP}" *) (* HW_HANDOFF = "IP_UnsignedDivide.hwdef" *) 
module IP_UnsignedDivide
   (clock,
    denom,
    numer,
    quotient,
    remain);
  input clock;
  input [31:0]denom;
  input [31:0]numer;
  output [31:0]quotient;
  output [31:0]remain;

  wire clock_1;
  wire [31:0]denom_1;
  wire [63:0]div_gen_0_m_axis_dout_tdata;
  wire [31:0]numer_1;
  wire [31:0]xlslice_0_Dout;
  wire [31:0]xlslice_1_Dout;

  assign clock_1 = clock;
  assign denom_1 = denom[31:0];
  assign numer_1 = numer[31:0];
  assign quotient[31:0] = xlslice_1_Dout;
  assign remain[31:0] = xlslice_0_Dout;
  IP_UnsignedDivide_div_gen_0_1 div_gen_0
       (.aclk(clock_1),
        .m_axis_dout_tdata(div_gen_0_m_axis_dout_tdata),
        .s_axis_dividend_tdata(numer_1),
        .s_axis_dividend_tvalid(1'b0),
        .s_axis_divisor_tdata(denom_1),
        .s_axis_divisor_tvalid(1'b0));
  IP_UnsignedDivide_xlslice_0_2 xlslice_0
       (.Din(div_gen_0_m_axis_dout_tdata),
        .Dout(xlslice_0_Dout));
  IP_UnsignedDivide_xlslice_1_0 xlslice_1
       (.Din(div_gen_0_m_axis_dout_tdata),
        .Dout(xlslice_1_Dout));
endmodule
