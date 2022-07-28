/*

Memory mapping

 2 bank bits
13 row bits
 9 column bits
--------------
24 bits total

2222 1111 1111 1100 0000 0000
3210 9876 5432 1098 7654 3210
.RRR RRRR RRRR RRBB CCCC CCCC 0

*/

module SDRAM_controller #(
	parameter FREQUENCY = 100_000_000,
	parameter ADDRESS_WIDTH = 13,
	parameter DATA_WIDTH = 16
)(
	input i_reset,
	input i_clock,
	input i_clock_sdram,			// Offset from i_clock to help with timing.

	input i_request,
	input i_rw,
	input [31:0] i_address,			// Must be 4 byte aligned.
	input [31:0] i_wdata,
	output logic [31:0] o_rdata,
	output bit o_ready,

	output bit sdram_clk,
	output bit sdram_clk_en,
	output bit sdram_cas_n,
	output bit sdram_cs_n,
	output bit sdram_ras_n,
	output bit sdram_we_n,
	output bit [1:0] sdram_dqm,
	output bit [1:0] sdram_bs,		// Also commonly called BA.
	output bit [ADDRESS_WIDTH-1:0] sdram_addr,
	
	//inout [DATA_WIDTH-1:0] sdram_data
	
	input [DATA_WIDTH-1:0] sdram_rdata,
	output bit [DATA_WIDTH-1:0] sdram_wdata,
	output bit sdram_data_rw
);
	// Parameters
	localparam STARTUP_COUNT	= 20000; // ( 100 * FREQUENCY) /     1_000_000;	// 100 us
	localparam tRP_COUNT		= 4; //2; // (  20 * FREQUENCY) / 1_000_000_000;	// 20 ns
	localparam tRFC_COUNT		= 8; //7; // (  65 * FREQUENCY) / 1_000_000_000;	// 65 ns
	localparam tMRD_COUNT		= 2000; // (2000 * FREQUENCY) /   100_000_000;	// 2000 cyc @ 100 MHz
	localparam tRCD_COUNT		= 5; //7; // (  65 * FREQUENCY) / 1_000_000_000;	// 65 ns
	
	// Types
	typedef enum bit [3:0]
	{
		CMD_NOP	= 4'b0111,
		CMD_PRECHARGE = 4'b0010,
		CMD_SET_MODE = 4'b0000,
		CMD_REFRESH = 4'b0001,
		CMD_ACTIVATE = 4'b0011,
		CMD_READ = 4'b0101,
		CMD_WRITE = 4'b0100
	} command_t;

	typedef enum bit [5:0]
	{
		STATE_STARTUP,
		STATE_STARTUP_PRECHARGE,
		STATE_STARTUP_WAIT_PRECHARGE,
		STATE_STARTUP_AUTO_REFRESH_1,
		STATE_STARTUP_WAIT_AUTO_REFRESH_1,
		STATE_STARTUP_AUTO_REFRESH_2,
		STATE_STARTUP_WAIT_AUTO_REFRESH_2,
		STATE_STARTUP_SET_MODE,
		STATE_STARTUP_WAIT_SET_MODE,
		STATE_IDLE,
		STATE_REFRESH,			// 10
		STATE_REFRESH_2,		// 11
		STATE_WAIT_REFRESH,
		STATE_ACTIVATE,			// 13
		STATE_WAIT_ACTIVATE,
		STATE_READ,				// 15
		STATE_WAIT_READ,
		STATE_WRITE,			// 17
		STATE_WAIT_WRITE,
		STATE_PRECHARGE,		// 19
		STATE_WAIT_PRECHARGE
	} state_t;

	typedef enum bit [0:0]
	{
		WBM_PROGRAMMED_BURST_LENGTH = 1'b0,	// Burst read and burst write
		WBM_SINGLE_LOCATION_ACCESS = 1'b1	// Burst read and single write
	}
	write_burst_mode_t;

	typedef enum bit [2:0]
	{
		CAS_2 = 3'b010,
		CAS_3 = 3'b011
	}
	cas_t;

	state_t state = STATE_STARTUP;
	bit [15:0] count = STARTUP_COUNT;
	bit [31:0] refresh = 0;
	bit should_refresh = 1'b0;
	command_t command = CMD_NOP;
	bit [31:0] wdata;
	bit [22:0] address;

	// Initial
	initial begin
		o_ready <= 1'b0;

		// Ensure DQM and CKE are high during initial.
		sdram_dqm = 2'b11;
		sdram_clk_en = 1'b0;

		// Others
		sdram_bs = 2'b00;
		sdram_addr = 13'b0;
		sdram_wdata = 0;
		sdram_data_rw = 1'b0;
	end

	// Combinatorial
	assign sdram_clk = i_clock_sdram;
	assign sdram_cs_n = command[3];
	assign sdram_ras_n = command[2];
	assign sdram_cas_n = command[1];
	assign sdram_we_n = command[0];

	//=============================================

    logic [15:0] r_ram_data = 0;
    logic [15:0] r_ram_data_ext_clk = 0;
    always_ff @(posedge i_clock) begin
        { r_ram_data, r_ram_data_ext_clk } <= { r_ram_data_ext_clk, sdram_rdata };
    end	

	//=============================================
	
	always_ff @(posedge i_clock) begin
		if (i_reset) begin
			state <= STATE_STARTUP;
			count <= STARTUP_COUNT;
			refresh <= 0;
			should_refresh <= 1'b0;
			command <= CMD_NOP;
		end
		else begin

			// Decrement startup counter.
			count <= count - 1;

			// Check if we should refresh.
			refresh <= refresh + 1;
			if (refresh >= 32'd20_0) begin
				refresh <= 0;
				should_refresh <= 1'b1;
			end

			case (state)
			
				/*
				Startup stage, output NOP.
				Enable clock after some time.
				*/
				STATE_STARTUP: begin
					command <= CMD_NOP;
					
					// Enable clock after 25% of startup time has passed.
					if (count <= (STARTUP_COUNT * 3) / 4) begin
						sdram_dqm <= 2'b00;
						sdram_clk_en <= 1'b1;
					end

					if (count == 0) begin
						state <= STATE_STARTUP_PRECHARGE;
					end
				end

				/*
				Precharge all banks.
				*/
				STATE_STARTUP_PRECHARGE: begin
					command <= CMD_PRECHARGE;

					sdram_bs <= 2'b00;
					sdram_addr <= { 2'b00, 1'b1, 10'b0000000000 };	// A10 set to precharge all banks.

					count <= tRP_COUNT;
					state <= STATE_STARTUP_WAIT_PRECHARGE;
				end
				
				/*
				Wait until startup precharge has finished.
				*/
				STATE_STARTUP_WAIT_PRECHARGE: begin
					command <= CMD_NOP;
					if (count == 0) begin
						state <= STATE_STARTUP_AUTO_REFRESH_1;
					end				
				end
				
				/*
				Perform startup auto refresh.
				*/
				STATE_STARTUP_AUTO_REFRESH_1: begin
					command <= CMD_REFRESH;
					count <= tRFC_COUNT;
					state <= STATE_STARTUP_WAIT_AUTO_REFRESH_1;
				end

				/*
				Wait until startup auto refresh finished.
				*/
				STATE_STARTUP_WAIT_AUTO_REFRESH_1: begin
					command <= CMD_NOP;
					if (count == 0) begin
						state <= STATE_STARTUP_AUTO_REFRESH_2;
					end	
				end				
				
				/*
				Perform startup auto refresh.
				*/
				STATE_STARTUP_AUTO_REFRESH_2: begin
					command <= CMD_REFRESH;
					count <= tRFC_COUNT;
					state <= STATE_STARTUP_WAIT_AUTO_REFRESH_2;
				end

				/*
				Wait until startup auto refresh finished.
				*/
				STATE_STARTUP_WAIT_AUTO_REFRESH_2: begin
					command <= CMD_NOP;
					if (count == 0) begin
						state <= STATE_STARTUP_SET_MODE;
					end	
				end	

				/*
				Set DRAM mode.
				*/
				STATE_STARTUP_SET_MODE: begin
					command <= CMD_SET_MODE;
					sdram_bs <= 2'b00;
					sdram_addr <= { 3'b000, WBM_PROGRAMMED_BURST_LENGTH, 2'b00, CAS_2, 1'b0, 3'b001 };	// ?,Write Burst Mode,?,CAS,Burst Type,Burst Length
					count <= tMRD_COUNT;
					state <= STATE_STARTUP_WAIT_SET_MODE;
				end
				
				/*
				Wait until mode has been set.
				*/
				STATE_STARTUP_WAIT_SET_MODE: begin
					command <= CMD_NOP;
					if (count == 0) begin
						state <= STATE_IDLE;
					end	
				end

				//=================================================

				/*
				*/
				STATE_IDLE: begin
					command <= CMD_NOP;

					sdram_data_rw <= 1'b0;

					//if (!i_request)
						o_ready <= 1'b0;

					if (should_refresh) begin
						should_refresh <= 1'b0;
						state <= STATE_REFRESH;
					end
					else if (i_request) begin
						// Got request, start with activating bank/row.
						address <= i_address[24:2];
						wdata <= i_wdata;
						state <= STATE_ACTIVATE;
					end
				end

				/*
				*/
				STATE_REFRESH: begin
					command <= CMD_REFRESH;
					state <= STATE_REFRESH_2;
				end

				STATE_REFRESH_2: begin
					command <= CMD_REFRESH;
					count <= tRFC_COUNT;
					state <= STATE_WAIT_REFRESH;
				end

				/*
				*/
				STATE_WAIT_REFRESH: begin
					command <= CMD_NOP;
					if (count == 0) begin
						state <= STATE_IDLE;
					end
				end

				/*
				*/
				STATE_ACTIVATE: begin
					command <= CMD_ACTIVATE;
					
					sdram_bs <= address[9:8];
					sdram_addr <= address[22:10];
					//sdram_dqm <= 2'b11;

					count <= 3; // tRCD_COUNT;
					state <= STATE_WAIT_ACTIVATE;			
				end

				/*
				*/
				STATE_WAIT_ACTIVATE: begin
					command <= CMD_NOP;
					if (count == 0) begin

						sdram_addr <= { 4'b0000, address[7:0], 1'b0 };
						//sdram_addr[10] <= 1'b0;
						sdram_addr[10] <= 1'b1;
						//sdram_dqm <= 2'b00;
						
						sdram_wdata <= wdata[31:16];
						sdram_data_rw <= i_rw;

						state <= i_rw ? STATE_WRITE : STATE_READ;	
					end
				end

				/*
				*/
				STATE_READ: begin
					command <= CMD_READ;
					count <= tRCD_COUNT;
					state <= STATE_WAIT_READ;
				end

				/*
				*/
				STATE_WAIT_READ: begin
					command <= CMD_NOP;

					// if (count == tRCD_COUNT - 2) begin
					// 	o_rdata[31:16] <= sdram_rdata;
					// end
					// else if (count == tRCD_COUNT - 3) begin
					// 	o_rdata[15:0] <= sdram_rdata;
					// end

					if (count == tRCD_COUNT - 4) begin
						o_rdata[31:16] <= r_ram_data;
					end
					else if (count == tRCD_COUNT - 5) begin
						o_rdata[15:0] <= r_ram_data;
					end

					if (count == 0) begin
						o_ready <= 1'b1;
						
						//state <= STATE_PRECHARGE;

						count <= tRP_COUNT;
						state <= STATE_WAIT_PRECHARGE;						
					end
				end

				/*
				*/
				STATE_WRITE: begin
					command <= CMD_WRITE;
					count <= tRCD_COUNT;
					state <= STATE_WAIT_WRITE;
				end

				/*
				*/
				STATE_WAIT_WRITE: begin
					command <= CMD_NOP;

					sdram_wdata <= wdata[15:0];

					if (count == 0) begin
						o_ready <= 1'b1;

						//state <= STATE_PRECHARGE;
						
						count <= tRP_COUNT;
						state <= STATE_WAIT_PRECHARGE;						
					end			
				end

				/*
				*/
				STATE_PRECHARGE: begin
					command <= CMD_PRECHARGE;

					sdram_bs <= 2'b00;
					
					//sdram_addr <= { 2'b00, 1'b1, 10'b0000000000 };	// A10 set to precharge all banks.
					sdram_addr <= { 4'b0000, address[7:0], 1'b0 };
					sdram_addr[10] <= 1'b1;

					//sdram_dqm <= 2'b11;

					count <= tRP_COUNT;
					state <= STATE_WAIT_PRECHARGE;
				end

				/*
				*/
				STATE_WAIT_PRECHARGE: begin
					command <= CMD_NOP;

					if (!i_request)
						o_ready <= 1'b0;

					if (count == 0) begin
						state <= STATE_IDLE;
					end
				end

				/*
				*/
				default: begin
					state <= STATE_STARTUP;
				end
			endcase

		end
	end


endmodule
