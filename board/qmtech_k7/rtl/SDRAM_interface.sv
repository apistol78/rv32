`timescale 1ns / 1ps

module SDRAM_interface(
	input i_reset,
	input i_clock,      // 100 MHz
	input i_clock_sdram,

	input i_request,
	input i_rw,
	input [31:0] i_address,
	input [31:0] i_wdata,
	output logic [31:0] o_rdata,
	output o_ready,
	
    // Chip
    inout [15:0] ddr3_dq,
    inout [1:0] ddr3_dqs_n,
    inout [1:0] ddr3_dqs_p,
    output [13:0] ddr3_addr,
    output [2:0] ddr3_ba,
    output ddr3_ras_n,
    output ddr3_cas_n,
    output ddr3_we_n,
    output ddr3_reset_n,
    output [0:0] ddr3_ck_p,
    output [0:0] ddr3_ck_n,
    output [0:0] ddr3_cke,
    output [0:0] ddr3_odt,
);
/*
    IP_SDRAM_DDR3 sdram_ddr3(
        .ddr3_dq(),
        .ddr3_dqs_n(),
        .ddr3_dqs_p(),
        .ddr3_addr(),
        .ddr3_ba(),
        .ddr3_ras_n(),
        .ddr3_cas_n(),
        .ddr3_we_n(),
        .ddr3_reset_n(),
        .ddr3_ck_p(),
        .ddr3_ck_n(),
        .ddr3_cke(),
        .ddr3_odt(),

.sys_clk_i,
.clk_ref_i,


  );*/

endmodule
