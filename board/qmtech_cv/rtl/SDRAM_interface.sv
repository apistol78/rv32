
// SDRAM interface

// Wrapping 16-bit DRAM chip on QM-tech Core Board.

module SDRAM_interface(
	input wire i_reset,
	input wire i_clock,
	input wire i_clock_sys,		// 50 MHz oscillator
	output wire o_clock_pll,	// 100 MHz PLL output
	
	input wire i_request,
	input wire i_rw,
	input wire [31:0] i_address,
	input wire [31:0] i_wdata,
	output reg [31:0] o_rdata,
	output reg o_ready,

	output sdram_clk,
	output sdram_clk_en,
	output sdram_cas_n,
	output sdram_ce_n,
	output sdram_ras_n,
	output sdram_we_n,
	output sdram_dqml,
	output sdram_dqmh,
	output [1:0] sdram_ba,
	output [12:0] sdram_addr,
	inout [15:0] sdram_data
);

	logic [15:0] sdram_control_wdata = 0;
	logic sdram_control_wr = 0;
	logic [24:0] sdram_control_waddr = 0;
	logic sdram_control_wload = 0;
	wire [15:0] sdram_control_wuse;

	wire [15:0] sdram_control_rdata;
	logic sdram_control_rd = 0;
	logic [24:0] sdram_control_raddr = 0;
	logic sdram_control_rload = 0;
	wire [15:0] sdram_control_ruse;

	Sdram_Control sdram_control(
		.REF_CLK(i_clock_sys),
		.RESET_N(!i_reset),
		.CLK(o_clock_pll),

		.WR_DATA(sdram_control_wdata),
		.WR(sdram_control_wr),
		.WR_ADDR(sdram_control_waddr),
		.WR_MAX_ADDR(24'h1ffffff),
		.WR_LENGTH(2),
		.WR_LOAD(sdram_control_wload),
		.WR_CLK(i_clock),
		.WR_FULL(),
		.WR_USE(sdram_control_wuse),

		.RD_DATA(sdram_control_rdata),
		.RD(sdram_control_rd),
		.RD_ADDR(sdram_control_raddr),
		.RD_MAX_ADDR(24'h1ffffff),
		.RD_LENGTH(2),
		.RD_LOAD(sdram_control_rload),
		.RD_CLK(i_clock),
		.RD_EMPTY(),
		.RD_USE(sdram_control_ruse),

		.SA(sdram_addr),
		.BA(sdram_ba),
		.CS_N(sdram_ce_n),
		.CKE(sdram_clk_en),
		.RAS_N(sdram_ras_n),
		.CAS_N(sdram_cas_n),
		.WE_N(sdram_we_n),
		.DQ(sdram_data),
		.DQM({ sdram_dqmh, sdram_dqml }),
		.SDR_CLK(sdram_clk)
	);

	logic [7:0] state = 0;

	initial begin
		o_ready = 0;
	end

	always_ff @(posedge i_clock) begin

		case (state)
		0: begin
			if (!o_ready && i_request) begin
				if (!i_rw) begin
					sdram_control_raddr <= { i_address[25:2], 1'b0 };
					sdram_control_rload <= 1;
					state <= 1;
				end
				else begin
					sdram_control_waddr <= { i_address[25:2], 1'b0 };
					sdram_control_wload <= 1;
					state <= 2;
				end
			end
			else if (o_ready && !i_request) begin
				o_ready <= 0;
			end
		end

		// ===================
		// Read

		1: begin
			sdram_control_rload <= 0;
			state <= 8;
		end

		8: begin
			if (sdram_control_ruse >= 2) begin
				sdram_control_rd <= 1;
				state <= 5;
			end
		end

		5: begin
			sdram_control_rd <= 0;
			state <= 10;
		end

		10: begin
			o_rdata <= { 16'b0, sdram_control_rdata };
			sdram_control_rd <= 1;
			state <= 6;
		end

		6: begin
			sdram_control_rd <= 0;
			state <= 11;
		end

		11: begin
			o_rdata <= { sdram_control_rdata, o_rdata[15:0] };
			o_ready <= 1;
			state <= 0;
		end

		// ===================
		// Write

		2: begin
			sdram_control_wload <= 0;
			state <= 9;
		end

		9: begin
			sdram_control_wr <= 1;
			sdram_control_wdata <= i_wdata[15:0];
			state <= 7;
		end

		7: begin
			sdram_control_wr <= 0;
			state <= 3;
		end

		3: begin
			sdram_control_wr <= 1;
			sdram_control_wdata <= i_wdata[31:16];
			state <= 4;
		end

		4: begin
			sdram_control_wr <= 0;
			if (sdram_control_wuse == 0) begin
				o_ready <= 1;
				state <= 0;
			end
		end

		endcase

	end

endmodule