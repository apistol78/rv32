`include "ClockDivider.v"
`include "CPU.v"
`include "BRAM.v"
`include "BROM.v"
`include "GPIO.v"
`include "FIFO.v"
`include "I2C.v"
`include "Memory_16_to_32.v"
`include "SD.v"
`include "SRAM_tb.v"
`include "Video_tb.v"

`timescale 10ns / 1ns // time-unit = 1 ns, precision = 10 ps

module SoC_tb;

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
	wire ram_rw;
	wire [31:0] ram_address;
	wire [31:0] ram_wdata;
	wire [31:0] ram_rdata;
	wire ram_ready;
	BRAM ram(
		.i_clock(clock),
		.i_request(ram_select && cpu_request),
		.i_rw(ram_rw),
		.i_address(ram_address),
		.i_wdata(ram_wdata),
		.o_rdata(ram_rdata),
		.o_ready(ram_ready)
	);

	// SRAM
	wire sram16_clock;
	wire sram16_request;
	wire sram16_rw;
	wire [17:0] sram16_address;
	wire [15:0] sram16_wdata;
	wire [15:0] sram16_rdata;
	wire sram16_ready;
	SRAM_tb sram16(
		.i_clock(sram16_clock),
		.i_request(sram16_request),
		.i_rw(sram16_rw),
		.i_address(sram16_address),
		.i_wdata(sram16_wdata),
		.o_rdata(sram16_rdata),
		.o_ready(sram16_ready)
	);

	wire sram32_enable;
	wire sram32_rw;
	wire [31:0] sram32_address;
	wire [31:0] sram32_wdata;
	wire [31:0] sram32_rdata;
	wire sram32_ready;
	Memory_16_to_32 sram32(
		.i_clock(clock),
		.i_request(sram32_select && cpu_request),
		.i_rw(sram32_rw),
		.i_address(sram32_address),
		.i_wdata(sram32_wdata),
		.o_rdata(sram32_rdata),
		.o_ready(sram32_ready),

		.o_ram_clock(sram16_clock),
		.o_ram_request(sram16_request),
		.o_ram_rw(sram16_rw),
		.o_ram_address(sram16_address),
		.o_ram_wdata(sram16_wdata),
		.i_ram_rdata(sram16_rdata),
		.i_ram_ready(sram16_ready)
	);

	// Video
	wire video_select;
	wire video_rw;
	wire [31:0] video_address;
	wire [31:0] video_wdata;
	wire [31:0] video_rdata;
	Video_tb video(
		.i_enable(video_select && cpu_request),
		.i_rw(video_rw),
		.i_address(video_address),
		.i_wdata(video_wdata),
		.o_rdata(video_rdata)
	);

	// GPIO
	wire gpio_select;
	wire gpio_rw;
	wire [31:0] gpio_wdata;
	wire [31:0] gpio_rdata;
	wire [35:0] GPIO;
	GPIO gpio(
		.i_enable(gpio_select && cpu_request),
		.i_rw(gpio_rw),
		.i_wdata(gpio_wdata),
		.o_rdata(gpio_rdata),
		// ---
		.GPIO(GPIO)
	);

	// I2C
	wire i2c_select;
	wire i2c_rw;
	wire [31:0] i2c_wdata;
	wire [31:0] i2c_rdata;
	wire I2C_SCL;
	wire I2C_SDA;
	I2C i2c(
		.i_enable(i2c_select && cpu_request),
		.i_rw(i2c_rw),
		.i_wdata(i2c_wdata),
		.o_rdata(i2c_rdata),
		// ---
		.I2C_SCL(I2C_SCL),
		.I2C_SDA(I2C_SDA)
	);

	// SD
	wire sd_select;
	wire sd_rw;
	wire [31:0] sd_wdata;
	wire [31:0] sd_rdata;
	wire SD_CLK;
	wire SD_CMD;
	wire [3:0] SD_DAT;
	SD sd(
		.i_enable(sd_select && cpu_request),
		.i_rw(sd_rw),
		.i_wdata(sd_wdata),
		.o_rdata(sd_rdata),
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
	CPU cpu(
        .i_reset(reset),
		.i_clock(clock),
		.o_rw(cpu_rw),
		.o_request(cpu_request),
		.i_ready(cpu_ready),
		.o_address(cpu_address),
		.i_data(cpu_rdata),
		.o_data(cpu_wdata)
	);


	//=====================================

	assign rom_select = (cpu_address >= 32'h00000000 && cpu_address < 32'h00010000);
	assign rom_address = cpu_address - 32'h00000000;

	assign ram_select = (cpu_address >= 32'h00010000 && cpu_address < 32'h00020000);
	assign ram_rw = cpu_rw;
	assign ram_address = cpu_address - 32'h00010000;
	assign ram_wdata = cpu_wdata;

	assign sram32_select = (cpu_address >= 32'h10000000 && cpu_address < 32'h20000000);
	assign sram32_rw = cpu_rw;
	assign sram32_address = cpu_address - 32'h10000000;
	assign sram32_wdata = cpu_wdata;

	assign led_select = (cpu_address >= 32'h50000000 && cpu_address < 32'h50000010);

	assign uart_select = (cpu_address >= 32'h50000010 && cpu_address < 32'h50000020);

	assign video_select = (cpu_address >= 32'h40000000 && cpu_address < 32'h50000000);
	assign video_rw = cpu_rw;
	assign video_address = cpu_address - 32'h40000000;
	assign video_wdata = cpu_wdata;

	assign gpio_select = (cpu_address >= 32'h50000020 && cpu_address < 32'h50000030);
	assign gpio_rw = cpu_rw;
	assign gpio_wdata = cpu_wdata;
	
	assign i2c_select = (cpu_address >= 32'h50000030 && cpu_address < 32'h50000040);
	assign i2c_rw = cpu_rw;
	assign i2c_wdata = cpu_wdata;

	assign sd_select = (cpu_address >= 32'h50000040 && cpu_address < 32'h50000050);
	assign sd_rw = cpu_rw;
	assign sd_wdata = cpu_wdata;

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
		i2c_select ? 1'b1 :
		sd_select ? 1'b1 :
		1'b0;

	// Generate clock.
	initial begin
		forever #1 begin
			clock <= !clock;
		end
	end

	// Simulate.
	initial begin
		$dumpfile("SoC_tb.vcd");
		$dumpvars(0, SoC_tb);

        #2
        reset <= 0;

		repeat(100000) @(posedge clock);

		$finish;
	end
	
endmodule