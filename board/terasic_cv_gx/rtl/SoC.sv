/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/

`define ENABLE_DDR2LP
//`define ENABLE_HSMC_XCVR
//`define ENABLE_SMA
//`define ENABLE_REFCLK
`define ENABLE_GPIO

`timescale 1ns/1ns

module SoC(

	///////// ADC ///////// 1.2 V ///////
	output             ADC_CONVST,
	output             ADC_SCK,
	output             ADC_SDI,
	input              ADC_SDO,

	///////// AUD ///////// 2.5 V ///////
	input              AUD_ADCDAT,
	inout              AUD_ADCLRCK,
	inout              AUD_BCLK,
	output             AUD_DACDAT,
	inout              AUD_DACLRCK,
	output             AUD_XCK,

	///////// CLOCK /////////
	input              CLOCK_125_p, ///LVDS
	input              CLOCK_50_B5B, ///3.3-V LVTTL
	input              CLOCK_50_B6A,
	input              CLOCK_50_B7A, ///2.5 V
	input              CLOCK_50_B8A,

	///////// CPU /////////
	input              CPU_RESET_n, ///3.3V LVTTL

`ifdef ENABLE_DDR2LP
	///////// DDR2LP ///////// 1.2-V HSUL ///////
	output      [9:0]  DDR2LP_CA,
	output      [1:0]  DDR2LP_CKE,
	output             DDR2LP_CK_n, ///DIFFERENTIAL 1.2-V HSUL
	output             DDR2LP_CK_p, ///DIFFERENTIAL 1.2-V HSUL
	output      [1:0]  DDR2LP_CS_n,
	output      [3:0]  DDR2LP_DM,
	inout       [31:0] DDR2LP_DQ,
	inout       [3:0]  DDR2LP_DQS_n, ///DIFFERENTIAL 1.2-V HSUL
	inout       [3:0]  DDR2LP_DQS_p, ///DIFFERENTIAL 1.2-V HSUL
	input              DDR2LP_OCT_RZQ, ///1.2 V
`endif /*ENABLE_DDR2LP*/

	input				uart_1_rx,	// PIN_T21	GPIO[0]
	output				uart_1_tx,	// PIN_D26	GPIO[1]
/*
`ifdef ENABLE_GPIO
	///////// GPIO ///////// 3.3-V LVTTL ///////
	inout       [35:0] GPIO,
`else	
	///////// HEX2 ///////// 1.2 V ///////
	output      [6:0]  HEX2,

	///////// HEX3 ///////// 1.2 V ///////
	output      [6:0]  HEX3,		
`endif
*/

	///////// HDMI /////////
	output             HDMI_TX_CLK,
	output      [23:0] HDMI_TX_D,
	output             HDMI_TX_DE,
	output             HDMI_TX_HS,
	input              HDMI_TX_INT,
	output             HDMI_TX_VS,

	///////// HEX0 /////////
	output      [6:0]  HEX0,

	///////// HEX1 /////////
	output      [6:0]  HEX1,


	///////// HSMC ///////// 2.5 V ///////
	input              HSMC_CLKIN0,
	input       [2:1]  HSMC_CLKIN_n,
	input       [2:1]  HSMC_CLKIN_p,
	output             HSMC_CLKOUT0,
	output      [2:1]  HSMC_CLKOUT_n,
	output      [2:1]  HSMC_CLKOUT_p,
	inout       [3:0]  HSMC_D,
`ifdef ENABLE_HSMC_XCVR		
	input       [3:0]  HSMC_GXB_RX_p, /// 1.5-V PCML
	output      [3:0]  HSMC_GXB_TX_p, /// 1.5-V PCML
`endif /*ENABLE_HSMC_XCVR*/		
	inout       [16:0] HSMC_RX_n,
	inout       [16:0] HSMC_RX_p,
	inout       [16:0] HSMC_TX_n,
	inout       [16:0] HSMC_TX_p,


	///////// I2C ///////// 2.5 V ///////
	output             I2C_SCL,
	inout              I2C_SDA,

	///////// KEY ///////// 1.2 V ///////
	input       [3:0]  KEY,

	///////// LEDG ///////// 2.5 V ///////
	output      [7:0]  LEDG,

	///////// LEDR ///////// 2.5 V ///////
	output      [9:0]  LEDR,

`ifdef ENABLE_REFCLK
	///////// REFCLK ///////// 1.5-V PCML ///////
	input              REFCLK_p0,
	input              REFCLK_p1,
`endif /*ENABLE_REFCLK*/

	///////// SD ///////// 3.3-V LVTTL ///////
	output             SD_CLK,
	inout              SD_CMD,
	inout       [3:0]  SD_DAT,

`ifdef ENABLE_SMA
	///////// SMA ///////// 1.5-V PCML ///////
	input              SMA_GXB_RX_p,
	output             SMA_GXB_TX_p,
`endif /*ENABLE_SMA*/

	///////// SRAM ///////// 3.3-V LVTTL ///////
	output      [17:0] SRAM_A,
	output             SRAM_CE_n,
	inout       [15:0] SRAM_D,
	output             SRAM_LB_n,
	output             SRAM_OE_n,
	output             SRAM_UB_n,
	output             SRAM_WE_n,

	///////// SW ///////// 1.2 V ///////
	input       [9:0]  SW,

	///////// UART ///////// 2.5 V ///////
	input              UART_RX,
	output             UART_TX
);

