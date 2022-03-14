
// SDRAM interface

// Wrapping 16-bit DRAM chip on QM-tech Core Board.

module SDRAM_interface_2(
	input i_reset,
	input i_clock,      // 100 MHz
	input i_clock_sdram,

	input i_request,
	input i_rw,
	input [31:0] i_address,
	input [31:0] i_wdata,
	output logic [31:0] o_rdata,
	output o_ready,

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

    assign sdram_clk = i_clock_sdram;

    logic sdram_control_wb_cyc = 0;
    logic sdram_control_wb_stb = 0;
    logic sdram_control_wb_we = 0;
    wire [22:0] sdram_control_wb_addr;
    wire [31:0] sdram_control_wb_wdata;
    logic [3:0] sdram_control_wb_sel = 4'b1111;
    wire sdram_control_wb_stall;
    wire sdram_control_wb_ack;
    wire [31:0] sdram_control_wb_rdata;

    wire sdram_control_dmod;
    wire [15:0] sdram_control_o_ram_data;
    wire [15:0] sdram_control_i_ram_data;


    assign sdram_data = (sdram_control_dmod) ? sdram_control_o_ram_data : 16'hz;

    logic [15:0] r_ram_data = 0;
    logic [15:0] r_ram_data_ext_clk = 0;
    always_ff @(posedge i_clock) begin
        { r_ram_data, r_ram_data_ext_clk } <= { r_ram_data_ext_clk, sdram_data };
    end

    assign sdram_control_i_ram_data = r_ram_data;


    wbsdram sdram_control(
        .i_clk(i_clock),
        
        .i_wb_cyc(sdram_control_wb_cyc),
        .i_wb_stb(sdram_control_wb_stb),
        .i_wb_we(sdram_control_wb_we),
        .i_wb_addr(sdram_control_wb_addr),
        .i_wb_data(sdram_control_wb_wdata),
        .i_wb_sel(sdram_control_wb_sel),
        .o_wb_stall(sdram_control_wb_stall),
        .o_wb_ack(sdram_control_wb_ack),
        .o_wb_data(sdram_control_wb_rdata),

        .o_ram_cs_n(sdram_ce_n),
        .o_ram_cke(sdram_clk_en),
        .o_ram_ras_n(sdram_ras_n),
        .o_ram_cas_n(sdram_cas_n),
        .o_ram_we_n(sdram_we_n),
        .o_ram_bs(sdram_ba),
        .o_ram_addr(sdram_addr),
        .o_ram_dmod(sdram_control_dmod),
        .i_ram_data(sdram_control_i_ram_data),
        .o_ram_data(sdram_control_o_ram_data),
        .o_ram_dqm({ sdram_dqmh, sdram_dqml }),
        .o_debug()
    );

    // ======

	typedef enum bit [1:0]
	{
		STATE_IDLE		= 2'd0,
		STATE_READ_0	= 2'd1,
		STATE_WRITE_0	= 2'd2
	} state_t;

	state_t state = STATE_IDLE;
	logic ready = 0;

	assign o_ready = ready; // && i_request;
	
	always_comb begin
		sdram_control_wb_addr = { i_address[24:2] };
        sdram_control_wb_wdata = i_wdata;
        sdram_control_wb_we = i_rw;
	end

	always_ff @(posedge i_clock) begin

		if (i_reset) begin
			state <= STATE_IDLE;
			ready <= 0;
		end
		else begin

			case (state)
			STATE_IDLE: begin
				if (!ready && i_request && !sdram_control_wb_stall) begin
					if (!i_rw) begin
                        sdram_control_wb_cyc <= 1;
                        sdram_control_wb_stb <= 1;
						state <= STATE_READ_0;
					end
					else begin
                        sdram_control_wb_cyc <= 1;
                        sdram_control_wb_stb <= 1;
						state <= STATE_WRITE_0;
					end
				end
				else if (!i_request) begin
					ready <= 0;
				end
			end

			// ===================
			// Read

			STATE_READ_0: begin
                if (sdram_control_wb_ack) begin
                    o_rdata <= sdram_control_wb_rdata;
                    sdram_control_wb_cyc <= 0;
                    sdram_control_wb_stb <= 0;
                    ready <= 1;
                    state <= STATE_IDLE;
                end
			end

			// ===================
			// Write

			STATE_WRITE_0: begin
                if (sdram_control_wb_ack) begin
                    sdram_control_wb_cyc <= 0;
                    sdram_control_wb_stb <= 0;
                    ready <= 1;
                    state <= STATE_IDLE;
                end
			end

			// ===================

			default: begin
				state <= STATE_IDLE;
			end

			endcase

		end
	end

endmodule