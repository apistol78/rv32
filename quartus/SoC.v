`define ENABLE_DDR2LP
//`define ENABLE_HSMC_XCVR
//`define ENABLE_SMA
//`define ENABLE_REFCLK
`define ENABLE_GPIO

module SoC(

      ///////// ADC ///////// 1.2 V ///////
      output             ADC_CONVST,
      output             ADC_SCK,
      output             ADC_SDI,
      input              ADC_SDO,

      ///////// AUD ///////// 2.5 V ///////
      input              AUD_ADCDAT,
      inout              AUD_ADCLRCK,
      inout              AUD_BCLK,
      output             AUD_DACDAT,
      inout              AUD_DACLRCK,
      output             AUD_XCK,

      ///////// CLOCK /////////
      input              CLOCK_125_p, ///LVDS
      input              CLOCK_50_B5B, ///3.3-V LVTTL
      input              CLOCK_50_B6A,
      input              CLOCK_50_B7A, ///2.5 V
      input              CLOCK_50_B8A,

      ///////// CPU /////////
      input              CPU_RESET_n, ///3.3V LVTTL

`ifdef ENABLE_DDR2LP
      ///////// DDR2LP ///////// 1.2-V HSUL ///////
      output      [9:0]  DDR2LP_CA,
      output      [1:0]  DDR2LP_CKE,
      output             DDR2LP_CK_n, ///DIFFERENTIAL 1.2-V HSUL
      output             DDR2LP_CK_p, ///DIFFERENTIAL 1.2-V HSUL
      output      [1:0]  DDR2LP_CS_n,
      output      [3:0]  DDR2LP_DM,
      inout       [31:0] DDR2LP_DQ,
      inout       [3:0]  DDR2LP_DQS_n, ///DIFFERENTIAL 1.2-V HSUL
      inout       [3:0]  DDR2LP_DQS_p, ///DIFFERENTIAL 1.2-V HSUL
      input              DDR2LP_OCT_RZQ, ///1.2 V
`endif /*ENABLE_DDR2LP*/

`ifdef ENABLE_GPIO
      ///////// GPIO ///////// 3.3-V LVTTL ///////
      inout       [35:0] GPIO,
`else	
      ///////// HEX2 ///////// 1.2 V ///////
      output      [6:0]  HEX2,

      ///////// HEX3 ///////// 1.2 V ///////
      output      [6:0]  HEX3,		
		
		
`endif /*ENABLE_GPIO*/

      ///////// HDMI /////////
      output             HDMI_TX_CLK,
      output      [23:0] HDMI_TX_D,
      output             HDMI_TX_DE,
      output             HDMI_TX_HS,
      input              HDMI_TX_INT,
      output             HDMI_TX_VS,

      ///////// HEX0 /////////
      output      [6:0]  HEX0,

      ///////// HEX1 /////////
      output      [6:0]  HEX1,


      ///////// HSMC ///////// 2.5 V ///////
      input              HSMC_CLKIN0,
      input       [2:1]  HSMC_CLKIN_n,
      input       [2:1]  HSMC_CLKIN_p,
      output             HSMC_CLKOUT0,
      output      [2:1]  HSMC_CLKOUT_n,
      output      [2:1]  HSMC_CLKOUT_p,
      inout       [3:0]  HSMC_D,
`ifdef ENABLE_HSMC_XCVR		
      input       [3:0]  HSMC_GXB_RX_p, /// 1.5-V PCML
      output      [3:0]  HSMC_GXB_TX_p, /// 1.5-V PCML
`endif /*ENABLE_HSMC_XCVR*/		
      inout       [16:0] HSMC_RX_n,
      inout       [16:0] HSMC_RX_p,
      inout       [16:0] HSMC_TX_n,
      inout       [16:0] HSMC_TX_p,


      ///////// I2C ///////// 2.5 V ///////
      output             I2C_SCL,
      inout              I2C_SDA,

      ///////// KEY ///////// 1.2 V ///////
      input       [3:0]  KEY,

      ///////// LEDG ///////// 2.5 V ///////
      output      [7:0]  LEDG,

      ///////// LEDR ///////// 2.5 V ///////
      output      [9:0]  LEDR,

