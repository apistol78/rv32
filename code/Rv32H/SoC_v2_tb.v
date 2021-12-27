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
	assign ram_rw = cpu_rw;
	assign ram_address = cpu_address - 32'h00010000;
	assign ram_wdata = cpu_wdata;

	assign cpu_rdata =
		rom_select ? rom_rdata :
		ram_select ? ram_rdata :
		32'h00000000;

	assign cpu_ready =
		rom_select ? rom_ready :
		ram_select ? ram_ready :
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

		repeat(50) @(posedge clock);

		$finish;
	end
	
endmodule