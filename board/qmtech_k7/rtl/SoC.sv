/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
`timescale 1ns / 1ps

`define FREQUENCY 125_000_000

module SoC(
    input sys_clk,
    input sys_rst,	// SW2, pulled high

    //output led_1,
    //output led_2,
    //output led_3,

	input uart_0_rx,
	output uart_0_tx,
	input uart_1_rx,
	output uart_1_tx,
    
	output sd_clk,
	inout sd_cmd,
	inout [3:0] sd_dat,

    output i2s_sdout,
    output i2s_sclk,
    output i2s_lrck,
    output i2s_mclk,

	output hdmi_nreset,
    inout hdmi_sda,
    output hdmi_scl,
	output hdmi_idck,
	output hdmi_vsync,
	output hdmi_hsync,
	output hdmi_de,
	input hdmi_int,
	output [7:0] hdmi_r,
	output [7:0] hdmi_g,
	output [7:0] hdmi_b,
    output hdmi_sdo,
    output hdmi_sck,
    output hdmi_mclk,
    output hdmi_ws,

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

    wire clock;
	wire clock_ref;
	wire clock_video;
    IP_Clock ipclk(
        .clk_out1(clock),		// 125 MHz
		.clk_out2(clock_ref),	// 200 MHz
		.clk_out3(clock_video),	// 27 MHz
        .reset(1'b0),
        .clk_in1(sys_clk)
    );
    
	//=====================================

	wire reset;
	RESET rst(
		.i_clock(clock),
		.i_reset_sw(~sys_rst),
		.o_reset_0(),
		.o_reset_1(),
		.o_reset_2(reset)
	);

	//=====================================
	// ROM ($00000000)

	wire rom_select;
	wire [31:0] rom_address;
	wire [31:0] rom_rdata;
	wire rom_ready;
	BROM rom(
		.i_clock(clock),
		.i_request(rom_select && bus_request),
		.i_address(rom_address),
		.o_rdata(rom_rdata),
		.o_ready(rom_ready)
	);
	
	//=====================================
	// RAM ($10000000 - $10001000)

	wire ram_select;
	wire [31:0] ram_address;
	wire [31:0] ram_rdata;
	wire ram_ready;
	BRAM #(
		.WIDTH(32),
		.SIZE(32'h400),
		.ADDR_LSH(2)
	) ram(
		.i_clock(clock),
		.i_request(ram_select && bus_request),
		.i_rw(bus_rw),
		.i_address(ram_address),
		.i_wdata(bus_wdata),
		.o_rdata(ram_rdata),
		.o_ready(ram_ready)
	);

	//=====================================
    // SDRAM ($20000000)

	wire w_sdram_request;
	wire w_sdram_rw;
	wire [31:0] w_sdram_address;
	wire [127:0] w_sdram_wdata;
	wire [127:0] w_sdram_rdata;
	wire w_sdram_ready;
	
	wire sdram_select;
	wire [31:0] sdram_address;
	wire [31:0] sdram_rdata;
	wire sdram_ready;	

    SDRAM_interface sdram(
		.i_reset(reset),
		.i_clock(clock),
		.i_clock_ref(clock_ref),

		.i_request(w_sdram_request),
		.i_rw(w_sdram_rw),
		.i_address(w_sdram_address),
		.i_wdata(w_sdram_wdata),
		.o_rdata(w_sdram_rdata),
		.o_ready(w_sdram_ready),
		
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
		.ddr3_odt(ddr3_odt)
    );

	LRU_cache sdram_lru(
		.i_clock(clock),

		.i_request(sdram_select && bus_request),
		.i_rw(bus_rw),
		.i_address(sdram_address),
		.i_wdata(bus_wdata),
		.o_rdata(sdram_rdata),
		.o_ready(sdram_ready),
		.i_oddeven(bus_pa_busy),	// Instruction or data request

		.o_sdram_request(w_sdram_request),
		.o_sdram_rw(w_sdram_rw),
		.o_sdram_address(w_sdram_address),
		.o_sdram_wdata(w_sdram_wdata),
		.i_sdram_rdata(w_sdram_rdata),
		.i_sdram_ready(w_sdram_ready)
	);
	
	//====================================================
	// BUS

	// Single port bus.
	wire bus_rw;
	wire bus_request;
	wire bus_ready;
	wire [31:0] bus_address;
	wire [31:0] bus_rdata;
	wire [31:0] bus_wdata;

	wire bus_pa_request;
	wire bus_pa_ready;
	wire [31:0] bus_pa_address;
	wire [31:0] bus_pa_rdata;
	wire bus_pa_busy;

	wire bus_pb_rw;
	wire bus_pb_request;
	wire bus_pb_ready;
	wire [31:0] bus_pb_address;
	wire [31:0] bus_pb_rdata;
	wire [31:0] bus_pb_wdata;

	BusAccess #(
	   .REGISTERED(1)
	) bus(
		.i_reset(reset),
		.i_clock(clock),

		// Bus
		.o_bus_rw(bus_rw),
		.o_bus_request(bus_request),
		.i_bus_ready(bus_ready),
		.o_bus_address(bus_address),
		.i_bus_rdata(bus_rdata),
		.o_bus_wdata(bus_wdata),

		// Port A (Instruction bus)
		.i_pa_request(cpu_ibus_request),
		.o_pa_ready(cpu_ibus_ready),
		.i_pa_address(cpu_ibus_address),
		.o_pa_rdata(cpu_ibus_rdata),
		.o_pa_busy(bus_pa_busy),

		// Port B (Data bus)
		.i_pb_rw(cpu_dbus_rw),
		.i_pb_request(cpu_dbus_request),
		.o_pb_ready(cpu_dbus_ready),
		.i_pb_address(cpu_dbus_address),
		.o_pb_rdata(cpu_dbus_rdata),
		.i_pb_wdata(cpu_dbus_wdata),

		// Port C (DMA)
		.i_pc_rw(dma_bus_rw),
		.i_pc_request(dma_bus_request),
		.o_pc_ready(dma_bus_ready),
		.i_pc_address(dma_bus_address),
		.o_pc_rdata(dma_bus_rdata),
		.i_pc_wdata(dma_bus_wdata)
	);

	//====================================================
	// CPU

	wire cpu_ibus_request;
	wire cpu_ibus_ready;
	wire [31:0] cpu_ibus_address;
	wire [31:0] cpu_ibus_rdata;
	wire cpu_dbus_rw;
	wire cpu_dbus_request;
	wire cpu_dbus_ready;
	wire [31:0] cpu_dbus_address;
	wire [31:0] cpu_dbus_rdata;
	wire [31:0] cpu_dbus_wdata;
	wire cpu_fault;

	CPU #(
		.DCACHE_REGISTERED(0)
	) cpu(
        .i_reset(reset),
		.i_clock(clock),

		// Control
		.i_timer_interrupt(timer_interrupt),
		.i_external_interrupt(plic_interrupt),

		// Instruction bus
		.o_ibus_request(cpu_ibus_request),
		.i_ibus_ready(cpu_ibus_ready),
		.o_ibus_address(cpu_ibus_address),
		.i_ibus_rdata(cpu_ibus_rdata),
		
		// Data bus
		.o_dbus_rw(cpu_dbus_rw),
		.o_dbus_request(cpu_dbus_request),
		.i_dbus_ready(cpu_dbus_ready),
		.o_dbus_address(cpu_dbus_address),
		.i_dbus_rdata(cpu_dbus_rdata),
		.o_dbus_wdata(cpu_dbus_wdata),

		// Debug
		.o_fault(cpu_fault)
	);
	
	//=====================================

	assign rom_select = bus_address[31:28] == 4'h0;
	assign rom_address = { 4'h0, bus_address[27:0] };

	assign ram_select = bus_address[31:28] == 4'h1;
	assign ram_address = { 4'h0, bus_address[27:0] };

	assign sdram_select = bus_address[31:28] == 4'h2;
	assign sdram_address = { 4'h0, bus_address[27:0] };
	
	assign bridge_select = bus_address[31:28] == 4'h5;

	//=====================================

	assign bus_rdata =
		rom_select		? rom_rdata		:
		ram_select		? ram_rdata		:
		sdram_select	? sdram_rdata	:
		bridge_select 	? bridge_rdata	:
		32'h00000000;
		
	assign bus_ready =
		rom_select		? rom_ready		:
		ram_select		? ram_ready		:
		sdram_select	? sdram_ready	:
		bridge_select	? bridge_ready	:
		1'b0;	
	

	//=====================================
	// "NORTH" BRIDGE
	//=====================================


	// LEDS
	wire led_select;
	wire led_ready;
	wire [9:0] led_led;
	LED led(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(led_select && bridge_far_request),
		.i_wdata(bridge_far_wdata),
		.o_ready(led_ready),
		.LEDR(led_led)
	);

	assign hdmi_nreset = led_led[0];


	// UART (FTDI)
	wire uart_0_select;
	wire [1:0] uart_0_address;
	wire [31:0] uart_0_rdata;
	wire uart_0_ready;
	UART #(
		.PRESCALE(`FREQUENCY / (115200 * 8))
	) uart_0(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(uart_0_select && bridge_far_request),
		.i_rw(bridge_far_rw),
		.i_address(uart_0_address),
		.i_wdata(bridge_far_wdata),
		.o_rdata(uart_0_rdata),
		.o_ready(uart_0_ready),
		// ---
		.UART_RX(uart_0_rx),
		.UART_TX(uart_0_tx)
	);

	// UART (IO)
	wire uart_1_select;
	wire [1:0] uart_1_address;
	wire [31:0] uart_1_rdata;
	wire uart_1_ready;
	UART #(
		.PRESCALE(`FREQUENCY / (115200 * 8))
	) uart_1(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(uart_1_select && bridge_far_request),
		.i_rw(bridge_far_rw),
		.i_address(uart_1_address),
		.i_wdata(bridge_far_wdata),
		.o_rdata(uart_1_rdata),
		.o_ready(uart_1_ready),
		// ---
		.UART_RX(uart_1_rx),
		.UART_TX(uart_1_tx)
	);

	// I2C
	wire i2c_select;
	wire [31:0] i2c_rdata;
	wire i2c_ready;
	I2C i2c(
		.i_clock(clock),
		.i_request(i2c_select && bridge_far_request),
		.i_rw(bridge_far_rw),
		.i_wdata(bridge_far_wdata),
		.o_rdata(i2c_rdata),
		.o_ready(i2c_ready),
		// ---
		.I2C_SCL(hdmi_scl),
		.I2C_SDA(hdmi_sda)
	);

	// SD
	wire sd_select;
	wire [31:0] sd_rdata;
	wire sd_ready;
	SD sd(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(sd_select && bridge_far_request),
		.i_rw(bridge_far_rw),
		.i_wdata(bridge_far_wdata),
		.o_rdata(sd_rdata),
		.o_ready(sd_ready),
		// ---
		.SD_CLK(sd_clk),
		.SD_CMD(sd_cmd),
		.SD_DAT(sd_dat)
	);

	// TIMER
	wire timer_select;
	wire [2:0] timer_address;
	wire [31:0] timer_rdata;
	wire timer_ready;
	wire timer_interrupt;
	Timer #(
		.FREQUENCY(`FREQUENCY),
		.DEVICEID(4)
	) timer(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(timer_select && bridge_far_request),
		.i_rw(bridge_far_rw),
		.i_address(timer_address),
		.i_wdata(bridge_far_wdata),
		.o_rdata(timer_rdata),
		.o_ready(timer_ready),
		.o_interrupt(timer_interrupt)
	);

	// AUDIO
	wire audio_output_busy;
	wire [15:0] audio_output_sample;
	wire audio_sdout;
	wire audio_sclk;
	wire audio_lrck;
	wire audio_mclk;

	AUDIO_i2s_output #(
		.FREQUENCY(`FREQUENCY)
	) audio_i2s_output(
		.i_clock(clock),
		
		.o_busy(audio_output_busy),
		.i_sample(audio_output_sample),

		.o_i2s_sdout(audio_sdout),
		.o_i2s_sclk(audio_sclk),
		.o_i2s_lrck(audio_lrck),
		.o_i2s_mclk(audio_mclk)
	);

	// Wire both I2S chip and HDMI output.
	assign i2s_sdout = audio_sdout;
	assign i2s_sclk = audio_sclk;
	assign i2s_lrck = audio_lrck;
	assign i2s_mclk = audio_mclk;

	assign hdmi_sdo = audio_sdout;
	assign hdmi_sck = audio_sclk;
	assign hdmi_ws = audio_lrck;
	assign hdmi_mclk = audio_mclk;

	wire audio_select;
	wire audio_ready;
	AUDIO_controller audio_controller(
		.i_reset(reset),
		.i_clock(clock),

		.i_request(audio_select && bridge_far_request),
		.i_wdata(bridge_far_wdata[15:0]),
		.o_ready(audio_ready),

		.i_output_busy(audio_output_busy),
		.o_output_sample(audio_output_sample)
	);

	// DMA
	wire dma_select;
	wire [1:0] dma_address;
	wire [31:0] dma_rdata;
	wire dma_ready;

	wire dma_bus_rw;
	wire dma_bus_request;
	wire dma_bus_ready;
	wire [31:0] dma_bus_address;
	wire [31:0] dma_bus_rdata;
	wire [31:0] dma_bus_wdata;

	DMA dma(
		.i_reset(reset),
		.i_clock(clock),

		// CPU
		.i_request(dma_select && bridge_far_request),
		.i_rw(bridge_far_rw),
		.i_address(dma_address),
		.i_wdata(bridge_far_wdata),
		.o_rdata(dma_rdata),
		.o_ready(dma_ready),

		// System
		.i_stall(1'b0),
		
		// Bus
		.o_bus_rw(dma_bus_rw),
		.o_bus_request(dma_bus_request),
		.i_bus_ready(dma_bus_ready),
		.o_bus_address(dma_bus_address),
		.i_bus_rdata(dma_bus_rdata),
		.o_bus_wdata(dma_bus_wdata)
	);

	// PLIC
	wire plic_interrupt;
	wire plic_select;
	wire [23:0] plic_address;
	wire [31:0] plic_rdata;
	wire plic_ready;
	PLIC plic(
		.i_reset(reset),
		.i_clock(clock),

		.i_interrupt_0(0),
		.i_interrupt_1(0),
		.i_interrupt_2(0),
		.i_interrupt_3(0),

		.o_interrupt(plic_interrupt),

		.i_request(plic_select && bridge_far_request),
		.i_rw(bridge_far_rw),
		.i_address(plic_address),
		.i_wdata(bridge_far_wdata),
		.o_rdata(plic_rdata),
		.o_ready(plic_ready)
	);

	// VIDEO
	assign hdmi_hsync = ~vga_hsync;
	assign hdmi_vsync = ~vga_vsync;
	assign hdmi_de = vga_enable;
	assign hdmi_idck = ~clock_video;
	assign hdmi_r = vmode_video_rdata[7:0];
	assign hdmi_g = vmode_video_rdata[15:8];
	assign hdmi_b = vmode_video_rdata[23:16];

	// Video signal generator (640*480 @ 60 Hz)
	wire vga_hsync;
	wire vga_vsync;
	wire vga_enable;
	wire [10:0] vga_pos_x;
	wire [10:0] vga_pos_y;
	VIDEO_VGA #(
		.HLINE(800),	// whole line
		.HBACK(48),		// back porch
		.HFRONT(16),	// front porch
		.HPULSE(96),	// sync pulse
		.VLINE(525),	// whole frame
		.VBACK(33),		// back porch
		.VFRONT(10),	// front porch
		.VPULSE(2)		// sync pulse
	) vga(
		.i_clock(clock_video),
		.i_clock_out(clock),
		.o_hsync(vga_hsync),
		.o_vsync(vga_vsync),
		.o_data_enable(vga_enable),
		.o_pos_x(vga_pos_x),
		.o_pos_y(vga_pos_y)
	);

	// Video memory.
	wire vram_pa_request;
	wire vram_pa_rw;
	wire [31:0] vram_pa_address;
	wire [31:0] vram_pa_wdata;
	wire [31:0] vram_pa_rdata;
	wire vram_pa_ready;

	wire vram_pb_request;
	wire vram_pb_rw;
	wire [31:0] vram_pb_address;
	wire [31:0] vram_pb_wdata;
	wire [31:0] vram_pb_rdata;
	wire vram_pb_ready;

	BRAM_1rw1rw #(
		.WIDTH(32),
		.SIZE(2*320*240),
		.ADDR_LSH(2)
	) vram(
		.i_clock(clock),

		.i_pa_request(vram_pa_request),
		.i_pa_rw(vram_pa_rw),
		.i_pa_address(vram_pa_address),
		.i_pa_wdata(vram_pa_wdata),
		.o_pa_rdata(vram_pa_rdata),
		.o_pa_ready(vram_pa_ready),

		.i_pb_request(vram_pb_request),
		.i_pb_rw(vram_pb_rw),
		.i_pb_address(vram_pb_address),
		.i_pb_wdata(vram_pb_wdata),
		.o_pb_rdata(vram_pb_rdata),
		.o_pb_ready(vram_pb_ready)
	);

	// Video mode; chunky 8-bit palette.
	wire vram_select;
	wire [31:0] vram_address;
	wire [31:0] vram_rdata;
	wire vram_ready;	
	wire [31:0] vmode_video_rdata;

	VIDEO_controller #(
		.PPITCH(320)
	) video_controller(
		.i_clock(clock),
		
		// CPU interface.
		.i_cpu_request(vram_select && bridge_far_request),
		.i_cpu_rw(bridge_far_rw),
		.i_cpu_address(vram_address),
		.i_cpu_wdata(bridge_far_wdata),
		.o_cpu_rdata(vram_rdata),
		.o_cpu_ready(vram_ready),
		
		// Video signal interface.
		.i_video_request(vga_enable_scale),
		.i_video_pos_x(vga_pos_x[9:1]),
		.i_video_pos_y(vga_pos_y[9:1]),
		.o_video_rdata(vmode_video_rdata),
		
		// Video RAM interface.
		.o_vram_pa_request(vram_pa_request),
		.o_vram_pa_rw(vram_pa_rw),
		.o_vram_pa_address(vram_pa_address),
		.o_vram_pa_wdata(vram_pa_wdata),
		.i_vram_pa_rdata(vram_pa_rdata),
		.i_vram_pa_ready(vram_pa_ready),

		.o_vram_pb_request(vram_pb_request),
		.o_vram_pb_rw(vram_pb_rw),
		.o_vram_pb_address(vram_pb_address),
		.o_vram_pb_wdata(vram_pb_wdata),
		.i_vram_pb_rdata(vram_pb_rdata),
		.i_vram_pb_ready(vram_pb_ready)
	);

	// Bridge controller.
	wire bridge_select;
	wire [27:0] bridge_address;
	wire [31:0] bridge_rdata;
	wire bridge_ready;

	wire bridge_far_request;
	wire bridge_far_rw;
	wire [27:0] bridge_far_address;
	wire [31:0] bridge_far_wdata;
	wire [31:0] bridge_far_rdata;
	wire bridge_far_ready;

	BRIDGE #(
		.REGISTERED(1)
	) bridge(
		.i_clock		(clock),
		.i_reset		(reset),

		// Near
		.i_request		(bridge_select && bus_request),
		.i_rw			(bus_rw),
		.i_address		(bus_address[27:0]),
		.i_wdata		(bus_wdata),
		.o_rdata		(bridge_rdata),
		.o_ready		(bridge_ready),

		// Far
		.o_far_request	(bridge_far_request),
		.o_far_rw		(bridge_far_rw),
		.o_far_address	(bridge_far_address),
		.o_far_wdata	(bridge_far_wdata),
		.i_far_rdata	(bridge_far_rdata),
		.i_far_ready	(bridge_far_ready)
	);

	assign led_select = bridge_far_address[27:24] == 4'h0;

	assign uart_0_select = bridge_far_address[27:24] == 4'h1;
	assign uart_0_address = bridge_far_address[3:2];

	assign uart_1_select = bridge_far_address[27:24] == 4'h2;
	assign uart_1_address = bridge_far_address[3:2];

	assign i2c_select = bridge_far_address[27:24] == 4'h3;

	assign sd_select = bridge_far_address[27:24] == 4'h4;

	assign timer_select = bridge_far_address[27:24] == 4'h5;
	assign timer_address = bridge_far_address[4:2];

	assign audio_select = bridge_far_address[27:24] == 4'h6;

	assign dma_select = bridge_far_address[27:24] == 4'h7;
	assign dma_address = bridge_far_address[3:2];

	assign plic_select = bridge_far_address[27:24] == 4'h8;
	assign plic_address = bridge_far_address[23:0];

	assign vram_select = bridge_far_address[27:24] == 4'ha;
	assign vram_address = { 8'h0, bridge_far_address[23:0] };

	assign bridge_far_rdata =
		uart_0_select	? uart_0_rdata	:
		uart_1_select	? uart_1_rdata	:
		i2c_select		? i2c_rdata		:
		sd_select		? sd_rdata		:
		timer_select	? timer_rdata	:
		dma_select		? dma_rdata		:
		plic_select		? plic_rdata	:
		vram_select 	? vram_rdata	:
		32'h00000000;
	
	assign bridge_far_ready =
		led_select		? led_ready		:
		uart_0_select	? uart_0_ready	:
		uart_1_select	? uart_1_ready	:
		i2c_select		? i2c_ready		:
		sd_select		? sd_ready		:
		timer_select	? timer_ready	:
		audio_ready		? audio_ready	:
		dma_select		? dma_ready		:
		plic_select		? plic_ready	:
		vram_select		? vram_ready	:
		1'b0;

endmodule