`define FREQUENCY 100000000

	wire clock;
	wire clock_video;
	
	IP_PLL_Clk pll_clk(
		.refclk(CLOCK_125_p),
		.rst(!CPU_RESET_n),
		.outclk_0(clock),		// 100 MHz
		.outclk_1(clock_video),	// 25 MHz
		.locked()
	);

	//=====================================

	wire soft_reset;
	wire global_reset;
	wire reset;
	RESET rst(
		.i_clock(clock),
		.i_reset_sw(KEY[0]),
		.o_reset_0(soft_reset),
		.o_reset_1(global_reset),
		.o_reset_2(reset)
	);

	wire soft_reset_n = !soft_reset;
	wire global_reset_n = !global_reset;

	//=====================================
	// ROM
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
	// RAM
	wire ram_select;
	wire [31:0] ram_address;
	wire [31:0] ram_rdata;
	wire ram_ready;
	BRAM #(
		.WIDTH(32),
		.SIZE(32'h200),
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
	// SDRAM
	wire sdram_select;
	wire [31:0] sdram_address;
	wire [31:0] sdram_rdata;
	wire sdram_ready;

	SDRAM_interface sdram(
		.i_global_reset_n(global_reset_n),
		.i_soft_reset_n(soft_reset_n),
		.i_clock_pll_ref(clock),
		// ---
		.i_reset(reset),
		.i_clock(clock),
		.i_request(sdram_select && bus_request),
		.i_rw(bus_rw),
		.i_address(sdram_address),
		.i_wdata(bus_wdata),
		.o_rdata(sdram_rdata),
		.o_ready(sdram_ready),
		// ---
		.DDR2LP_CA(DDR2LP_CA),
		.DDR2LP_CKE(DDR2LP_CKE),
		.DDR2LP_CK_n(DDR2LP_CK_n),
		.DDR2LP_CK_p(DDR2LP_CK_p),
		.DDR2LP_CS_n(DDR2LP_CS_n),
		.DDR2LP_DM(DDR2LP_DM),
		.DDR2LP_DQ(DDR2LP_DQ),
		.DDR2LP_DQS_n(DDR2LP_DQS_n),
		.DDR2LP_DQS_p(DDR2LP_DQS_p),
		.DDR2LP_OCT_RZQ(DDR2LP_OCT_RZQ)
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
	
	// 7:0
	assign LEDG = { uart_1_rx, uart_1_tx, cpu_fault };

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
		.LEDR(LEDR)
	);

	// UART (FTDI)
	wire uart_0_select;
	wire [1:0] uart_0_address;
	wire [31:0] uart_0_rdata;
	wire uart_0_ready;
	wire uart_0_interrupt;
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
		.o_interrupt(uart_0_interrupt),
		// ---
		.UART_RX(UART_RX),
		.UART_TX(UART_TX)
	);

	// UART (SHID)
	wire uart_1_select;
	wire [1:0] uart_1_address;
	wire [31:0] uart_1_rdata;
	wire uart_1_ready;
	wire uart_1_interrupt;
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
		.o_interrupt(uart_1_interrupt),
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
		.I2C_SCL(I2C_SCL),
		.I2C_SDA(I2C_SDA)
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
		.SD_CLK(SD_CLK),
		.SD_CMD(SD_CMD),
		.SD_DAT(SD_DAT)
	);

	// TIMER
	wire timer_select;
	wire [2:0] timer_address;
	wire [31:0] timer_rdata;
	wire timer_ready;
	wire timer_interrupt;
	Timer #(
		.FREQUENCY(`FREQUENCY),
		.DEVICEID(2)
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

		.i_interrupt_0(HDMI_TX_INT),
		.i_interrupt_1(0),
		.i_interrupt_2(uart_0_interrupt | uart_1_interrupt),
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
	assign HDMI_TX_DE = vga_enable;
	assign HDMI_TX_CLK = clock_video;

	// Video signal generator
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

	assign HDMI_TX_HS = vga_hsync;
	assign HDMI_TX_VS = vga_vsync;	
	
	// Video physical memory.
	wire video_sram_request;
	wire video_sram_rw;
	wire [31:0] video_sram_address;
	wire [31:0] video_sram_wdata;
	wire [31:0] video_sram_rdata;
	wire video_sram_ready;
	SRAM_interface video_sram(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(video_sram_request),
		.i_rw(video_sram_rw),
		.i_address(video_sram_address),
		.i_wdata(video_sram_wdata),
		.o_rdata(video_sram_rdata),
		.o_ready(video_sram_ready),
		// ---
		.SRAM_A(SRAM_A),
		.SRAM_D(SRAM_D),
		.SRAM_CE_n(SRAM_CE_n),
		.SRAM_OE_n(SRAM_OE_n),
		.SRAM_WE_n(SRAM_WE_n),
		.SRAM_LB_n(SRAM_LB_n),
		.SRAM_UB_n(SRAM_UB_n)
	);

	// Video memory dual port.
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

	BusAccess #(
		.REGISTERED(0)
	) vram_bus(
		.i_reset(reset),
		.i_clock(clock),

		.o_bus_rw(video_sram_rw),
		.o_bus_request(video_sram_request),
		.i_bus_ready(video_sram_ready),
		.o_bus_address(video_sram_address),
		.i_bus_rdata(video_sram_rdata),
		.o_bus_wdata(video_sram_wdata),

		.i_pa_request(1'b0),
		.o_pa_ready(),
		.i_pa_address(32'h0),
		.o_pa_rdata(),

		.i_pb_rw(vram_pb_rw),
		.i_pb_request(vram_pb_request),
		.o_pb_ready(vram_pb_ready),
		.i_pb_address(vram_pb_address),
		.o_pb_rdata(vram_pb_rdata),
		.i_pb_wdata(vram_pb_wdata),

		.i_pc_rw(vram_pa_rw),
		.i_pc_request(vram_pa_request),
		.o_pc_ready(vram_pa_ready),
		.i_pc_address(vram_pa_address),
		.o_pc_rdata(vram_pa_rdata),
		.i_pc_wdata(vram_pa_wdata)
	);

	// Video mode; chunky 8-bit palette.
	wire vram_select;
	wire [31:0] vram_address;
	wire [31:0] vram_rdata;
	wire vram_ready;

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
		.i_video_hsync(vga_hsync),
		.i_video_vsync(vga_vsync),
		.i_video_request(vga_enable),
		.i_video_pos_x(vga_pos_x[9:1]),
		.i_video_pos_y(vga_pos_y[9:1]),
		.o_video_rdata(HDMI_TX_D),
		
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
		.REGISTERED(0)
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
		dma_select		? dma_ready		:
		plic_select		? plic_ready	:
		vram_select		? vram_ready	:
		1'b0;

endmodule
