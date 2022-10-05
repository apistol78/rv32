`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_CSR #(
	parameter FREQUENCY,
	parameter VENDORID,
	parameter ARCHID,
	parameter IMPID,
	parameter HARTID
)(
	input i_reset,
	input i_clock,

	// External interrupt input.
	input i_timer_interrupt,
	input i_external_interrupt,
	output o_external_interrupt_enable,

	// Software interrupt input.
	input i_ecall,
	input i_mret,

	// Instruction I/O access.
	input [11:0] i_index,
	output bit [31:0] o_rdata,
	input i_wdata_wr,
	input [31:0] i_wdata,

	// Direct read access.
	output [31:0] o_epc,

	// Pending interrupt output.
	output bit o_irq_pending,
	output bit [31:0] o_irq_pc,
	input i_irq_dispatched,
	input [31:0] i_irq_epc,

	// Retired instructions.
	input [63:0] i_retired
);
	localparam PRESCALE = FREQUENCY / 1000;
	localparam PRESCALE_WIDTH = $clog2(PRESCALE);

	bit mstatus_mpie = 0;
	bit mstatus_mie = 0;
	bit mie_meie = 0; 
	bit mie_mtie = 0;
	bit mie_msie = 0;
	bit [31:0] mtvec = 0;
	bit [31:0] mepc = 0;
	bit [31:0] mcause = 0;
	bit mip_meip = 0;
	bit mip_mtip = 0;
	bit mip_msip = 0;
	bit [31:0] mscratch = 0;

	wire [31:0] mstatus = { 27'b0, mstatus_mpie, mstatus_mie, 3'b0 };
	wire [31:0] mie = { 20'b0, mie_meie, 3'b0, mie_mtie, 3'b0, mie_msie, 3'b0 };	
	wire [31:0] mip = { 20'b0, mip_meip, 3'b0, mip_mtip, 3'b0, mip_msip, 3'b0 };

	bit [63:0] cycle = 0;
	bit [63:0] wtime = 0;
	bit [PRESCALE_WIDTH - 1:0] prescale = 0;
	bit [2:0] issued = 0;

	assign o_external_interrupt_enable = !o_irq_pending && mstatus_mie && mie_meie;
	assign o_epc = mepc;

	// Read CSR value by index.
	always_comb begin
		o_rdata = 0;
		if (i_index == `CSR_MSTATUS)
			o_rdata = mstatus;
		else if (i_index == `CSR_MIE)
			o_rdata = mie;
		else if (i_index == `CSR_MTVEC)
			o_rdata = mtvec;
		else if (i_index == `CSR_MSCRATCH)
			o_rdata = mscratch;
		else if (i_index == `CSR_MEPC)
			o_rdata = mepc;
		else if (i_index == `CSR_MCAUSE)
			o_rdata = mcause;
		else if (i_index == `CSR_MIP)
			o_rdata = mip;
		else if (i_index == `CSR_CYCLE)
			o_rdata = cycle[31:0];
		else if (i_index == `CSR_CYCLEH)
			o_rdata = cycle[63:32];
		else if (i_index == `CSR_TIME)
			o_rdata = wtime[31:0];
		else if (i_index == `CSR_TIMEH)
			o_rdata = wtime[63:32];
		else if (i_index == `CSR_INSTRET)
			o_rdata = i_retired[31:0];
		else if (i_index == `CSR_INSTRETH)
			o_rdata = i_retired[63:32];
		else if (i_index == `CSR_MVENDORID)
			o_rdata = VENDORID;
		else if (i_index == `CSR_MARCHID)
			o_rdata = ARCHID;
		else if (i_index == `CSR_MIMPID)
			o_rdata = IMPID;
		else if (i_index == `CSR_MHARTID)
			o_rdata = HARTID;
	end

	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			mstatus_mie <= 0;
			mie_meie <= 0;
			mie_mtie <= 0;
			mie_msie <= 0;
			mtvec <= 0;
			mepc <= 0;
			mcause <= 0;
			mip_meip <= 0;
			mip_mtip <= 0;
			mip_msip <= 0;
			mscratch <= 0;
			issued <= 0;
			o_irq_pending <= 1'b0;
		end
		else begin
			// Write CSR registers.
			if (i_wdata_wr) begin
				if (i_index == `CSR_MSTATUS) begin
					mstatus_mie <= i_wdata[3];
				end
				else if (i_index == `CSR_MIE) begin
					mie_meie <= i_wdata[11];
					mie_mtie <= i_wdata[7];
					mie_msie <= i_wdata[3];
				end
				else if (i_index == `CSR_MTVEC)
					mtvec <= i_wdata;
				else if (i_index == `CSR_MSCRATCH)
					mscratch <= i_wdata;
				else if (i_index == `CSR_MEPC)
					mepc <= i_wdata;
				else if (i_index == `CSR_MIP) begin
					mip_meip <= i_wdata[11];
					mip_mtip <= i_wdata[7];
					mip_msip <= i_wdata[3];
				end
			end

			// Latch interrupts pending.
			if (i_timer_interrupt && mie_mtie) begin
				mip_mtip <= 1'b1;
			end
			if (i_external_interrupt && mie_meie) begin
				mip_meip <= 1'b1;
			end
			if (i_ecall && mie_msie) begin
				mip_msip <= 1'b1;
			end

			// Issue interrupts.
			if (!o_irq_pending && mstatus_mie) begin
				if (mip_mtip) begin
					o_irq_pending <= 1'b1;
					o_irq_pc <= mtvec;

					mstatus_mpie <= mstatus_mie;
					mstatus_mie <= 1'b0;

					mcause <= 32'h80000000 | (1 << 7);
					issued <= 3'd1;
				end
				else if (mip_meip) begin
					o_irq_pending <= 1'b1;
					o_irq_pc <= mtvec;

					mstatus_mpie <= mstatus_mie;
					mstatus_mie <= 1'b0;

					mcause <= 32'h80000000 | (1 << 11);					
					issued <= 3'd2;
				end
				else if (mip_msip) begin
					o_irq_pending <= 1'b1;
					o_irq_pc <= mtvec;

					mstatus_mpie <= mstatus_mie;
					mstatus_mie <= 1'b0;

					mcause <= 32'h00000000 | (1 << 11);					
					issued <= 3'd4;
				end
			end

			// Restore interrupt enable from "stack".
			if (i_mret) begin
				mstatus_mie <= mstatus_mpie;
			end
			
			// Clear interrupt pending flags.
			if (i_irq_dispatched) begin
				mepc <= i_irq_epc;

				if (issued[0])
					mip_mtip <= 1'b0;
				if (issued[1])
					mip_meip <= 1'b0;
				if (issued[2])
					mip_msip <= 1'b0;

				o_irq_pending <= 1'b0;
				issued <= 0;
			end
		end
	end

	// Cycle counter.
	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			cycle <= 0;
		end
		else begin
			cycle <= cycle + 1;
		end
	end

	// Wall time counter.
	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			wtime <= 0;
			prescale <= 0;
		end
		else begin
			prescale <= prescale + 1;
			if (prescale >= PRESCALE) begin
				wtime <= wtime + 1;
				prescale <= 0;
			end
		end
	end

endmodule
