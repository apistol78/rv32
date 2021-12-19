`include "Bus.v"
`include "CPU.v"
`include "BRAM.v"
`include "BROM.v"
`include "Memory_16_to_32.v"
`include "SRAM_tb.v"
`include "Video_tb.v"

`timescale 10ns / 1ns // time-unit = 1 ns, precision = 10 ps

module SoC_tb;

    reg reset = 1'b1;
	reg clock = 1'b0;

    // ROM
	wire rom_enable;
	wire [31:0] rom_address;
	wire [31:0] rom_rdata;
	BROM rom(
		.i_enable(rom_enable),
		.i_address(rom_address),
		.o_rdata(rom_rdata)
	);

	// RAM
	wire ram_enable;
	wire ram_rw;
	wire [31:0] ram_address;
	wire [31:0] ram_wdata;
	wire [31:0] ram_rdata;
	BRAM ram(
		.i_enable(ram_enable),
		.i_rw(ram_rw),
		.i_address(ram_address),
		.i_wdata(ram_wdata),
		.o_rdata(ram_rdata)
	);

	// SRAM
	wire sram16_clock;
	wire sram16_enable;
	wire sram16_rw;
	wire [17:0] sram16_address;
	wire [15:0] sram16_wdata;
	wire [15:0] sram16_rdata;
	wire sram16_ready;
	SRAM_tb sram16(
		.i_clock(sram16_clock),
		.i_enable(sram16_enable),
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
		.i_enable(sram32_enable),
		.i_rw(sram32_rw),
		.i_address(sram32_address),
		.i_wdata(sram32_wdata),
		.o_rdata(sram32_rdata),
		.o_ready(sram32_ready),

		.o_ram_clock(sram16_clock),
		.o_ram_enable(sram16_enable),
		.o_ram_rw(sram16_rw),
		.o_ram_address(sram16_address),
		.o_ram_wdata(sram16_wdata),
		.i_ram_rdata(sram16_rdata),
		.i_ram_ready(sram16_ready)
	);

	// Video
	wire video_enable;
	wire video_rw;
	wire [31:0] video_address;
	wire [31:0] video_wdata;
	wire [31:0] video_rdata;
	Video_tb video(
		.i_enable(video_enable),
		.i_rw(video_rw),
		.i_address(video_address),
		.i_wdata(video_wdata),
		.o_rdata(video_rdata)
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

	// Bus
	wire bus_rw;
	wire bus_request;
	wire bus_ready;
	wire [31:0] bus_address;
	wire [31:0] bus_rdata;
	wire [31:0] bus_wdata;	
	Bus bus(
		.i_clock(clock),

		.i_cpu_rw(cpu_rw),
		.i_cpu_request(cpu_request),
		.o_cpu_ready(cpu_ready),
		.i_cpu_address(cpu_address),
		.o_cpu_rdata(cpu_rdata),
		.i_cpu_wdata(cpu_wdata),

		.o_bus_rw(bus_rw),
		.o_bus_request(bus_request),
		.i_bus_ready(bus_ready),
		.o_bus_address(bus_address),
		.i_bus_rdata(bus_rdata),
		.o_bus_wdata(bus_wdata)		
	);

	//=====================================

	assign rom_enable = bus_request && (bus_address >= 32'h00000000 && bus_address < 32'h00010000);
	assign rom_address = bus_address - 32'h00000000;

	assign ram_enable = bus_request && (bus_address >= 32'h00010000 && bus_address < 32'h00020000);
	assign ram_rw = bus_rw;
	assign ram_address = bus_address - 32'h00010000;
	assign ram_wdata = bus_wdata;

	assign sram32_enable = bus_request && (bus_address >= 32'h10000000 && bus_address < 32'h20000000);
	assign sram32_rw = bus_rw;
	assign sram32_address = bus_address - 32'h10000000;
	assign sram32_wdata = bus_wdata;

	assign video_enable = bus_request && (bus_address >= 32'h40000000 && bus_address < 32'h50000000);
	assign video_rw = bus_rw;
	assign video_address = bus_address - 32'h40000000;
	assign video_wdata = bus_wdata;

	assign bus_rdata =
		rom_enable ? rom_rdata :
		ram_enable ? ram_rdata :
		sram32_enable ? sram32_rdata :
		video_enable ? video_rdata :
		32'h00000000;

	assign bus_ready =
		rom_enable ? 1'b1 :
		ram_enable ? 1'b1 :
		sram32_enable ? sram32_ready :
		video_enable ? 1'b1 :
		1'b1;

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

		repeat(1000) @(posedge clock);

		$finish;
	end
	
endmodule