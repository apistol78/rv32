/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

// SDRAM interface
module SDRAM_interface(
	input wire i_global_reset_n,
	input wire i_soft_reset_n,
	input wire i_clock_pll_ref,
	
	input wire i_reset,
	input wire i_clock,
	input wire i_request,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
	output reg o_ready,

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

	wire avl_ready;
	wire avl_rdata_valid;
	reg avl_read_req = 0;
	reg avl_write_req = 0;
	wire avl_burstbegin;
	wire local_init_done;
	wire local_cal_success;
	wire pll_locked;
	
	assign DDR2LP_CKE[1] = 1'b0;
	assign DDR2LP_CS_n[1] = 1'b0;
	
	IP_SDRAM sdram(
		.pll_ref_clk(i_clock_pll_ref),
		.global_reset_n(i_global_reset_n),
		.soft_reset_n(i_soft_reset_n),
		
		.afi_clk(),
		.afi_half_clk(),
		.afi_reset_n(),
		.afi_reset_export_n(),

		.mem_ca(DDR2LP_CA),
		.mem_cke(DDR2LP_CKE[0]),
		.mem_ck(DDR2LP_CK_p),
		.mem_ck_n(DDR2LP_CK_n),
		.mem_cs_n(DDR2LP_CS_n[0]),
		.mem_dm(DDR2LP_DM),
		.mem_dq(DDR2LP_DQ),
		.mem_dqs_n(DDR2LP_DQS_n),
		.mem_dqs(DDR2LP_DQS_p),
		.oct_rzqin(DDR2LP_OCT_RZQ),

		.avl_ready_0(avl_ready),
		.avl_burstbegin_0(avl_burstbegin),
		.avl_addr_0(i_address[28:2]),
		.avl_rdata_valid_0(avl_rdata_valid),
		.avl_rdata_0(o_rdata),
		.avl_wdata_0(i_wdata),
		.avl_read_req_0(avl_read_req),
		.avl_write_req_0(avl_write_req),
		.avl_size_0(3'b001),
		.avl_be_0(4'hf),

		.mp_cmd_clk_0_clk(i_clock),
		.mp_cmd_reset_n_0_reset_n(i_soft_reset_n),
		.mp_rfifo_clk_0_clk(i_clock),
		.mp_rfifo_reset_n_0_reset_n(i_soft_reset_n),
		.mp_wfifo_clk_0_clk(i_clock),
		.mp_wfifo_reset_n_0_reset_n(i_soft_reset_n),
		
		.local_init_done(local_init_done),
		.local_cal_success(local_cal_success),
		
		.pll_locked(pll_locked)
	);
	
	reg [3:0] state = 0;
	reg [3:0] next_state = 0;
	
	assign avl_burstbegin = avl_read_req || avl_write_req;
	
	always @(posedge i_clock) begin
		if (i_reset) begin
			state <= 0;
		end
		else begin
			state <= next_state;
		end
	end

	always @(*) begin

		next_state = state;

		avl_read_req = 0;
		avl_write_req = 0;

		o_ready = 0;

		case (state)
			0: begin
				if (i_request && local_init_done) begin
					if (!i_rw)
						avl_read_req = 1;
					else
						avl_write_req = 1;

					next_state = 1;
				end
			end

			1: begin
				if (!i_rw)
					avl_read_req = 1;
				else
					avl_write_req = 1;

				if (avl_ready) begin
					avl_read_req = 0;
					avl_write_req = 0;

					if (i_rw || avl_rdata_valid)
						o_ready = 1;

					if (!i_request)
						next_state = 0;
				end
			end
		endcase
	end

endmodule