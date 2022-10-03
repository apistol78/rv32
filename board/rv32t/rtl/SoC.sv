`include "CPU_Defines.sv"

`timescale 1ns/1ns

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
	output             	UART_1_TX
);

`define FREQUENCY 100000000

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
	// wire w_sdram_request;
	// wire w_sdram_rw;
	// wire [31:0] w_sdram_address;
	// wire [127:0] w_sdram_wdata;
	// wire [127:0] w_sdram_rdata;
	// wire w_sdram_ready;
	// wire w_sdram_valid;
	
	wire sdram_select;
	wire [31:0] sdram_address;
	wire [31:0] sdram_rdata;
	wire sdram_ready;

	BRAM_latency #(
		.WIDTH(32),
		.SIZE(32'h2000000 / 4),
		.ADDR_LSH(2)
		// .SIZE(32'h1000000 / 16),
		// .ADDR_LSH(4),
		// .LATENCY(16)
	) sdram(
		.i_clock(clock),

		// .i_request(w_sdram_request),
		// .i_rw(w_sdram_rw),
		// .i_address(w_sdram_address),
		// .i_wdata(w_sdram_wdata),
		// .o_rdata(w_sdram_rdata),
		// .o_ready(w_sdram_ready),
		// .o_valid(w_sdram_valid)

		.i_request(sdram_select && bus_request),
		.i_rw(bus_rw),
		.i_address(sdram_address),
		.i_wdata(bus_wdata),
		.o_rdata(sdram_rdata),
		.o_ready(sdram_ready),
		.o_valid()		
	);

	// LRU_cache sdram_lru(
	// 	.i_clock(clock),

	// 	.i_request(sdram_select && bus_request),
	// 	.i_rw(bus_rw),
	// 	.i_address(sdram_address),
	// 	.i_wdata(bus_wdata),
	// 	.o_rdata(sdram_rdata),
	// 	.o_ready(sdram_ready),
	// 	.i_oddeven(bus_pa_busy),	// Instruction or data request

	// 	.o_sdram_request(w_sdram_request),
	// 	.o_sdram_rw(w_sdram_rw),
	// 	.o_sdram_address(w_sdram_address),
	// 	.o_sdram_wdata(w_sdram_wdata),
	// 	.i_sdram_rdata(w_sdram_rdata),
	// 	.i_sdram_ready(w_sdram_ready)
	// );
	
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
		.o_pb_busy(),

		// Port C (DMA)
		.i_pc_rw(dma_bus_rw),
		.i_pc_request(dma_bus_request),
		.o_pc_ready(dma_bus_ready),
		.i_pc_address(dma_bus_address),
		.o_pc_rdata(dma_bus_rdata),
		.i_pc_wdata(dma_bus_wdata),
		.o_pc_busy()
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
	wire cpu_pipeline_disable;
	wire cpu_fault;

	CPU #(
		.FREQUENCY(`FREQUENCY),
		.STACK_POINTER(32'h20110000),
		.DCACHE_REGISTERED(0),
		.ICACHE_REGISTERED(0)
	) cpu(
        .i_reset(reset),
		.i_clock(clock),

		// Control
		.i_timer_interrupt(timer_interrupt),
		.i_external_interrupt(plic_interrupt),
		.o_external_interrupt_enable(plic_interrupt_enable),

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
		.i_pipeline_disable(cpu_pipeline_disable),
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
	Timer timer(
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

	AUDIO_pwm_output audio_pwm_output(
		.i_clock(clock),
		.o_busy(audio_output_busy),
		.i_sample(audio_output_sample),
		.o_pwm(AUDIO_PWM)
	);

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
	bit [1:0] vb = 2'b00;
	always_ff @(posedge clock)
		vb <= { vb[0], ~vga_vblank };

	wire plic_interrupt_enable;
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

		.i_interrupt_enable(plic_interrupt_enable),
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
		.RAM_SIZE(32'h2000000)
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
	assign HDMI_TX_DE = vga_hblank && vga_vblank;
	assign HDMI_TX_CLK = vga_clock;

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
	wire [9:0] vga_pos_x;
	wire [9:0] vga_pos_y;

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
		.o_pos_x(vga_pos_x),
		.o_pos_y(vga_pos_y)
	);

	assign HDMI_TX_HS = vga_hsync;
	assign HDMI_TX_VS = vga_vsync;
	
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

	BRAM_dual #(
		.WIDTH(32),
		.SIZE(2*320*200),
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
		.i_video_hblank(vga_hblank),
		.i_video_vblank(vga_vblank),
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

	bit debug_bus_fault = 0;
	bit [31:0] debug_bus_fault_address = 0;
	bit [1:0] debug_bus_fault_type = 0;

	bit debug_request = 0;
	bit [13:0] debug_select = 0;

	always_comb begin
		debug_bus_fault = !bus_valid_select && bus_request;
		debug_bus_fault_address = bus_address;
		debug_bus_fault_type =
			cpu_ibus_request ? 2'd1 :
			cpu_dbus_request ? 2'd2 :
			dma_bus_request ? 2'd3 :
			2'd0;

		debug_request = bus_request;
		debug_select =
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
