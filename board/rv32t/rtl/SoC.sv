/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
`include "CPU_Defines.sv"

`timescale 1ns/1ns
`define FREQUENCY 100000000

module SoC(

	output				AUDIO_PWM,

	input				CLOCK_125_p,

	input 				CPU_RESET_n,

	output             	HDMI_TX_CLK,
	output      [23:0] 	HDMI_TX_D,
	output             	HDMI_TX_DE,
	output             	HDMI_TX_HS,
	input              	HDMI_TX_INT,
	output             	HDMI_TX_VS,

	output             	I2C_SCL,
	inout              	I2C_SDA,

	input       [3:0]  	KEY,

	output      [9:0]  	LEDR,

	output             	SD_CLK,
	input              	SD_CMD_in,
	input       [3:0]  	SD_DAT_in,
	output             	SD_CMD_out,
	output      [3:0]  	SD_DAT_out,

	input              	UART_RX,
	output             	UART_TX,

	input              	UART_1_RX,
	output             	UART_1_TX,

	// CPU diagnostics
	output [31:0] o_icache_hit,
	output [31:0] o_icache_miss,
	output [31:0] o_dcache_hit,
	output [31:0] o_dcache_miss,
	output o_execute_busy,
	output o_memory_busy,

	// BUS diagnostics
	output bit o_debug_bus_request,
	output bit o_debug_bus_rw,
	output bit [13:0] o_debug_bus_select,
	output bit o_debug_bus_fault,
	output bit [31:0] o_debug_bus_fault_address,
	output bit [1:0] o_debug_bus_fault_type
);

	// Since we want to share pins with HW
	// this clock will actually be simulated at 100 MHz.
	wire clock = CLOCK_125_p;
	wire reset = !CPU_RESET_n;
  
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
	// SDRAM
	wire sdram_rw_0;
	wire sdram_request_0;
	wire [31:0] sdram_address_0;
	wire [31:0] sdram_wdata_0;
	wire [31:0] sdram_rdata_0;
	wire sdram_ready_0;

	BRAM #(
		.WIDTH(32),
		.SIZE(32'h2000000 / 4),
		.ADDR_LSH(2)
	) sdram(
		.i_clock(clock),
		.i_request(sdram_request_0),
		.i_rw(sdram_rw_0),
		.i_address(sdram_address_0),
		.i_wdata(sdram_wdata_0),
		.o_rdata(sdram_rdata_0),
		.o_ready(sdram_ready_0),
		.o_valid()		
	);
	

	wire sdram_select;
	wire [31:0] sdram_address;
	wire [31:0] sdram_rdata;
	wire sdram_ready;

	WriteBuffer #(
		.DEPTH(4),
		.STALL_READ(1)
	) sdram_wb(
		.i_reset(reset),
		.i_clock(clock),
		
		.o_empty(),
		.o_full(),

		.o_bus_rw(sdram_rw_0),
		.o_bus_request(sdram_request_0),
		.i_bus_ready(sdram_ready_0),
		.o_bus_address(sdram_address_0),
		.i_bus_rdata(sdram_rdata_0),
		.o_bus_wdata(sdram_wdata_0),

		.i_rw(bus_rw),
		.i_request(sdram_select && bus_request),
		.o_ready(sdram_ready),
		.i_address(sdram_address),
		.o_rdata(sdram_rdata),
		.i_wdata(bus_wdata)
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

	BusValidator #(.NAME("BUS")) bus_validator(
		.i_clock(clock),
		.i_bus_request(bus_request),
		.i_bus_ready(bus_ready)
	);

	//=====================================
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
		.FREQUENCY(`FREQUENCY),
		.STACK_POINTER(32'h20110000),
		.DCACHE_REGISTERED(1),
		.ICACHE_REGISTERED(1)
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
		.o_icache_hit(o_icache_hit),
		.o_icache_miss(o_icache_miss),
		.o_dcache_hit(o_dcache_hit),
		.o_dcache_miss(o_dcache_miss),
		.o_execute_busy(o_execute_busy),
		.o_memory_busy(o_memory_busy),
		.o_fault(cpu_fault)
	);

	BusValidator #(.NAME("DBUS")) dbus_validator(
		.i_clock(clock),
		.i_bus_request(cpu_dbus_request),
		.i_bus_ready(cpu_dbus_ready)
	);
	BusValidator #(.NAME("IBUS")) ibus_validator(
		.i_clock(clock),
		.i_bus_request(cpu_ibus_request),
		.i_bus_ready(cpu_ibus_ready)
	);

	//=====================================

	assign rom_select = bus_address[31:28] == 4'h0;
	assign rom_address = { 4'h0, bus_address[27:0] };

	assign sdram_select = bus_address[31:28] == 4'h2;
	assign sdram_address = { 4'h0, bus_address[27:0] };

	assign bridge_select = bus_address[31:28] == 4'h5;

	//=====================================

	assign bus_rdata =
		rom_select		? rom_rdata		:
		sdram_select	? sdram_rdata	:
		bridge_select 	? bridge_rdata	:
		32'h00000000;
		
	assign bus_ready =
		rom_select		? rom_ready		:
		sdram_select	? sdram_ready	:
		bridge_select	? bridge_ready	:
		1'b0;


	//=====================================
	// "NORTH" BRIDGE
	//=====================================
	
	// UART (USB)
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

	// UART (IO)
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
		.UART_RX(UART_1_RX),
		.UART_TX(UART_1_TX)
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
	wire [1:0] sd_address;
	wire [31:0] sd_rdata;
	wire sd_ready;
	SD sd(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(sd_select && bridge_far_request),
		.i_rw(bridge_far_rw),
		.i_address(sd_address),
		.i_wdata(bridge_far_wdata),
		.o_rdata(sd_rdata),
		.o_ready(sd_ready),
		// ---
		.SD_CLK(SD_CLK),
		.SD_CMD_in(SD_CMD_in),
		.SD_DAT_in(SD_DAT_in),
		.SD_CMD_out(SD_CMD_out),
		.SD_DAT_out(SD_DAT_out)
	);

	// TIMER
	wire timer_select;
	wire [3:0] timer_address;
	wire [31:0] timer_rdata;
	wire timer_ready;
	wire timer_interrupt;
	Timer #(
		.FREQUENCY(`FREQUENCY)
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
	wire [31:0] audio_output_reload;

	AUDIO_pwm_output audio_pwm_output(
		.i_clock(clock),
		.o_busy(audio_output_busy),
		.i_sample(audio_output_sample),
		.i_reload(audio_output_reload),
		.o_pwm(AUDIO_PWM)
	);

	wire audio_select;
	wire [3:0] audio_address;
	wire [31:0] audio_rdata;
	wire audio_ready;
	wire audio_interrupt;
	AUDIO_controller audio_controller(
		.i_reset(reset),
		.i_clock(clock),

		.i_request(audio_select && bridge_far_request),
		.i_rw(bridge_far_rw),
		.i_address(audio_address),
		.i_wdata(bridge_far_wdata[15:0]),
		.o_rdata(audio_rdata),
		.o_ready(audio_ready),
		.o_interrupt(audio_interrupt),

		.i_output_busy(audio_output_busy),
		.o_output_sample(audio_output_sample),
		.o_output_reload(audio_output_reload)
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
	bit [1:0] vb = 2'b00;
	always_ff @(posedge clock)
		vb <= { vb[0], ~vga_vblank };

	wire plic_interrupt;
	wire plic_select;
	wire [23:0] plic_address;
	wire [31:0] plic_rdata;
	wire plic_ready;
	PLIC plic(
		.i_reset(reset),
		.i_clock(clock),

		.i_interrupt_0(vb == 2'b01),		// Video vertical blank.
		.i_interrupt_1(audio_interrupt),	// Audio
		.i_interrupt_2(0),
		.i_interrupt_3(0),

		.i_interrupt_enable(1'b1),
		.o_interrupt(plic_interrupt),

		.i_request(plic_select && bridge_far_request),
		.i_rw(bridge_far_rw),
		.i_address(plic_address),
		.i_wdata(bridge_far_wdata),
		.o_rdata(plic_rdata),
		.o_ready(plic_ready)
	);

	// System registers.
	wire sysreg_select;
	wire [2:0] sysreg_address;
	wire [31:0] sysreg_rdata;
	wire sysreg_ready;	
	SystemRegisters #(
		.FREQUENCY(`FREQUENCY),
		.DEVICEID(1),
		.RAM_SIZE(32'h2000000),
		.BOOTMODE(0)
	) sysreg(
		.i_reset(reset),
		.i_clock(clock),

		// CPU
		.i_request(sysreg_select && bridge_far_request),
		.i_rw(bridge_far_rw),
		.i_address(sysreg_address),
		.i_wdata(bridge_far_wdata),
		.o_rdata(sysreg_rdata),
		.o_ready(sysreg_ready),

		// Signals
		.o_reset_switch(),
		.o_leds(LEDR),
		.o_sil9024_reset()
	);

	// VIDEO
	// Delay video signals to improve timing with controller.
	bit [7:0] dly_vga_data_enable;
	bit [7:0] dly_vga_clock;
	bit [7:0] dly_vga_hsync;
	bit [7:0] dly_vga_vsync;
	always_ff @(posedge clock) begin
		dly_vga_data_enable <= { dly_vga_data_enable[6:0], vga_data_enable };
		dly_vga_clock <= { dly_vga_clock[6:0], vga_clock };
		dly_vga_hsync <= { dly_vga_hsync[6:0], vga_hsync };
		dly_vga_vsync <= { dly_vga_vsync[6:0], vga_vsync };
	end

	assign HDMI_TX_DE = dly_vga_data_enable[2];
	assign HDMI_TX_CLK = dly_vga_clock[2];
	assign HDMI_TX_HS = dly_vga_hsync[2];
	assign HDMI_TX_VS = dly_vga_vsync[2];

	// Video clock generator.
	wire vga_clock;
	ClockDivider #(
		.CLOCK_RATE(`FREQUENCY),
		.BAUD_RATE(25_000_000)
	) vga_clock_divider(
		.i_reset(reset),
		.i_clock(clock),
		.o_clock(vga_clock)
	);

	// Video signal generator.
	wire vga_hsync;
	wire vga_vsync;
	wire vga_hblank;
	wire vga_vblank;
	wire vga_data_enable;
	wire [10:0] vga_pos_x;
	wire [10:0] vga_pos_y;

	VIDEO_VGA #(
		/*
		// 640 * 480 * 60hz
		.HLINE(800),	// whole line
		.HBACK(48),		// back porch
		.HFRONT(16),	// front porch
		.HPULSE(96),	// sync pulse
		.VLINE(525),	// whole frame
		.VBACK(33),		// back porch
		.VFRONT(10),	// front porch
		.VPULSE(2)		// sync pulse
		.VSPOL(0),
		.HSPOL(0)
		*/
		// 640 * 400 * 70hz
		.HLINE(800),	// whole line
		.HBACK(48),		// back porch
		.HFRONT(16),	// front porch
		.HPULSE(96),	// sync pulse
		.VLINE(449),	// whole frame
		.VBACK(35),		// back porch
		.VFRONT(12),	// front porch
		.VPULSE(2),		// sync pulse
		.VSPOL(1),
		.HSPOL(0)
	) vga(
		.i_clock(vga_clock),
		.i_clock_out(clock),
		.o_clock(),
		.o_hsync(vga_hsync),
		.o_vsync(vga_vsync),
		.o_hblank(vga_hblank),
		.o_vblank(vga_vblank),
		.o_data_enable(vga_data_enable),
		.o_pos_x(vga_pos_x),
		.o_pos_y(vga_pos_y)
	);

	// Video physical memory.
	wire video_sram_request;
	wire video_sram_rw;
	wire [31:0] video_sram_address;
	wire [31:0] video_sram_wdata;
	wire [31:0] video_sram_rdata;
	wire video_sram_ready;
	BRAM_latency #(
		.WIDTH(32),
		.SIZE(640 * 400 * 2),
		.ADDR_LSH(2),
		.LATENCY(11)
	) video_sram(
		.i_clock(clock),
		.i_request(video_sram_request),
		.i_rw(video_sram_rw),
		.i_address(video_sram_address),
		.i_wdata(video_sram_wdata),
		.o_rdata(video_sram_rdata),
		.o_ready(video_sram_ready),
		.o_valid()
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

	DualPort vram_bus(
		.i_reset(reset),
		.i_clock(clock),

		.o_bus_rw(video_sram_rw),
		.o_bus_request(video_sram_request),
		.i_bus_ready(video_sram_ready),
		.o_bus_address(video_sram_address),
		.i_bus_rdata(video_sram_rdata),
		.o_bus_wdata(video_sram_wdata),

		// Video output access.
		.i_pb_rw(vram_pb_rw),
		.i_pb_request(vram_pb_request),
		.o_pb_ready(vram_pb_ready),
		.i_pb_address(vram_pb_address),
		.o_pb_rdata(vram_pb_rdata),
		.i_pb_wdata(vram_pb_wdata),

		// Video CPU access.
		.i_pc_rw(vram_pa_rw),
		.i_pc_request(vram_pa_request),
		.o_pc_ready(vram_pa_ready),
		.i_pc_address(vram_pa_address),
		.o_pc_rdata(vram_pa_rdata),
		.i_pc_wdata(vram_pa_wdata)
	);

	BusValidator #(.NAME("VBUS")) vbus_validator(
		.i_clock(clock),
		.i_bus_request(vram_pb_request),
		.i_bus_ready(vram_pb_ready)
	);

	// Video mode; chunky 8-bit palette.
	wire vram_select;
	wire [31:0] vram_address;
	wire [31:0] vram_rdata;
	wire vram_ready;	

	VIDEO_controller #(
		.MAX_PITCH(640)
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
		.i_video_hblank(vga_hblank),
		.i_video_vblank(vga_vblank),
		.i_video_pos_x(vga_pos_x),
		.i_video_pos_y(vga_pos_y),
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

	// BRIDGE
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

	// BusValidator #(.NAME("BRIDGE FAR")) bridge_far_validator(
	// 	.i_clock(clock),
	// 	.i_bus_request(bridge_far_request),
	// 	.i_bus_ready(bridge_far_ready)
	// );

	assign uart_0_select = bridge_far_address[27:24] == 4'h1;
	assign uart_0_address = bridge_far_address[3:2];

	assign uart_1_select = bridge_far_address[27:24] == 4'h2;
	assign uart_1_address = bridge_far_address[3:2];

	assign i2c_select = bridge_far_address[27:24] == 4'h3;

	assign sd_select = bridge_far_address[27:24] == 4'h4;
	assign sd_address = bridge_far_address[3:2];

	assign timer_select = bridge_far_address[27:24] == 4'h5;
	assign timer_address = bridge_far_address[5:2];

	assign audio_select = bridge_far_address[27:24] == 4'h6;
	assign audio_address = bridge_far_address[5:2];

	assign dma_select = bridge_far_address[27:24] == 4'h7;
	assign dma_address = bridge_far_address[3:2];

	assign plic_select = bridge_far_address[27:24] == 4'h8;
	assign plic_address = bridge_far_address[23:0];

	assign sysreg_select = bridge_far_address[27:24] == 4'h9;
	assign sysreg_address = bridge_far_address[4:2];

	assign vram_select = bus_address[27:24] == 4'ha;
	assign vram_address = { 8'h0, bridge_far_address[23:0] };

	assign bridge_far_rdata =
		uart_0_select	? uart_0_rdata	:
		uart_1_select	? uart_1_rdata	:
		i2c_select		? i2c_rdata		:
		sd_select		? sd_rdata		:
		timer_select	? timer_rdata	:
		audio_select	? audio_rdata	:
		dma_select		? dma_rdata		:
		plic_select		? plic_rdata	:
		sysreg_select	? sysreg_rdata	:
		vram_select		? vram_rdata	:
		32'h00000000;
	
	assign bridge_far_ready =
		uart_0_select	? uart_0_ready	:
		uart_1_select	? uart_1_ready	:
		i2c_ready		? i2c_ready		:
		sd_select		? sd_ready		:
		timer_select	? timer_ready	:
		audio_ready		? audio_ready	:
		dma_select		? dma_ready		:
		plic_select		? plic_ready	:
		sysreg_select	? sysreg_ready	:
		vram_select		? vram_ready	:
		1'b0;


	//=====================================
	// DEBUG

	wire bus_valid_select =
		rom_select				|
		sdram_select			|
		vram_select				|
		bridge_select			|
		sd_select				|
		i2c_select				|
		audio_select			|
		dma_select				|
		timer_select			|
		plic_select				|
		sysreg_select;

	always_comb begin
		o_debug_bus_fault = !bus_valid_select && bus_request;
		o_debug_bus_fault_address = bus_address;
		o_debug_bus_fault_type =
			cpu_ibus_request ? 2'd1 :
			cpu_dbus_request ? 2'd2 :
			dma_bus_request ? 2'd3 :
			2'd0;

		o_debug_bus_request = bus_request;
		o_debug_bus_rw = bus_rw;
		o_debug_bus_select =
		{
			rom_select,
			sdram_select,
			vram_select,
			uart_0_select,
			uart_1_select,
			sd_select,
			i2c_select,
			audio_select,
			dma_select,
			timer_select,
			plic_select,
			sysreg_select
		};
	end

endmodule
