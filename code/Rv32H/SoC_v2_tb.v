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
`include "CPU_Registers.v"
`include "CPU_Writeback.v"

`include "BRAM.v"
`include "BROM.v"
`include "ClockDivider.v"
`include "GPIO.v"
`include "FIFO.v"
`include "I2C.v"
`include "Registers.v"
`include "SD.v"
`include "SRAM_interface.v"
`include "SRAM_tb.v"
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
		.i_request(rom_select && cpu_request),
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
		.i_request(ram_select && cpu_request),
		.i_rw(cpu_rw),
		.i_address(ram_address),
		.i_wdata(cpu_wdata),
		.o_rdata(ram_rdata),
		.o_ready(ram_ready)
	);

	// SRAM
	wire [17:0] SRAM_A;
	wire [15:0] SRAM_D;
	wire SRAM_CE_n;
	wire SRAM_OE_n;
	wire SRAM_WE_n;
	wire SRAM_LB_n;
	wire SRAM_UB_n;
	SRAM_tb sram_tb(
		.SRAM_A(SRAM_A),
		.SRAM_D(SRAM_D),
		.SRAM_CE_n(SRAM_CE_n),
		.SRAM_OE_n(SRAM_OE_n),
		.SRAM_WE_n(SRAM_WE_n),
		.SRAM_LB_n(SRAM_LB_n),
		.SRAM_UB_n(SRAM_UB_n)
	);

	wire sram_select;
	wire [31:0] sram_address;
	wire [31:0] sram_rdata;
	wire sram_ready;
	SRAM_interface sram(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(sram_select && cpu_request),
		.i_rw(cpu_rw),
		.i_address(sram_address),
		.i_wdata(cpu_wdata),
		.o_rdata(sram_rdata),
		.o_ready(sram_ready),
		// ---
		.SRAM_A(SRAM_A),
		.SRAM_D(SRAM_D),
		.SRAM_CE_n(SRAM_CE_n),
		.SRAM_OE_n(SRAM_OE_n),
		.SRAM_WE_n(SRAM_WE_n),
		.SRAM_LB_n(SRAM_LB_n),
		.SRAM_UB_n(SRAM_UB_n)
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
		.i_request(sdram_select && cpu_request),
		.i_rw(cpu_rw),
		.i_address(sdram_address),
		.i_wdata(cpu_wdata),
		.o_rdata(sdram_rdata),
		.o_ready(sdram_ready)
	);

	// UART
	wire uart_select;
	wire [31:0] uart_rdata;
	UART_tb uart(
		.i_clock(clock),
		.i_request(uart_select && cpu_request),
		.i_rw(cpu_rw),
		.i_wdata(cpu_wdata),
		.o_rdata(uart_rdata)
	);

	// GPIO
	wire gpio_select;
	wire [31:0] gpio_address;
	wire [31:0] gpio_rdata;
	wire gpio_ready;
	wire [35:0] GPIO;
	GPIO gpio(
		.i_clock(clock),
		.i_request(gpio_select && cpu_request),
		.i_rw(cpu_rw),
		.i_address(gpio_address),
		.i_wdata(cpu_wdata),
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
		.i_request(i2c_select && cpu_request),
		.i_rw(cpu_rw),
		.i_wdata(cpu_wdata),
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
		.i_request(sd_select && cpu_request),
		.i_rw(cpu_rw),
		.i_wdata(cpu_wdata),
		.o_rdata(sd_rdata),
		.o_ready(sd_ready),
		// ---
		.SD_CLK(SD_CLK),
		.SD_CMD(SD_CMD),
		.SD_DAT(SD_DAT)
	);

    // CPU
	wire cpu_rw;
	wire cpu_request;
	wire cpu_ready;
	wire [31:0] cpu_address;
	wire [31:0] cpu_rdata;
	wire [31:0] cpu_wdata;
	CPU_v2 cpu(
        .i_reset(reset),
		.i_clock(clock),

		// Bus
		.o_bus_rw(cpu_rw),
		.o_bus_request(cpu_request),
		.i_bus_ready(cpu_ready),
		.o_bus_address(cpu_address),
		.i_bus_rdata(cpu_rdata),
		.o_bus_wdata(cpu_wdata)
	);

	//=====================================

	assign rom_select = (cpu_address < 32'h00010000);
	assign rom_address = cpu_address - 32'h00000000;

	assign ram_select = (cpu_address >= 32'h00010000 && cpu_address < 32'h00020000);
	assign ram_address = cpu_address - 32'h00010000;

	assign sram_select = (cpu_address >= 32'h10000000 && cpu_address < 32'h20000000);
	assign sram_address = cpu_address - 32'h10000000;

	assign sdram_select = (cpu_address >= 32'h20000000 && cpu_address < 32'h40000000);
	assign sdram_address = cpu_address - 32'h20000000;

	wire led_select = (cpu_address >= 32'h50000000 && cpu_address < 32'h50000010);

	assign uart_select = (cpu_address >= 32'h50000010 && cpu_address < 32'h50000020);

	assign gpio_select = (cpu_address >= 32'h50000020 && cpu_address < 32'h50000030);
	assign gpio_address = cpu_address - 32'h50000020;
	
	assign i2c_select = (cpu_address >= 32'h50000030 && cpu_address < 32'h50000040);

	assign sd_select = (cpu_address >= 32'h50000040 && cpu_address < 32'h50000050);

	assign cpu_rdata =
		rom_select ? rom_rdata :
		ram_select ? ram_rdata :
		sram_select ? sram_rdata :
		sdram_select ? sdram_rdata :
		uart_select ? uart_rdata :
		gpio_select ? gpio_rdata :
		i2c_select ? i2c_rdata :
		sd_select ? sd_rdata :
		32'h00000000;

	assign cpu_ready =
		rom_select ? rom_ready :
		ram_select ? ram_ready :
		sram_select ? sram_ready :
		sdram_select ? sdram_ready :
		led_select ? 1'b1 :
		uart_select ? 1'b1 :
		gpio_select ? gpio_ready :
		i2c_select ? i2c_ready :
		sd_select ? sd_ready :
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