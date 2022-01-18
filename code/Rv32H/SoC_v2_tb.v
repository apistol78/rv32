`include "CPU_v2.v"
`include "CPU_ALU.v"
`include "CPU_BusAccess.v"
`include "CPU_DCache.v"
`include "CPU_Decode.v"
`include "CPU_Divide.v"
`include "CPU_Execute.v"
`include "CPU_Fetch.v"
`include "CPU_ICache.v"
`include "CPU_Memory.v"
`include "CPU_Multiply.v"
`include "CPU_Prefetch.v"
`include "CPU_Registers.v"
`include "CPU_Writeback.v"

`include "BRAM.v"
`include "BRAM_clear.v"
`include "BROM.v"
`include "ClockDivider.v"
`include "GPIO.v"
`include "FIFO.v"
`include "I2C.v"
`include "Registers.v"
`include "SD.v"
`include "SRAM_interface.v"
`include "SRAM_tb.v"
`include "Timer.v"
`include "UART_tb.v"
`include "VGA.v"

`timescale 1ns/1ns

module SoC_v2_tb(
`ifdef __VERILATOR__
	input wire i_reset,
	input wire i_clock
`endif
);

`ifdef __VERILATOR__
	wire reset = i_reset;
	wire clock = i_clock;
`else
    reg reset = 1'b1;
	reg clock = 1'b0;
`endif

	// VGA signal generator
	wire vga_hsync;
	wire vga_vsync;
	wire vga_data_enable;
	wire [15:0] vga_address;
	wire vga_clock;
	VGA #(
		.PRESCALE(100000000 / 25000000)
	) vga(
		.i_clock(clock),
		.o_hsync(vga_hsync),
		.o_vsync(vga_vsync),
		.o_data_enable(vga_data_enable),
		.o_vga_address(vga_address),
		.o_vga_clock(vga_clock)
	);

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
	BRAM ram(
		.i_clock(clock),
		.i_request(ram_select && bus_request),
		.i_rw(bus_rw),
		.i_address(ram_address),
		.i_wdata(bus_wdata),
		.o_rdata(ram_rdata),
		.o_ready(ram_ready)
	);

	// SDRAM
	wire sdram_select;
	wire [31:0] sdram_address;
	wire [31:0] sdram_rdata;
	wire sdram_ready;
	BRAM #(
		.WIDTH(32),
		.SIZE(32'h100000),
		.ADDR_LSH(2)
	) sdram(
		.i_clock(clock),
		.i_request(sdram_select && bus_request),
		.i_rw(bus_rw),
		.i_address(sdram_address),
		.i_wdata(bus_wdata),
		.o_rdata(sdram_rdata),
		.o_ready(sdram_ready)
	);

	// UART
	wire uart_select;
	wire [1:0] uart_address;
	wire [31:0] uart_rdata;
	wire uart_ready;
	UART_tb uart(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(uart_select && bus_request),
		.i_rw(bus_rw),
		.i_address(uart_address),
		.i_wdata(bus_wdata),
		.o_rdata(uart_rdata),
		.o_ready(uart_ready)
	);

	// GPIO
	wire gpio_select;
	wire [1:0] gpio_address;
	wire [31:0] gpio_rdata;
	wire gpio_ready;
	wire [35:0] GPIO;
	GPIO gpio(
		.i_clock(clock),
		.i_request(gpio_select && bus_request),
		.i_rw(bus_rw),
		.i_address(gpio_address),
		.i_wdata(bus_wdata),
		.o_rdata(gpio_rdata),
		.o_ready(gpio_ready),
		// ---
		.GPIO(GPIO)
	);

	// I2C
	wire i2c_select;
	wire [31:0] i2c_rdata;
	wire i2c_ready;
	wire I2C_SCL;
	wire I2C_SDA;
	I2C i2c(
		.i_clock(clock),
		.i_request(i2c_select && bus_request),
		.i_rw(bus_rw),
		.i_wdata(bus_wdata),
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
	wire SD_CLK;
	wire SD_CMD;
	wire [3:0] SD_DAT;
	SD sd(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(sd_select && bus_request),
		.i_rw(bus_rw),
		.i_wdata(bus_wdata),
		.o_rdata(sd_rdata),
		.o_ready(sd_ready),
		// ---
		.SD_CLK(SD_CLK),
		.SD_CMD(SD_CMD),
		.SD_DAT(SD_DAT)
	);

	// Timer
	wire timer_select;
	wire [1:0] timer_address;
	wire [31:0] timer_rdata;
	wire timer_ready;
	Timer #(
		.FREQUENCY(100000000)
	) timer(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(timer_select && bus_request),
		.i_address(timer_address),
		.o_rdata(timer_rdata),
		.o_ready(timer_ready),
		
		// Debug
		.i_retire_count(cpu_retire_count)
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

	CPU_BusAccess bus(
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
		.i_pb_wdata(cpu_dbus_wdata)
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
	wire [31:0] cpu_retire_count;
	CPU_v2 cpu(
        .i_reset(reset),
		.i_clock(clock),

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
		.o_retire_count(cpu_retire_count)
	);

	//=====================================

	assign rom_select = (bus_address < 32'h00010000);
	assign rom_address = bus_address - 32'h00000000;

	assign ram_select = (bus_address >= 32'h00010000 && bus_address < 32'h00020000);
	assign ram_address = bus_address - 32'h00010000;

	assign sdram_select = (bus_address >= 32'h20000000 && bus_address < 32'h40000000);
	assign sdram_address = bus_address - 32'h20000000;

	// assign vram_select = (bus_address >= 32'h40000000 && bus_address < 32'h50000000);
	// assign vram_address = bus_address - 32'h40000000;

	wire led_select = (bus_address >= 32'h50000000 && bus_address < 32'h50000010);
	
	assign uart_select = (bus_address >= 32'h50000010 && bus_address < 32'h50000020);
	assign uart_address = bus_address[3:2]; // - 32'h50000010;

	assign gpio_select = (bus_address >= 32'h50000020 && bus_address < 32'h50000030);
	assign gpio_address = bus_address[3:2]; // - 32'h50000020;

	assign i2c_select = (bus_address >= 32'h50000030 && bus_address < 32'h50000040);

	assign sd_select = (bus_address >= 32'h50000040 && bus_address < 32'h50000050);

	assign timer_select = (bus_address >= 32'h50000050 && bus_address < 32'h50000060);
	assign timer_address = bus_address[3:2]; // - 32'h50000050;

	assign bus_rdata =
		rom_select ? rom_rdata :
		ram_select ? ram_rdata :
		sdram_select ? sdram_rdata :
		uart_select ? uart_rdata :
		gpio_select ? gpio_rdata :
		sd_select ? sd_rdata :
		i2c_select ? i2c_rdata :
		timer_select ? timer_rdata :
		32'h00000000;

	assign bus_ready =
		rom_select ? rom_ready :
		ram_select ? ram_ready :
		sdram_select ? sdram_ready :
		led_select ? 1'b1 :
		uart_select ? uart_ready :
		gpio_select ? gpio_ready :
		sd_select ? sd_ready :
		i2c_select ? i2c_ready :
		timer_select ? timer_ready :
		1'b0;

`ifdef __ICARUS__
	initial begin
		forever #5 begin
			clock <= !clock;
		end
	end

	initial begin
		$dumpfile("SoC_v2_tb.vcd");
		$dumpvars(0, SoC_v2_tb);

        reset <= 0;

		repeat(100000) @(posedge clock);

		$finish;
	end
`endif
	
endmodule