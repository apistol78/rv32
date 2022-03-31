-- Copyright 1986-2021 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2021.2 (lin64) Build 3367213 Tue Oct 19 02:47:39 MDT 2021
-- Date        : Thu Mar 31 18:42:51 2022
-- Host        : pn-conan-ws running 64-bit Pop!_OS 21.10
-- Command     : write_vhdl -force -mode synth_stub
--               /home/apistol/private/rv32/board/qmtech_k7/ip/IP_UnsignedMultiply/ip/IP_UnsignedMultiply_mult_gen_0_0/IP_UnsignedMultiply_mult_gen_0_0_stub.vhdl
-- Design      : IP_UnsignedMultiply_mult_gen_0_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7k325tffg676-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity IP_UnsignedMultiply_mult_gen_0_0 is
  Port ( 
    CLK : in STD_LOGIC;
    A : in STD_LOGIC_VECTOR ( 31 downto 0 );
    B : in STD_LOGIC_VECTOR ( 31 downto 0 );
    P : out STD_LOGIC_VECTOR ( 63 downto 0 )
  );

end IP_UnsignedMultiply_mult_gen_0_0;

architecture stub of IP_UnsignedMultiply_mult_gen_0_0 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "CLK,A[31:0],B[31:0],P[63:0]";
attribute x_core_info : string;
attribute x_core_info of stub : architecture is "mult_gen_v12_0_17,Vivado 2021.2";
begin
end;
