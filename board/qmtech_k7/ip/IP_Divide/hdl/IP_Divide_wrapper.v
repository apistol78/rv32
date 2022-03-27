//Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2021.2 (lin64) Build 3367213 Tue Oct 19 02:47:39 MDT 2021
//Date        : Sun Mar 27 13:33:56 2022
//Host        : pn-conan-ws running 64-bit Pop!_OS 21.10
//Command     : generate_target IP_Divide_wrapper.bd
//Design      : IP_Divide_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module IP_Divide_wrapper
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

  wire clock;
  wire [31:0]denom;
  wire [31:0]numer;
  wire [31:0]quotient;
  wire [31:0]remain;

  IP_Divide IP_Divide_i
       (.clock(clock),
        .denom(denom),
        .numer(numer),
        .quotient(quotient),
        .remain(remain));
endmodule
