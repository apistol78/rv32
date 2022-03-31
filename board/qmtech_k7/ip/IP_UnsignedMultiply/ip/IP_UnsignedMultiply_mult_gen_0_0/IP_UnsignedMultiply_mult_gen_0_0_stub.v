// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2021.2 (lin64) Build 3367213 Tue Oct 19 02:47:39 MDT 2021
// Date        : Thu Mar 31 18:42:51 2022
// Host        : pn-conan-ws running 64-bit Pop!_OS 21.10
// Command     : write_verilog -force -mode synth_stub
//               /home/apistol/private/rv32/board/qmtech_k7/ip/IP_UnsignedMultiply/ip/IP_UnsignedMultiply_mult_gen_0_0/IP_UnsignedMultiply_mult_gen_0_0_stub.v
// Design      : IP_UnsignedMultiply_mult_gen_0_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7k325tffg676-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "mult_gen_v12_0_17,Vivado 2021.2" *)
module IP_UnsignedMultiply_mult_gen_0_0(CLK, A, B, P)
/* synthesis syn_black_box black_box_pad_pin="CLK,A[31:0],B[31:0],P[63:0]" */;
  input CLK;
  input [31:0]A;
  input [31:0]B;
  output [63:0]P;
endmodule
