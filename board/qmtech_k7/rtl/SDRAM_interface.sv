`timescale 1ns / 1ps

module SDRAM_interface(
	input i_reset,
	input i_clock,
	input i_clock_ref,	// 200 MHz

	input i_request,
	input i_rw,
	input [31:0] i_address,
	input [31:0] i_wdata,
	output logic [31:0] o_rdata,
	output logic o_ready,
	
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
	output [1:0] ddr3_dm,
	output [0:0] ddr3_odt
);

	logic [27:0] app_addr;
	logic [2:0] app_cmd;
	logic app_en;

	logic [127:0] app_wdf_data;
	logic [15:0] app_wdf_mask;
	logic app_wdf_wren;
	
	wire [127:0] app_rd_data;
	wire app_rd_data_end;
	wire app_rd_data_valid;
	wire app_rdy;
	wire app_wdf_rdy;

	wire ui_clk;
	wire ui_clk_sync_rst;

	wire init_calib_complete;

	IP_SDRAM_DDR3 sdram_ddr3(
		.ddr3_dq(ddr3_dq),
		.ddr3_dqs_n(ddr3_dqs_n),
		.ddr3_dqs_p(ddr3_dqs_p),
		.ddr3_addr(ddr3_addr),
		.ddr3_ba(ddr3_ba),
		.ddr3_ras_n(ddr3_ras_n),
		.ddr3_cas_n(ddr3_cas_n),
		.ddr3_we_n(ddr3_we_n),
		.ddr3_reset_n(ddr3_reset_n),
		.ddr3_ck_p(ddr3_ck_p),
		.ddr3_ck_n(ddr3_ck_n),
		.ddr3_cke(ddr3_cke),
		.ddr3_dm(ddr3_dm),
		.ddr3_odt(ddr3_odt),

		.sys_clk_i(i_clock),
		.clk_ref_i(i_clock_ref),
		
		.app_addr(app_addr),
		.app_cmd(app_cmd),
		.app_en(app_en),

		.app_wdf_data(app_wdf_data),
		.app_wdf_end(app_wdf_wren),   // Tied with app_wdf_wren.
		.app_wdf_mask(app_wdf_mask),
		.app_wdf_wren(app_wdf_wren),

		.app_rd_data(app_rd_data),
		.app_rd_data_end(app_rd_data_end),
		.app_rd_data_valid(app_rd_data_valid),
		.app_rdy(app_rdy),
		.app_wdf_rdy(app_wdf_rdy),

		.app_sr_req(1'b0),
		.app_ref_req(1'b0),
		.app_zq_req(1'b0),
		.app_sr_active(),
		.app_ref_ack(),
		.app_zq_ack(),

		.ui_clk(ui_clk),
		.ui_clk_sync_rst(ui_clk_sync_rst),
		
		.init_calib_complete(init_calib_complete),
		.device_temp(),
		.sys_rst(i_reset)
 	);

	localparam CMD_READ = 3'b001;
	localparam CMD_WRITE = 3'b000;

	typedef enum bit [2:0]
	{
		IDLE,
		WAIT_READ,
		WAIT_WRITE,
		WAIT_END_REQUEST
	} state_t;

	state_t state = IDLE;
	
	initial begin
	   o_ready = 1'b0;
	end

	always_ff @(posedge i_clock) begin

		case (state)
		IDLE: begin
			if (i_request && init_calib_complete) begin
				if (!i_rw) begin
					// Read
					if (app_rdy) begin
						app_en <= 1;
						app_addr <= i_address[29:2];
						app_cmd <= CMD_READ;
						state <= WAIT_READ;
					end
				end
				else begin
					// Write
					if (app_rdy && app_wdf_rdy) begin
						app_en <= 1;
						app_wdf_wren <= 1;
						app_addr <= i_address[29:2];
						app_wdf_mask <= 16'hfff0;	// Only write 32 bit.
						app_cmd <= CMD_WRITE;
						app_wdf_data <= { 96'b0, i_wdata };
						state <= WAIT_WRITE;
					end
				end
			end
		end

		WAIT_READ: begin
			if (app_rdy && app_en)
				app_en <= 0;
			if (app_rd_data_valid) begin
				o_rdata <= app_rd_data[31:0];
				o_ready <= 1;
				state <= WAIT_END_REQUEST;
			end
		end

		WAIT_WRITE: begin
			if (app_rdy && app_en)
				app_en <= 0;
			if (app_wdf_rdy && app_wdf_wren)
				app_wdf_wren <= 0;
			if (!app_en && !app_wdf_wren) begin
				o_ready <= 1;
				state <= WAIT_END_REQUEST;
			end
		end

		WAIT_END_REQUEST: begin
			if (!i_request) begin
				o_ready <= 0;
				state <= IDLE;
			end
		end

		endcase

	end

endmodule
