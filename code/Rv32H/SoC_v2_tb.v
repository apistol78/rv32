`include "CPU_v2.v"
`include "CPU_BusAccess.v"
`include "CPU_Decode.v"
`include "CPU_Execute.v"
`include "CPU_Fetch.v"
`include "CPU_Memory.v"
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
`include "Video_tb.v"

`timescale 10ns / 1ns // time-unit = 1 ns, precision = 10 ps

module SoC_v2_tb;

    reg reset = 1'b1;
	reg clock = 1'b0;

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

	wire sram32_select;
	wire [31:0] sram32_address;
	wire [31:0] sram32_rdata;
	wire sram32_ready;
	SRAM_interface sram(
		.i_clock(clock),
		.i_request(sram32_select && cpu_request),
		.i_rw(cpu_rw),
		.i_address(sram32_address),
		.i_wdata(cpu_wdata),
		.o_rdata(sram32_rdata),
		.o_ready(sram32_ready),
		// ---
		.SRAM_A(SRAM_A),
		.SRAM_D(SRAM_D),
		.SRAM_CE_n(SRAM_CE_n),
		.SRAM_OE_n(SRAM_OE_n),
		.SRAM_WE_n(SRAM_WE_n),
		.SRAM_LB_n(SRAM_LB_n),
		.SRAM_UB_n(SRAM_UB_n)
	);

	// Video
	wire video_select;
	wire [31:0] video_address;
	wire [31:0] video_rdata;
	Video_tb video(
		.i_clock(clock),
		.i_request(video_select && cpu_request),
		.i_rw(cpu_rw),
		.i_address(video_address),
		.i_wdata(cpu_wdata),
		.o_rdata(video_rdata)
	);

	// GPIO
	wire gpio_select;
	wire [31:0] gpio_rdata;
	wire [35:0] GPIO;
	GPIO gpio(
		.i_clock(clock),
		.i_request(gpio_select && cpu_request),
		.i_rw(cpu_rw),
		.i_wdata(cpu_wdata),
		.o_rdata(gpio_rdata),
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

	assign rom_select = (cpu_address >= 32'h00000000 && cpu_address < 32'h00010000);
	assign rom_address = cpu_address - 32'h00000000;

	assign ram_select = (cpu_address >= 32'h00010000 && cpu_address < 32'h00020000);
	assign ram_address = cpu_address - 32'h00010000;

	assign sram32_select = (cpu_address >= 32'h10000000 && cpu_address < 32'h20000000);
	assign sram32_address = cpu_address - 32'h10000000;

	assign led_select = (cpu_address >= 32'h50000000 && cpu_address < 32'h50000010);

	assign uart_select = (cpu_address >= 32'h50000010 && cpu_address < 32'h50000020);

	assign video_select = (cpu_address >= 32'h40000000 && cpu_address < 32'h50000000);
	assign video_address = cpu_address - 32'h40000000;

	assign gpio_select = (cpu_address >= 32'h50000020 && cpu_address < 32'h50000030);
	
	assign i2c_select = (cpu_address >= 32'h50000030 && cpu_address < 32'h50000040);

	assign sd_select = (cpu_address >= 32'h50000040 && cpu_address < 32'h50000050);

	assign cpu_rdata =
		rom_select ? rom_rdata :
		ram_select ? ram_rdata :
		sram32_select ? sram32_rdata :
		video_select ? video_rdata :
		gpio_select ? gpio_rdata :
		i2c_select ? i2c_rdata :
		sd_select ? sd_rdata :
		32'h00000000;

	assign cpu_ready =
		rom_select ? rom_ready :
		ram_select ? ram_ready :
		sram32_select ? sram32_ready :
		led_select ? 1'b1 :
		uart_select ? 1'b1 :
		video_select ? 1'b1 :
		gpio_select ? 1'b1 :
		i2c_select ? i2c_ready :
		sd_select ? sd_ready :
		1'b0;

	// Generate clock.
	initial begin
		forever #1 begin
			clock <= !clock;
		end
	end

	// Simulate.
	initial begin
		$dumpfile("SoC_v2_tb.vcd");
		$dumpvars(0, SoC_v2_tb);
/*
		reset <= 1;

        #2
        reset <= 0;

		repeat(50) @(posedge clock);
		#1
		reset <= 1;

        #2
*/
        reset <= 0;

		repeat(1000) @(posedge clock);

		$finish;
	end
	
endmodule