// Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2021.2 (lin64) Build 3367213 Tue Oct 19 02:47:39 MDT 2021
// Date        : Thu Apr 14 16:41:56 2022
// Host        : pn-conan-ws running 64-bit Pop!_OS 21.10
// Command     : write_verilog -force -mode synth_stub
//               /home/apistol/private/rv32/board/qmtech_k7/ip/IP_Clock/IP_Clock_stub.v
// Design      : IP_Clock
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7k325tffg676-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
module IP_Clock(clk_out1, clk_out2, clk_out3, reset, clk_in1)
/* synthesis syn_black_box black_box_pad_pin="clk_out1,clk_out2,clk_out3,reset,clk_in1" */;
  output clk_out1;
  output clk_out2;
  output clk_out3;
  input reset;
  input clk_in1;
endmodule
