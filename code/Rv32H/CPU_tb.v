`include "CPU.v"
`include "BRAM.v"
`include "BROM.v"
`include "SRAM_tb.v"
`include "Video_tb.v"

`timescale 1 ns/10 ps  // time-unit = 1 ns, precision = 10 ps

module CPU_tab;

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
	wire sram_enable;
	wire sram_rw;
	wire [31:0] sram_address;
	wire [31:0] sram_wdata;
	wire [31:0] sram_rdata;
	wire sram_ready;
	SRAM_tb sram(
		.i_clock(clock),
		.i_enable(sram_enable),
		.i_rw(sram_rw),
		.i_address(sram_address),
		.i_wdata(sram_wdata),
		.o_rdata(sram_rdata),
		.o_ready(sram_ready)
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



	//=====================================

	assign rom_enable = cpu_request && (cpu_address >= 32'h0000_0200 && cpu_address < 32'h0002_0000);
	assign rom_address = cpu_address - 32'h0000_0000;

	assign ram_enable = cpu_request && (cpu_address >= 32'h0002_0000 && cpu_address < 32'h0002_0000 + 32'h0000_8000);
	assign ram_rw = cpu_rw;
	assign ram_address = cpu_address - 32'h0002_0000;
	assign ram_wdata = cpu_wdata;

	assign sram_enable = cpu_request && (cpu_address >= 32'h0003_0000 && cpu_address < 32'h0003_0000 + 32'h0000_8000);
	assign sram_rw = cpu_rw;
	assign sram_address = cpu_address - 32'h0003_0000;
	assign sram_wdata = cpu_wdata;

	assign video_enable = cpu_request && (cpu_address >= 32'h1000_0000 && cpu_address < 32'h2000_0000);
	assign video_rw = cpu_rw;
	assign video_address = cpu_address - 32'h1000_0000;
	assign video_wdata = cpu_wdata;

    assign cpu_rdata = rom_enable ? rom_rdata : 'z;
    assign cpu_rdata = ram_enable ? ram_rdata : 'z;
	assign cpu_rdata = sram_enable ? sram_rdata : 'z;
    assign cpu_rdata = video_enable ? video_rdata : 'z;

	assign cpu_ready = rom_enable ? 1'b1 : 'z;
	assign cpu_ready = ram_enable ? 1'b1 : 'z;
	assign cpu_ready = sram_enable ? sram_ready : 'z;
	assign cpu_ready = video_enable ? 1'b1 : 'z;

	// Generate clock.
	initial begin
		forever #1 begin
			clock <= !clock;
		end
	end


	// Simulate.
	initial begin
/*
		$dumpfile("CPU_tb.vcd");
		$dumpvars(0, clock);
		$dumpvars(0, cpu_request);
		$dumpvars(0, cpu_address);
        $dumpvars(0, cpu_rdata);
		
		$dumpvars(0, ram_enable);
        $dumpvars(0, ram_address);
		$dumpvars(0, ram_rw);
        $dumpvars(0, ram_rdata);
        $dumpvars(0, ram_wdata);
*/
    
        #2
        reset <= 0;
        

		repeat(200) @(posedge clock);

		$finish;
	end
			
	
endmodule