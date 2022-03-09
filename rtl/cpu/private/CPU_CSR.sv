`include "CPU_Defines.sv"

`timescale 1ns/1ns

module CPU_CSR #(
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

	// Software interrupt input.
	input i_ecall,

	// Instruction I/O access.
	input [11:0] i_index,
	output logic [31:0] o_rdata,
	input i_wdata_wr,
	input [31:0] i_wdata,

	// Direct read access.
	output [31:0] o_epc,

	// Pending interrupt output.
	output logic o_irq_pending,
	output logic [31:0] o_irq_pc,
	input i_irq_dispatched,
	input [31:0] i_irq_epc
);

	logic mstatus_mie = 0;
	logic mie_meie = 0; 
	logic mie_mtie = 0;
	logic mie_msie = 0;
	logic [31:0] mtvec = 0;
	logic [31:0] mepc = 0;
	logic [31:0] mcause = 0;
	logic mip_meip = 0;
	logic mip_mtip = 0;
	logic mip_msip = 0;

	wire [31:0] mstatus = { 28'b0, mstatus_mie, 3'b0 };
	wire [31:0] mie = { 20'b0, mie_meie, 3'b0, mie_mtie, 3'b0, mie_msie, 3'b0 };	
	wire [31:0] mip = { 20'b0, mip_meip, 3'b0, mip_mtip, 3'b0, mip_msip, 3'b0 };

	assign o_epc = mepc;

	// Read CSR value by index.
	always @(*) begin
		o_rdata = 0;
		if (i_index == `CSR_MSTATUS)
			o_rdata = mstatus;
		else if (i_index == `CSR_MIE)
			o_rdata = mie;
		else if (i_index == `CSR_MTVEC)
			o_rdata = mtvec;
		else if (i_index == `CSR_MEPC)
			o_rdata = mepc;
		else if (i_index == `CSR_MCAUSE)
			o_rdata = mcause;
		else if (i_index == `CSR_MIP)
			o_rdata = mip;
		else if (i_index == `CSR_MVENDORID)
			o_rdata = VENDORID;
		else if (i_index == `CSR_MARCHID)
			o_rdata = ARCHID;
		else if (i_index == `CSR_MIMPID)
			o_rdata = IMPID;
		else if (i_index == `CSR_MHARTID)
			o_rdata = HARTID;
	end

	logic [2:0] issued = 0;
	logic rd = 0;

	always @(posedge i_clock) begin
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
				else if (i_index == `CSR_MEPC)
					mepc <= i_wdata;
			end

			// Check interrupts, set pending flags.
			if (mstatus_mie) begin
				if (i_timer_interrupt && mie_mtie) begin
					mip_mtip <= 1'b1;
				end
				if (i_external_interrupt && mie_meie) begin
					mip_meip <= 1'b1;
				end
				if (i_ecall && mie_msie) begin
					mip_msip <= 1'b1;
				end
			end

			// Issue interrupts.
			if (!o_irq_pending) begin
				if (mip_mtip) begin
					o_irq_pending <= 1;
					o_irq_pc <= mtvec;
					mcause <= 32'h80000000 | (1 << 7);
					issued <= 1;
				end
				else if (mip_meip) begin
					o_irq_pending <= 1;
					o_irq_pc <= mtvec;
					mcause <= 32'h80000000 | (1 << 11);					
					issued <= 2;
				end
				else if (mip_msip) begin
					o_irq_pending <= 1;
					o_irq_pc <= mtvec;
					mcause <= 32'h00000000 | (1 << 11);					
					issued <= 4;
				end
			end
			else begin
				rd <= i_irq_dispatched;
				if ({ rd, i_irq_dispatched } == 2'b01) begin
					mepc <= i_irq_epc;

					if (issued[0])
						mip_mtip <= 1'b0;
					if (issued[1])
						mip_meip <= 1'b0;
					if (issued[2])
						mip_msip <= 1'b0;

					o_irq_pending <= 0;
					issued <= 0;
				end
			end
		end
	end

endmodule
