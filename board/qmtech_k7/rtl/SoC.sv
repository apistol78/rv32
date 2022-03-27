`timescale 1ns / 1ps

`define FREQUENCY 100000000

module SoC(
    input sys_clk,
    input sys_rst_n,
    output led_1,
    output led_2,
    output led_3
);

    assign clock = sys_clk;

	reg [31:0] cont = 0;
	always@(posedge clock)
		cont <= (cont == 32'd4_000_001 ) ? 32'd0 : cont + 1'b1;

	reg[4:0] sample = 0;
	always @(posedge clock)
	begin
		if (cont == 32'd4_000_000)
			sample[4:0] = { sample[3:0], ~sys_rst_n };
		else 
			sample[4:0] = sample[4:0];
	end

	wire start_n = (sample[4:3] == 2'b01) ? 1'b0 : 1'b1;
	wire reset = !start_n;

    assign led_1 = ~led_led[0];
    assign led_2 = ~led_led[1];
    assign led_3 = ~led_led[2];

	//=====================================

	// VIDEO

	assign lcd_bkl = cont[11];		// Backlight PWM controlled
	assign lcd_clk = vga_clock;
	assign lcd_de = vga_enable;
	assign lcd_r = vbus_rdata[23:16];
	assign lcd_g = vbus_rdata[15:8];
	assign lcd_b = vbus_rdata[7:0];
	assign lcd_vsync = 1'b0;
	assign lcd_hsync = 1'b0;

	// Video memory.
	wire video_sram_request;
	wire video_sram_rw;
	wire [31:0] video_sram_address;
	wire [31:0] video_sram_wdata;
	wire [31:0] video_sram_rdata;
	wire video_sram_ready;
	BRAM #(
		.WIDTH(32),
		.SIZE(400*240/4),
		.ADDR_LSH(2)
	) video_sram(
		.i_clock(clock),
		.i_request(video_sram_request),
		.i_rw(video_sram_rw),
		.i_address(video_sram_address),
		.i_wdata(video_sram_wdata),
		.o_rdata(video_sram_rdata),
		.o_ready(video_sram_ready)
	);

	// Video mode; chunky 8-bit palette.
	wire vbus_select;
	wire [31:0] vbus_address;
	wire vbus_ready;
	wire vbus_fifo_full;
	wire [31:0] vbus_rdata;
	VMODE_chunky #(
		.PPITCH(400),
		.FIFO_DEPTH(8)
	) vmode_chunky(
		.i_clock(clock),
		
		.i_cpu_request(vbus_select && bus_request),
		.i_cpu_address(vbus_address),
		.i_cpu_wdata(bus_wdata),
		.o_cpu_ready(vbus_ready),
		
		.i_video_request(vga_enable),
		.i_video_pos_x(vga_pos_x[9:1]),	// 0 - 399
		.i_video_pos_y(vga_pos_y[9:1]),	// 0 - 239
		.o_video_rdata(vbus_rdata),
		
		.o_mem_request(video_sram_request),
		.o_mem_rw(video_sram_rw),
		.o_mem_address(video_sram_address),
		.o_mem_wdata(video_sram_wdata),
		.i_mem_rdata(video_sram_rdata),
		.i_mem_ready(video_sram_ready),
		
		.o_fifo_full(vbus_fifo_full)
	);

	// 800*480
	wire vga_enable;
	wire [10:0] vga_pos_x;
	wire [10:0] vga_pos_y;
	wire vga_clock;
	VIDEO_LCD_AT070NTN92 #(
		.SYSTEM_FREQUENCY(`FREQUENCY),
		.VGA_FREQUENCY(33_300_000),
		.HLINE(1056),
		.HBACK(46),
		.HFRONT(210),
		.HPULSE(1),
		.VLINE(525),
		.VBACK(23),
		.VFRONT(22),
		.VPULSE(1)
	) vga(
		.i_clock(clock),
		.o_data_enable(vga_enable),
		.o_pos_x(vga_pos_x),
		.o_pos_y(vga_pos_y),
		.o_vga_clock(vga_clock)
	);
	
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
	
	// LEDS
	wire led_select;
	wire led_ready;
	wire [9:0] led_led;
	LED led(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(led_select && bus_request),
		.i_wdata(bus_wdata),
		.o_ready(led_ready),
		.LEDR(led_led)
	);
	
	//=====================================
	
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
		.i_request(uart_0_select && bus_request),
		.i_rw(bus_rw),
		.i_address(uart_0_address),
		.i_wdata(bus_wdata),
		.o_rdata(uart_0_rdata),
		.o_ready(uart_0_ready),
		// ---
		.UART_RX(uart_rx),
		.UART_TX(uart_tx)
	);

	//=====================================

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
		.i_request(dma_select && bus_request),
		.i_rw(bus_rw),
		.i_address(dma_address),
		.i_wdata(bus_wdata),
		.o_rdata(dma_rdata),
		.o_ready(dma_ready),

		// System
		.i_stall(vbus_fifo_full),
		
		// Bus
		.o_bus_rw(dma_bus_rw),
		.o_bus_request(dma_bus_request),
		.i_bus_ready(dma_bus_ready),
		.o_bus_address(dma_bus_address),
		.i_bus_rdata(dma_bus_rdata),
		.o_bus_wdata(dma_bus_wdata)
	);
	
	//=====================================

	// Timer
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
		.i_request(timer_select && bus_request),
		.i_rw(bus_rw),
		.i_address(timer_address),
		.i_wdata(bus_wdata),
		.o_rdata(timer_rdata),
		.o_ready(timer_ready),
		.o_interrupt(timer_interrupt)
	);
	
	//=====================================

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

		.i_request(plic_select && bus_request),
		.i_rw(bus_rw),
		.i_address(plic_address),
		.i_wdata(bus_wdata),
		.o_rdata(plic_rdata),
		.o_ready(plic_ready)
	);
	
	//====================================================

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

	wire bus_pb_rw;
	wire bus_pb_request;
	wire bus_pb_ready;
	wire [31:0] bus_pb_address;
	wire [31:0] bus_pb_rdata;
	wire [31:0] bus_pb_wdata;

	BusAccess bus(
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
		.ICACHE_SIZE(10),
		.DCACHE_SIZE(8)
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
	
endmodule
