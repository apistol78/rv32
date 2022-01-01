
// SDRAM interface
module SDRAM_interface(
	input wire i_reset,
	input wire i_clock,
	input wire i_clock125,
	input wire i_request,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
	output wire o_ready,

	output wire [9:0] DDR2LP_CA,
	output wire [1:0] DDR2LP_CKE,
	output wire DDR2LP_CK_n,
	output wire DDR2LP_CK_p,
	output wire [1:0] DDR2LP_CS_n,
	output wire [3:0] DDR2LP_DM,
	inout wire [31:0] DDR2LP_DQ,
	inout wire [3:0] DDR2LP_DQS_n,
	inout wire [3:0] DDR2LP_DQS_p,
	input wire DDR2LP_OCT_RZQ
);

	IP_SDRAM sdram(
		.pll_ref_clk(i_clock),
		.global_reset_n(~i_reset),
		.soft_reset_n(~i_reset),
		
		.afi_clk(),                    //            afi_clk.clk
		.afi_half_clk(),               //       afi_half_clk.clk
		.afi_reset_n(),                //          afi_reset.reset_n
		.afi_reset_export_n(),

		.mem_ca(DD2LP_CA),
		.mem_cke(DDR2LP_CKE[0]),
		.mem_ck(DDR2LP_CK_p),
		.mem_ck_n(DDR2LP_CK_n),
		.mem_cs_n(DDR2LP_CS_n[0]),
		.mem_dm(DDR2LP_DM),
		.mem_dq(DDR2LP_DQ),
		.mem_dqs_n(DDR2LP_DQS_n),
		.mem_dqs(DDR2LP_DQS_p),
		.oct_rzqin(DDR2LP_OCT_RZQ),

		.avl_ready_0(o_ready),
		.avl_burstbegin_0(1'b0),
		.avl_addr_0(i_address[26:0]),
		.avl_rdata_0(o_rdata),
		.avl_wdata_0(i_wdata),
		.avl_read_req_0(i_request && !i_rw),
		.avl_write_req_0(i_request && i_rw),
		.avl_size_0(3'b1),
		.avl_be_0(4'hf),

		.mp_cmd_clk_0_clk(i_clock),
		.mp_cmd_reset_n_0_reset_n(~i_reset),
		.mp_rfifo_clk_0_clk(i_clock),
		.mp_rfifo_reset_n_0_reset_n(~i_reset),
		.mp_wfifo_clk_0_clk(i_clock),
		.mp_wfifo_reset_n_0_reset_n(~i_reset),
	);

endmodule