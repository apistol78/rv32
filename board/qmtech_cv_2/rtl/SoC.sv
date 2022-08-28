
`timescale 1ns/1ns

`define FREQUENCY 100000000

module SoC(
	input sys_clk,			// 50 MHz oscillator
	input sys_reset_n,

	input key_1,		// Active low.

	output led_1,
	output led_2,

	output audio_pwm_left,
	output audio_pwm_right,
	
	input uart_0_rx,
	output uart_0_tx,
	
	input uart_1_rx,
	output uart_1_tx,
	
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
	
	output sdram_clk,
	output sdram_clk_en,
	output sdram_cas_n,
	output sdram_ce_n,
	output sdram_ras_n,
	output sdram_we_n,
	output sdram_dqml_0,	// First chip
	output sdram_dqmh_0,
	output sdram_dqml_1,	// Second chip
	output sdram_dqmh_1,
	output [1:0] sdram_ba,
	output [12:0] sdram_addr,
	inout [31:0] sdram_data,

	output sd_clk,
	inout sd_cmd,
	inout [3:0] sd_dat,

	output [17:0] sram_a,
	output sram_ce_n,
	inout [15:0] sram_d,
	output sram_lb_n,
	output sram_oe_n,
	output sram_ub_n,
	output sram_we_n
);

	wire clock;				// 100MHz
	wire clock_sdram;		// 100MHz, phase shifted 7000 ps.
	wire clock_video;		// 25MHz
	IP_PLL_Clk pll_clk(
		.refclk(sys_clk),
		.rst(~sys_reset_n),
		.outclk_0(clock),
		.outclk_1(clock_sdram),
		.outclk_2(clock_video)
	);

	//=====================================

	bit reset_switch = 1'b0;
	wire reset;
	RESET rst(
		.i_clock(clock),
		.i_reset_sw(reset_switch || ~key_1),
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
    // SDRAM ($20000000)

	wire sdram_select;
	wire [31:0] sdram_address;
	wire [31:0] sdram_rdata;
	wire sdram_ready;

	wire [1:0] sdram_dqm;
	assign sdram_dqml_0 = sdram_dqm[0];
	assign sdram_dqmh_0 = sdram_dqm[1];
	assign sdram_dqml_1 = sdram_dqm[0];
	assign sdram_dqmh_1 = sdram_dqm[1];
	logic [31:0] sdram_data_r;
	wire [31:0] sdram_data_w;
	wire sdram_data_rw;

    SDRAM_controller #(
        .FREQUENCY(100000000),
		.SDRAM_DATA_WIDTH(32)
    ) sdram(
	    .i_reset(reset),
	    .i_clock(clock),
		.i_clock_sdram(clock_sdram),

	    .i_request(sdram_select && bus_request),
	    .i_rw(bus_rw),
	    .i_address(sdram_address),
	    .i_wdata(bus_wdata),
	    .o_rdata(sdram_rdata),
	    .o_ready(sdram_ready),

	    .sdram_clk(sdram_clk),
	    .sdram_clk_en(sdram_clk_en),
	    .sdram_cas_n(sdram_cas_n),
	    .sdram_cs_n(sdram_ce_n),
	    .sdram_ras_n(sdram_ras_n),
	    .sdram_we_n(sdram_we_n),
	    .sdram_dqm(sdram_dqm),
	    .sdram_bs(sdram_ba),		// Called BA in QMTech schematics
	    .sdram_addr(sdram_addr),
		.sdram_rdata(sdram_data_r),
		.sdram_wdata(sdram_data_w),
		.sdram_data_rw(sdram_data_rw)
    );

	assign sdram_data = sdram_data_rw ? sdram_data_w : 32'hz;
	assign sdram_data_r = sdram_data;

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
		.STACK_POINTER(32'h20110000),
		.DCACHE_REGISTERED(1)
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

	// UART (FTDI)
	wire uart_0_select;
	wire [1:0] uart_0_address;
	wire [31:0] uart_0_rdata;
	wire uart_0_ready;
	wire uart_0_interrupt;
	UART #(
		.PRESCALE(`FREQUENCY / (115200 * 8)),
		.RX_FIFO_DEPTH(16)
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
		.UART_RX(uart_0_rx),
		.UART_TX(uart_0_tx)
	);

	// UART (SHID)
	wire uart_1_select;
	wire [1:0] uart_1_address;
	wire [31:0] uart_1_rdata;
	wire uart_1_ready;
	wire uart_1_interrupt;
	UART #(
		.PRESCALE(`FREQUENCY / (115200 * 8)),
		.RX_FIFO_DEPTH(256)
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

	wire audio_pwm;
	AUDIO_pwm_output audio_pwm_output(
		.i_clock(clock),
		.o_busy(audio_output_busy),
		.i_sample(audio_output_sample),
		.o_pwm(audio_pwm)
	);
	assign audio_pwm_left = audio_pwm;
	assign audio_pwm_right = audio_pwm;

	wire audio_select;
	wire [31:0] audio_rdata;
	wire audio_ready;
	wire audio_interrupt;
	AUDIO_controller audio_controller(
		.i_reset(reset),
		.i_clock(clock),

		.i_request(audio_select && bridge_far_request),
		.i_rw(bridge_far_rw),
		.i_wdata(bridge_far_wdata[15:0]),
		.o_rdata(audio_rdata),
		.o_ready(audio_ready),
		.o_interrupt(audio_interrupt),

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

		.i_interrupt_0(0),					// Video
		.i_interrupt_1(audio_interrupt),	// Audio
		.i_interrupt_2(0), // uart_0_interrupt | uart_1_interrupt),	// UART
		.i_interrupt_3(0),

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
	wire [7:0] sysreg_leds;
	wire sysreg_sil9024_reset;
	SystemRegisters #(
		.FREQUENCY(`FREQUENCY),
		.DEVICEID(6),
		.RAM_SIZE(32'h1000000)
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
		.i_boot_mode_switch(1'b0),	// 0 - read elf, 1 - wait on uart
		.o_reset_switch(reset_switch),
		.o_leds(sysreg_leds),
		.o_sil9024_reset(sysreg_sil9024_reset)
	);

	assign led_1 = ~sysreg_leds[0];
	assign led_2 = ~sysreg_leds[1];
	assign hdmi_nreset = ~sysreg_sil9024_reset;

	// VIDEO
	assign hdmi_hsync = ~vga_hsync;
	assign hdmi_vsync = vga_vsync;
	assign hdmi_de = vga_enable;
	assign hdmi_idck = clock_video;
	assign hdmi_r = vmode_video_rdata[7:0];
	assign hdmi_g = vmode_video_rdata[15:8];
	assign hdmi_b = vmode_video_rdata[23:16];

	// Video signal generator.
	wire vga_hsync;
	wire vga_vsync;
	wire vga_enable;
	wire [10:0] vga_pos_x;
	wire [10:0] vga_pos_y;
	VIDEO_VGA #(
		
		// 640 * 480 * 60hz
		// .HLINE(800),	// whole line
		// .HBACK(48),		// back porch
		// .HFRONT(16),	// front porch
		// .HPULSE(96),	// sync pulse
		// .VLINE(525),	// whole frame
		// .VBACK(33),		// back porch
		// .VFRONT(10),	// front porch
		// .VPULSE(2)		// sync pulse

		// 640 * 400 * 70hz
		.HLINE(800),	// whole line
		.HBACK(48),		// back porch
		.HFRONT(16),	// front porch
		.HPULSE(96),	// sync pulse
		.VLINE(449),	// whole frame
		.VBACK(35),		// back porch
		.VFRONT(12),	// front porch
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
		.SRAM_A(sram_a),
		.SRAM_D(sram_d),
		.SRAM_CE_n(sram_ce_n),
		.SRAM_OE_n(sram_oe_n),
		.SRAM_WE_n(sram_we_n),
		.SRAM_LB_n(sram_lb_n),
		.SRAM_UB_n(sram_ub_n)
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
	wire vram_pb_busy;

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
		.o_pa_busy(),

		// Video output access.
		.i_pb_rw(vram_pb_rw),
		.i_pb_request(vram_pb_request),
		.o_pb_ready(vram_pb_ready),
		.i_pb_address(vram_pb_address),
		.o_pb_rdata(vram_pb_rdata),
		.i_pb_wdata(vram_pb_wdata),
		.o_pb_busy(vram_pb_busy),

		// Video CPU access.
		.i_pc_rw(vram_pa_rw),
		.i_pc_request(vram_pa_request),
		.o_pc_ready(vram_pa_ready),
		.i_pc_address(vram_pa_address),
		.o_pc_rdata(vram_pa_rdata),
		.i_pc_wdata(vram_pa_wdata),
		.o_pc_busy()
	);

	// Video mode; chunky 8-bit palette.
	wire vram_select;
	wire [31:0] vram_address;
	wire [31:0] vram_rdata;
	wire vram_ready;	
	wire [31:0] vmode_video_rdata;

	VMODE_chunky #(
		.PPITCH(320)
	) vmode_chunky(
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

	assign sysreg_select = bridge_far_address[27:24] == 4'h9;
	assign sysreg_address = bridge_far_address[4:2];

	assign vram_select = bridge_far_address[27:24] == 4'ha;
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
		vram_select 	? vram_rdata	:
		32'h00000000;
	
	assign bridge_far_ready =
		uart_0_select	? uart_0_ready	:
		uart_1_select	? uart_1_ready	:
		i2c_select		? i2c_ready		:
		sd_select		? sd_ready		:
		timer_select	? timer_ready	:
		audio_ready		? audio_ready	:
		dma_select		? dma_ready		:
		plic_select		? plic_ready	:
		sysreg_select	? sysreg_ready	:
		vram_select		? vram_ready	:
		1'b0;

endmodule