`ifdef ENABLE_REFCLK
      ///////// REFCLK ///////// 1.5-V PCML ///////
      input              REFCLK_p0,
      input              REFCLK_p1,
`endif /*ENABLE_REFCLK*/

      ///////// SD ///////// 3.3-V LVTTL ///////
      output             SD_CLK,
      inout              SD_CMD,
      inout       [3:0]  SD_DAT,

`ifdef ENABLE_SMA
      ///////// SMA ///////// 1.5-V PCML ///////
      input              SMA_GXB_RX_p,
      output             SMA_GXB_TX_p,
`endif /*ENABLE_SMA*/

      ///////// SRAM ///////// 3.3-V LVTTL ///////
      output      [17:0] SRAM_A,
      output             SRAM_CE_n,
      inout       [15:0] SRAM_D,
      output             SRAM_LB_n,
      output             SRAM_OE_n,
      output             SRAM_UB_n,
      output             SRAM_WE_n,

      ///////// SW ///////// 1.2 V ///////
      input       [9:0]  SW,

      ///////// UART ///////// 2.5 V ///////
      input              UART_RX,
      output             UART_TX
);

`define SOC_ENABLE_SDRAM
`define SOC_ENABLE_VGA
`define SOC_ENABLE_UART
`define SOC_ENABLE_I2C
`define SOC_ENABLE_SD

//	wire clock = CLOCK_125_p;

	wire clock;
	IP_PLL_Clk pll_clk(
		.refclk(CLOCK_125_p),
		.rst(!CPU_RESET_n),
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
			sample[4:0] = { sample[3:0], KEY[0] };
		else 
			sample[4:0] = sample[4:0];
	end

	assign soft_reset_n = (sample[1:0] == 2'b10) ? 1'b0 : 1'b1;
	assign global_reset_n = (sample[3:2] == 2'b10) ? 1'b0 : 1'b1;
	assign start_n = (sample[4:3] == 2'b01) ? 1'b0 : 1'b1;
	
	assign reset = !start_n; // CPU_RESET_n;

  
`ifdef SOC_ENABLE_VGA
	// Video memory.
	wire video_sram_request;
	wire video_sram_rw;
	wire [31:0] video_sram_address;
	wire [31:0] video_sram_wdata;
	wire [31:0] video_sram_rdata;
	wire video_sram_ready;

	SRAM_interface video_sram(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(video_sram_request),
		.i_rw(video_sram_rw),
		.i_address(video_sram_address),
		.i_wdata(video_sram_wdata),
		.o_rdata(video_sram_rdata),
		.o_ready(video_sram_ready),
		// ---
		.SRAM_A(SRAM_A),
		.SRAM_D(SRAM_D),
		.SRAM_CE_n(SRAM_CE_n),
		.SRAM_OE_n(SRAM_OE_n),
		.SRAM_WE_n(SRAM_WE_n),
		.SRAM_LB_n(SRAM_LB_n),
		.SRAM_UB_n(SRAM_UB_n)
	);
	/*
	BRAM #(
		.WIDTH(32),
		.SIZE(320*200),
		.ADDR_LSH(2)
	) video_sram(
		.i_clock(clock),
		.i_request(video_sram_request),
		.i_rw(video_sram_rw),
		.i_address(video_sram_address),
		.i_wdata(video_sram_wdata),
		.o_rdata(video_sram_rdata),
		.o_ready(video_sram_ready)
	);
	*/

	// Video controller.
	wire vram_select;
	wire [31:0] vram_address;
	wire vram_ready;
	VideoBus video_bus(
		.i_clock(clock),
		
		.i_cpu_request(vram_select && cpu_request),
		.i_cpu_address(vram_address),
		.i_cpu_wdata(cpu_wdata),
		.o_cpu_ready(vram_ready),
		
		.i_video_request(vga_enable),
		.i_video_address(vga_address),
		.o_video_rdata(HDMI_TX_D),
		
		.o_mem_request(video_sram_request),
		.o_mem_rw(video_sram_rw),
		.o_mem_address(video_sram_address),
		.o_mem_wdata(video_sram_wdata),
		.i_mem_rdata(video_sram_rdata),
		.i_mem_ready(video_sram_ready)
	);
  
	// Video signal generator
	wire vga_enable;
	wire [15:0] vga_address;
	VGA #(
		.PRESCALE(100000000 / 25000000)
	) vga(
		.i_clock(clock),
		.o_hsync(HDMI_TX_HS),
		.o_vsync(HDMI_TX_VS),
		.o_data_enable(vga_enable),
		.o_vga_address(vga_address),
		.o_vga_clock(HDMI_TX_CLK)
	);
	
	assign HDMI_TX_DE = vga_enable;
