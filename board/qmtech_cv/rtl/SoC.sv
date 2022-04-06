
`timescale 1ns/1ns

`define FREQUENCY 100000000

module SoC(
	input sys_clk,		// 50 MHz oscillator
	input sys_reset_n,
	
	input key_1,
	
	output led_1,
	
	input uart_rx,
	output uart_tx,
	
	output lcd_bkl,
	output lcd_clk,
	output lcd_de,
	output [7:0] lcd_r,
	output [7:0] lcd_g,
	output [7:0] lcd_b,
	
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
	wire clock_video;		// 26.6MHz
	IP_PLL_Clk pll_clk(
		.refclk(sys_clk),
		.rst(!sys_reset_n),
		.outclk_0(clock),
		.outclk_1(clock_sdram),
		.outclk_2(clock_video),
		.locked()
	);
	
	reg [31:0] cont = 0;
	always@(posedge clock)
		cont <= (cont == 32'd4_000_001 ) ? 32'd0 : cont + 1'b1;

	reg[4:0] sample = 0;
	always @(posedge clock)
	begin
		if (cont == 32'd4_000_000)
			sample[4:0] = { sample[3:0], key_1 };
		else 
			sample[4:0] = sample[4:0];
	end

	wire start_n = (sample[4:3] == 2'b01) ? 1'b0 : 1'b1;
	wire reset = !start_n;
	
	assign led_1 = !led_led[0];

	reg [31:0] counter = 0;
	always_ff @(posedge clock)
	   counter <= counter + 1;
	   
	//=====================================

	// VIDEO

	assign lcd_bkl = counter[15];		// Backlight PWM controlled
	assign lcd_clk = vga_clock;
	assign lcd_de = vga_enable;
	assign lcd_r = vmode_video_rdata[23:16];
	assign lcd_g = vmode_video_rdata[15:8];
	assign lcd_b = vmode_video_rdata[7:0];

	// Video signal generator.
	wire vga_enable;
	wire [10:0] vga_pos_x;
	wire [10:0] vga_pos_y;
	wire vga_clock;
	VIDEO_LCD_AT070NTN92 #(
		.SYSTEM_FREQUENCY(26_600_000),
		.VGA_FREQUENCY(26_600_000),
		.HLINE(862),
		.HBACK(46),
		.HFRONT(16),
		.VLINE(510),
		.VBACK(23),
		.VFRONT(7)
	) vga(
		.i_reset(reset),
		.i_clock(clock_video),
		.i_clock_out(clock),
		.o_data_enable(vga_enable),
		.o_pos_x(vga_pos_x),
		.o_pos_y(vga_pos_y),
		.o_vga_clock(vga_clock)
	);
	
	// Scale resolution to 640*400.
	wire [10:0] vga_pos_x_scale = (vga_pos_x >> 2) + (vga_pos_x >> 1) + (vga_pos_x >> 4); // (vga_pos_x * 640) / 800;
	wire [10:0] vga_pos_y_scale = (vga_pos_y >> 2) + (vga_pos_y >> 1) + (vga_pos_y >> 4); // (vga_pos_y * 400) / 480;
	
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
		.SIZE((320*200)/4),
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

	VMODE_chunky #(
		.PPITCH(320)
	) vmode_chunky(
		.i_clock(clock),
		
		// CPU interface.
		.i_cpu_request(vram_select && bus_request),
		.i_cpu_rw(bus_rw),
		.i_cpu_address(vram_address),
		.i_cpu_wdata(bus_wdata),
		.o_cpu_rdata(vram_rdata),
		.o_cpu_ready(vram_ready),
		
		// Video signal interface.
		.i_video_request(vga_enable),
		.i_video_pos_x(vga_pos_x_scale[9:1]),
		.i_video_pos_y(vga_pos_y_scale[9:1]),
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

	SDRAM_interface_2 sdram(
		.i_reset(reset),
		.i_clock(clock),
		.i_clock_sdram(clock_sdram),
		// ---
		.i_request(sdram_select && bus_request),
		.i_rw(bus_rw),
		.i_address(sdram_address),
		.i_wdata(bus_wdata),
		.o_rdata(sdram_rdata),
		.o_ready(sdram_ready),
		// ---
		.sdram_clk(sdram_clk),
		.sdram_clk_en(sdram_clk_en),
		.sdram_cas_n(sdram_cas_n),
		.sdram_ce_n(sdram_ce_n),
		.sdram_ras_n(sdram_ras_n),
		.sdram_we_n(sdram_we_n),
		.sdram_dqml(sdram_dqml),
		.sdram_dqmh(sdram_dqmh),
		.sdram_ba(sdram_ba),
		.sdram_addr(sdram_addr),
		.sdram_data(sdram_data)
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
		.i_stall(1'b0),
		
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
		.FREQUENCY(`FREQUENCY),
		.DEVICEID(3)
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
		.ICACHE_SIZE(4),
		.DCACHE_SIZE(4)
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
	
	assign vram_select = bus_address[31:28] == 4'h3;
	assign vram_address = { 4'h0, bus_address[27:0] };

	assign led_select = bus_address[31:28] == 4'h4;

	assign uart_0_select = bus_address[31:24] == 8'h50;
	assign uart_0_address = bus_address[3:2];

	assign dma_select = bus_address[31:28] == 4'h9;
	assign dma_address = bus_address[3:2];

	assign timer_select = bus_address[31:28] == 4'ha;
	assign timer_address = bus_address[4:2];
	
	assign plic_select = bus_address[31:28] == 4'hb;
	assign plic_address = bus_address[23:0];

	//=====================================

	assign bus_rdata =
		rom_select ? rom_rdata :
		ram_select ? ram_rdata :
		sdram_select ? sdram_rdata :
		vram_select ? vram_rdata :
		uart_0_select ? uart_0_rdata :
		dma_select ? dma_rdata :
		timer_select ? timer_rdata :
		plic_select ? plic_rdata :
		32'h00000000;
		
	assign bus_ready =
		rom_select ? rom_ready :
		ram_select ? ram_ready :
		sdram_select ? sdram_ready :
		vram_select ? vram_ready :
		led_select ? led_ready :
		uart_0_select ? uart_0_ready :
		dma_select ? dma_ready :
		timer_select ? timer_ready :
		plic_select ? plic_ready :
		1'b0;
	
endmodule
