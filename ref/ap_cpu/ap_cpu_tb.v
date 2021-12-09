`include "ap_cpu.v"
`include "ap_cpu_mem_16.v"
`include "ap_soft_ram.v"
`include "ap_soft_rom.v"

module ap_cpu_tb;


	reg clk = 1'b0;


	// ROM

	wire rom_en;
	wire [31:0] rom_addr;
	wire [15:0] rom_rdata;
	ap_soft_rom rom(
		.iEN(rom_en),
		.iADDR(rom_addr),
		.oDATA(rom_rdata)
	);

	wire cpu_rom_en;
	wire cpu_rom_rw;
	wire cpu_rom_rdy;
	wire [31:0] cpu_rom_addr;
	wire [31:0] cpu_rom_rdata;
	ap_cpu_mem_16 cpu_rom(
		.iCLK(clk),

		.iCPU_EN(cpu_rom_en),
		.iCPU_RW(cpu_rom_rw),
		.oCPU_RDY(cpu_rom_rdy),
		.iCPU_ADDR(cpu_rom_addr),
		.oCPU_RDATA(cpu_rom_rdata),

		.oMEM_EN(rom_en),
		.oMEM_ADDR(rom_addr),
		.iMEM_RDATA(rom_rdata)
	);


	// RAM

	wire ram_en;
	wire ram_rw;
	wire [31:0] ram_addr;
	wire [15:0] ram_wdata;
	wire [15:0] ram_rdata;
	ap_soft_ram ram(
		.iEN(ram_en),
		.iRW(ram_rw),
		.iADDR(ram_addr),
		.iWDATA(ram_wdata),
		.oRDATA(ram_rdata)
	);

	wire cpu_ram_en;
	wire cpu_ram_rw;
	wire cpu_ram_rdy;
	wire [31:0] cpu_ram_addr;
	wire [31:0] cpu_ram_wdata;
	wire [31:0] cpu_ram_rdata;
	ap_cpu_mem_16 cpu_ram(
		.iCLK(clk),

		.iCPU_EN(cpu_ram_en),
		.iCPU_RW(cpu_ram_rw),
		.oCPU_RDY(cpu_ram_rdy),
		.iCPU_ADDR(cpu_ram_addr),
		.iCPU_WDATA(cpu_ram_wdata),
		.oCPU_RDATA(cpu_ram_rdata),

		.oMEM_EN(ram_en),
		.oMEM_RW(ram_rw),
		.oMEM_ADDR(ram_addr),
		.oMEM_WDATA(ram_wdata),
		.iMEM_RDATA(ram_rdata)
	);





	wire cpu_rw;
	wire cpu_req;
	reg cpu_rdy = 1'b1;
	wire [31:0] cpu_addr;
	reg [31:0] cpu_rdata;
	wire [31:0] cpu_wdata;
	ap_cpu cpu(
		.iCLK(clk),
		.oRW(cpu_rw),
		.oREQ(cpu_req),
		.iRDY(cpu_rdy),
		.oADDR(cpu_addr),
		.iDATA(cpu_rdata),
		.oDATA(cpu_wdata)
	);



	//=====================================


	assign cpu_rom_en = (cpu_addr < 32'h1000);
	assign cpu_rom_addr = cpu_addr;
	
	
	assign cpu_ram_en = (cpu_addr >= 32'h1000 && cpu_addr < 32'h2000);
	assign cpu_ram_rw = cpu_rw;
	assign cpu_ram_addr = cpu_addr - 32'h1000;
	assign cpu_ram_wdata = cpu_ram_en ? cpu_wdata : 32'hzzzz;


	always @ (*) begin
		if (rom_en) begin
			cpu_rdata = cpu_rom_rdata;
			cpu_rdy = cpu_rom_rdy;
		end
		else if (cpu_ram_en) begin
			cpu_rdata = cpu_ram_rdata;
			cpu_rdy = cpu_ram_rdy;
		end
		else begin
			cpu_rdata = 32'h0;
			cpu_rdy = 1'b0;
		end
	end


	// Generate clock.
	initial begin
		forever #1 begin
			clk <= !clk;
		end
	end


	// Simulate.
	initial begin
		$dumpfile("ap_cpu.vcd");
		$dumpvars(0, clk);
		$dumpvars(0, cpu_rw);
		$dumpvars(0, cpu_req);
		$dumpvars(0, cpu_rdy);
		$dumpvars(0, rom_en);
		$dumpvars(0, ram_en);

		repeat(30) @(posedge clk);
		$finish;
	end
			
	
endmodule