`endif
	
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

`ifdef SOC_ENABLE_SDRAM
	// SDRAM
	wire sdram_select;
	wire [31:0] sdram_address;
	wire [31:0] sdram_rdata;
	wire sdram_ready;
	SDRAM_interface sdram(
		.i_global_reset_n(global_reset_n),
		.i_soft_reset_n(soft_reset_n),
		// ---
		.i_reset(reset),
		.i_clock(clock),
		.i_request(sdram_select && cpu_request),
		.i_rw(cpu_rw),
		.i_address(sdram_address),
		.i_wdata(cpu_wdata),
		.o_rdata(sdram_rdata),
		.o_ready(sdram_ready),
		// ---
		.DDR2LP_CA(DDR2LP_CA),
		.DDR2LP_CKE(DDR2LP_CKE),
		.DDR2LP_CK_n(DDR2LP_CK_n),
		.DDR2LP_CK_p(DDR2LP_CK_p),
		.DDR2LP_CS_n(DDR2LP_CS_n),
		.DDR2LP_DM(DDR2LP_DM),
		.DDR2LP_DQ(DDR2LP_DQ),
		.DDR2LP_DQS_n(DDR2LP_DQS_n),
		.DDR2LP_DQS_p(DDR2LP_DQS_p),
		.DDR2LP_OCT_RZQ(DDR2LP_OCT_RZQ)
	);
`endif

	// LEDS
	wire led_select;
	wire led_ready;
	LED led(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(led_select && cpu_request),
		.i_wdata(cpu_wdata),
		.o_ready(led_ready),
		.LEDR(LEDR)
	);
	
`ifdef SOC_ENABLE_UART
	// UART
	wire uart_select;
	wire [1:0] uart_address;
	wire [31:0] uart_rdata;
	wire uart_ready;
	UART #(
		//.PRESCALE(50000000 / (115200 * 8))
		.PRESCALE(100000000 / (115200 * 8))
	) uart(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(uart_select && cpu_request),
		.i_rw(cpu_rw),
		.i_address(uart_address),
		.i_wdata(cpu_wdata),
		.o_rdata(uart_rdata),
		.o_ready(uart_ready),
		// ---
		.UART_RX(UART_RX),
		.UART_TX(UART_TX)
	);
