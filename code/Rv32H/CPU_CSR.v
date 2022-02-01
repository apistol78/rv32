`include "CPU_Defines.v"

`timescale 1ns/1ns

module CPU_CSR(
	input wire i_reset,
	input wire i_clock,

	// External interrupt input.
	input wire i_interrupt,

	// Instruction I/O access.
	input wire [11:0] i_index,
	output reg [31:0] o_rdata,
	input wire i_wdata_wr,
	input wire [31:0] i_wdata,

	// Direct read access.
	output wire [31:0] o_epc,

	// Pending interrupt output.
	output reg o_irq_pending,
	output reg [31:0] o_irq_pc,
	input wire i_irq_dispatched,
	input wire [31:0] i_irq_epc
);

	reg mie_mtie;
	reg [31:0] mtvec;
	reg [31:0] mepc;
	reg [31:0] mcause;
	reg mip_mtip;

	wire [31:0] mie = { 24'b0, mie_mtie, 7'b0 };	
	wire [31:0] mip = { 24'b0, mip_mtip, 7'b0 };

	assign o_epc = mepc;

	initial begin
		mie_mtie = 0;
		mtvec = 0;
		mepc = 0;
		mcause = 0;
		mip_mtip = 0;
	end

	// Read CSR value by index.
	always @(*) begin
		o_rdata = 0;
		if (i_index == `CSR_MIE)
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
			o_rdata = 0;
		else if (i_index == `CSR_MARCHID)
			o_rdata = 0;
		else if (i_index == `CSR_MIMPID)
			o_rdata = 0;
		else if (i_index == `CSR_MHARTID)
			o_rdata = 0;
	end

	// Write CSR value by index.
	always @(posedge i_clock) begin
		if (i_reset) begin
			mtvec <= 0;
			mie_mtie <= 0;
		end
		else begin
			if (i_wdata_wr) begin
				if (i_index == `CSR_MIE) begin
					mie_mtie <= i_wdata[7];
				end
				else if (i_index == `CSR_MTVEC)
					mtvec <= i_wdata;
			end
		end
	end

	always @(posedge i_clock) begin
		if (i_reset) begin
			mepc <= 0;
			mcause <= 0;
			mip_mtip <= 0;
		end
		else begin
			if (i_interrupt) begin
				// \todo We're assuming only external interrupt is the timer.
				if (mie_mtie) begin
					o_irq_pending <= 1;
					o_irq_pc <= mtvec;
					mcause <= 32'h80000000 | (1 << 7);
					mip_mtip <= 1'b1;
				end
			end

			if (i_irq_dispatched) begin
				o_irq_pending <= 0;
				mepc <= i_irq_epc;
				mip_mtip <= 1'b0;
			end
		end
	end

endmodule
