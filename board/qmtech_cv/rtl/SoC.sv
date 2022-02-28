
`timescale 1ns/1ns

`define FREQUENCY 100000000

module SoC(
	input sys_clk,
	input sys_reset_n,
	
	input key_1,
	
	output led_1,
	
	input uart_rx,
	output uart_tx,
	
	output sdram_clk,
	output sdram_clk_en,
	output sdram_cas_n,
	output sdram_ce_n,
	output sdram_ras_n,
	output sdram_we_n,
	output sdram_dqml,
	output sdram_dqmh,
	output [1:0] sdram_ba,
	output [11:0] sdram_addr,
	inout [15:0] sdram_data
);

	assign io_a_8 = 1'b0; // sys_clk;

	wire clock;
	IP_PLL_Clk pll_clk(
		.refclk(sys_clk),
		.rst(!sys_reset_n),
		.outclk_0(clock),
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

	wire soft_reset_n = (sample[1:0] == 2'b10) ? 1'b0 : 1'b1;
	wire global_reset_n = (sample[3:2] == 2'b10) ? 1'b0 : 1'b1;
	wire start_n = (sample[4:3] == 2'b01) ? 1'b0 : 1'b1;
	wire reset = !start_n;

	assign led_1 = led_led[0];

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

	// RAM
	wire ram_select;
	wire [31:0] ram_address;
	wire [31:0] ram_rdata;
	wire ram_ready;
	BRAM #(
		.WIDTH(32),
		.SIZE(32'h8000),
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
	
	// UART (USB)
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
		.i_stall(vram_fifo_full),
		
		// Bus
		.o_bus_rw(dma_bus_rw),
		.o_bus_request(dma_bus_request),
		.i_bus_ready(dma_bus_ready),
		.o_bus_address(dma_bus_address),
		.i_bus_rdata(dma_bus_rdata),
		.o_bus_wdata(dma_bus_wdata)
	);

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
		.ICACHE_SIZE(8),
		.DCACHE_SIZE(10)
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
		uart_0_select ? uart_0_rdata :
		dma_select ? dma_rdata :
		timer_select ? timer_rdata :
		plic_select ? plic_rdata :
		32'h00000000;
		
	assign bus_ready =
		rom_select ? rom_ready :
		ram_select ? ram_ready :
		led_select ? led_ready :
		uart_0_select ? uart_0_ready :
		dma_select ? dma_ready :
		timer_select ? timer_ready :
		plic_select ? plic_ready :
		1'b0;
	
endmodule