`endif
	
`ifdef SOC_ENABLE_GPIO
	// GPIO
	wire gpio_select;
	wire [1:0] gpio_address;
	wire [31:0] gpio_rdata;
	wire gpio_ready;
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
`endif

`ifdef SOC_ENABLE_I2C
	// I2C
	wire i2c_select;
	wire [31:0] i2c_rdata;
	wire i2c_ready;
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
`endif

`ifdef SOC_ENABLE_SD
	// SD
	wire sd_select;
	wire [31:0] sd_rdata;
	wire sd_ready;
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
`endif

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
		.i_request(timer_select && cpu_request),
		.i_address(timer_address),
		.o_rdata(timer_rdata),
		.o_ready(timer_ready),
		
		// Debug
		.i_retire_count(cpu_retire_count)
	);
	
	// CPU
	wire cpu_rw;
	wire cpu_request;
	wire cpu_ready;
	wire [31:0] cpu_address;
	wire [31:0] cpu_rdata;
	wire [31:0] cpu_wdata;
	wire [31:0] cpu_retire_count;
	CPU_v2 cpu(
        .i_reset(reset),
		.i_clock(clock),

		// Bus
		.o_bus_rw(cpu_rw),
		.o_bus_request(cpu_request),
		.i_bus_ready(cpu_ready),
		.o_bus_address(cpu_address),
		.i_bus_rdata(cpu_rdata),
		.o_bus_wdata(cpu_wdata),
		
		// Debug
		.o_retire_count(cpu_retire_count)
	);
	
	//=====================================

	assign rom_select = (cpu_address < 32'h00010000);
	assign rom_address = cpu_address - 32'h00000000;

	assign ram_select = (cpu_address >= 32'h00010000 && cpu_address < 32'h00020000);
	assign ram_address = cpu_address - 32'h00010000;

`ifdef SOC_ENABLE_SDRAM
	assign sdram_select = (cpu_address >= 32'h20000000 && cpu_address < 32'h40000000);
	assign sdram_address = cpu_address - 32'h20000000;
`endif

`ifdef SOC_ENABLE_VGA
	assign vram_select = (cpu_address >= 32'h40000000 && cpu_address < 32'h50000000);
	assign vram_address = cpu_address - 32'h40000000;
`endif

	assign led_select = (cpu_address >= 32'h50000000 && cpu_address < 32'h50000010);
	
`ifdef SOC_ENABLE_UART
	assign uart_select = (cpu_address >= 32'h50000010 && cpu_address < 32'h50000020);
	assign uart_address = cpu_address[3:2]; // - 32'h50000010;
`endif

`ifdef SOC_ENABLE_GPIO
	assign gpio_select = (cpu_address >= 32'h50000020 && cpu_address < 32'h50000030);
	assign gpio_address = cpu_address[3:2]; // - 32'h50000020;
`endif
	
`ifdef SOC_ENABLE_I2C
	assign i2c_select = (cpu_address >= 32'h50000030 && cpu_address < 32'h50000040);
`endif

`ifdef SOC_ENABLE_SD
	assign sd_select = (cpu_address >= 32'h50000040 && cpu_address < 32'h50000050);
`endif

	assign timer_select = (cpu_address >= 32'h50000050 && cpu_address < 32'h50000060);
	assign timer_address = cpu_address[3:2]; // - 32'h50000050;

	assign cpu_rdata =
		rom_select ? rom_rdata :
		ram_select ? ram_rdata :
`ifdef SOC_ENABLE_SRAM
		sram_select ? sram_rdata :
`endif
`ifdef SOC_ENABLE_SDRAM
		sdram_select ? sdram_rdata :
`endif
`ifdef SOC_ENABLE_UART
		uart_select ? uart_rdata :
`endif
`ifdef SOC_ENABLE_GPIO
		gpio_select ? gpio_rdata :
`endif
`ifdef SOC_ENABLE_SD
		sd_select ? sd_rdata :
`endif
`ifdef SOC_ENABLE_I2C
		i2c_select ? i2c_rdata :
`endif
		timer_select ? timer_rdata :
		32'h00000000;
		
	assign cpu_ready =
		rom_select ? rom_ready :
		ram_select ? ram_ready :
`ifdef SOC_ENABLE_SDRAM
		sdram_select ? sdram_ready :
`endif
`ifdef SOC_ENABLE_VGA
		vram_select ? vram_ready :
`endif
		led_select ? led_ready :
`ifdef SOC_ENABLE_UART
		uart_select ? uart_ready :
`endif
`ifdef SOC_ENABLE_GPIO
		gpio_select ? gpio_ready :
`endif
`ifdef SOC_ENABLE_SD
		sd_select ? sd_ready :
`endif
`ifdef SOC_ENABLE_I2C
		i2c_select ? i2c_ready :
`endif
		timer_select ? timer_ready :
		1'b0;

	// 7:0
	assign LEDG = { soft_reset_n, global_reset_n, start_n };

endmodule
