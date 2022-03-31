//Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2021.2 (lin64) Build 3367213 Tue Oct 19 02:47:39 MDT 2021
//Date        : Thu Mar 31 18:41:59 2022
//Host        : pn-conan-ws running 64-bit Pop!_OS 21.10
//Command     : generate_target IP_UnsignedMultiply_wrapper.bd
//Design      : IP_UnsignedMultiply_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module IP_UnsignedMultiply_wrapper
   (clock,
    dataa,
    datab,
    result);
  input clock;
  input [31:0]dataa;
  input [31:0]datab;
  output [63:0]result;

  wire clock;
  wire [31:0]dataa;
  wire [31:0]datab;
  wire [63:0]result;

  IP_UnsignedMultiply IP_UnsignedMultiply_i
       (.clock(clock),
        .dataa(dataa),
        .datab(datab),
        .result(result));
endmodule
