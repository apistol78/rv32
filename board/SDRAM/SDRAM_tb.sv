/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/


module SDRAM_tb(
	input sys_clk,		// 50 MHz oscillator
	input sys_reset_n,
	
	output led_1,
	
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

	wire clock;				// 100MHz
	wire clock_sdram;		// 100MHz, phase shifted 9375 ps.
	wire clock_signaltap;
	
	IP_PLL_Clk pll_clk(
		.refclk(sys_clk),
		.rst(!sys_reset_n),
		.outclk_0(clock),
		.outclk_1(clock_sdram),
		.outclk_2(clock_signaltap)
	);
	
	wire reset = ~sys_reset_n;

	
	bit request = 1'b0;
	bit rw = 1'b0;
	bit [31:0] address;
	bit [31:0] wdata;
	wire [31:0] rdata;
	wire ready;
	

	wire [1:0] sdram_dqm;
	assign sdram_dqml = sdram_dqm[0];
	assign sdram_dqmh = sdram_dqm[1];
	logic [15:0] sdram_rdata;
	wire [15:0] sdram_wdata;
	wire sdram_data_rw;
	
    SDRAM_controller #(
        .FREQUENCY(100000000)
    ) sdram_controller(
		.i_reset(reset),
		.i_clock(clock),
		.i_clock_sdram(clock_sdram),

		.i_request(request),
		.i_rw(rw),
		.i_address(address),
		.i_wdata(wdata),
		.o_rdata(rdata),
		.o_ready(ready),

		.sdram_clk(sdram_clk),
		.sdram_clk_en(sdram_clk_en),
		.sdram_cas_n(sdram_cas_n),
		.sdram_cs_n(sdram_ce_n),
		.sdram_ras_n(sdram_ras_n),
		.sdram_we_n(sdram_we_n),
		.sdram_dqm(sdram_dqm),
		.sdram_bs(sdram_ba),		// Called BA in QMTech schematics
		.sdram_addr(sdram_addr),
		.sdram_rdata(sdram_rdata),
		.sdram_wdata(sdram_wdata),
		.sdram_data_rw(sdram_data_rw)
    );
	
	assign sdram_data = sdram_data_rw ? sdram_wdata : 16'hz;
	assign sdram_rdata = sdram_data;





	// SDRAM_interface_2 sdram(
	// 	.i_reset(reset),
	// 	.i_clock(clock),
	// 	.i_clock_sdram(clock_sdram),
	// 	// ---
	// 	.i_request(request),
	// 	.i_rw(rw),
	// 	.i_address(address),
	// 	.i_wdata(wdata),
	// 	.o_rdata(rdata),
	// 	.o_ready(ready),
	// 	// ---
	// 	.sdram_clk(sdram_clk),
	// 	.sdram_clk_en(sdram_clk_en),
	// 	.sdram_cas_n(sdram_cas_n),
	// 	.sdram_ce_n(sdram_ce_n),
	// 	.sdram_ras_n(sdram_ras_n),
	// 	.sdram_we_n(sdram_we_n),
	// 	.sdram_dqml(sdram_dqml),
	// 	.sdram_dqmh(sdram_dqmh),
	// 	.sdram_ba(sdram_ba),
	// 	.sdram_addr(sdram_addr),
	// 	.sdram_data(sdram_data)
	// );


	
	
	bit [31:0] error = 0;
	bit [2:0] led_mode = 0;
	bit [31:0] state = 0;

	initial begin
		address = 0;
	end
	
	always_ff @(posedge clock) begin
	
		if (reset) begin
			address <= 0;
			error <= 0;
			led_mode <= 0;
			state <= 0;
		end
		else begin
		
			case (state)
			
			// Write
			
			0: begin
				error <= 0;
			
				if (address <= 32'h0010_0000) begin
					led_mode <= 0;
					request <= 1'b1;
					rw <= 1'b1;
					wdata <= 32'hcafe_babe; // ^ address;		
					state <= 1;
				end
				else begin
					address <= 0;
					state <= 3;
				end
			end
			
			1: begin
				if (ready) begin
					request <= 1'b0;
					state <= 2;
				end
			end
			
			2: begin
				if (!ready) begin
					address <= address + 4;
					state <= 0;
				end
			end
			
			// Verify
			
			3: begin
				if (address <= 32'h0010_0000) begin
					led_mode <= 1;
					request <= 1'b1;
					rw <= 1'b0;
					state <= 4;
				end
				else begin
					address <= 0;
					state <= 6;
				end
			end
			
			4: begin
				if (ready) begin
				
					if (rdata != 32'hcafe_babe) // ^ address)
						error <= error + 1;
				
					request <= 1'b0;
					state <= 5;
				end
			end
			
			5: begin
				if (!ready) begin
					address <= address + 4;
					state <= 3;
				end
			end
			
			// Report
			
			6: begin
				if (error == 0)
					state <= 0;		// Begin new cycle
				else
					led_mode <= 2;
			end
			
			endcase
		end
	end
	
	
	bit [31:0] counter = 0;
	always_ff @(posedge clock) begin
		if (reset)
			counter <= 0;
		else
			counter <= counter + 1;
	end
	

	always_comb begin
		if (led_mode == 0)
			// Fast blink
			led_1 = counter[23];
		else if (led_mode == 1)
			// Slow blink
			led_1 = counter[25];
		else
			// Solid
			led_1 = reset;
	end

endmodule
