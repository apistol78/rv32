//`define ENABLE_DDR2LP
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

	assign reset = !CPU_RESET_n;
	assign clock = CLOCK_50_B5B;
	
	/*
	reg [2:0] counter = 0;
	always @ (posedge CLOCK_50_B5B) begin
		counter <= counter + 1;
	end
	assign clock = counter[0];
	*/
  
	wire vga_enable;
	wire [15:0] vga_address;
  
	// VRAM
	wire vram_select;
	wire [31:0] vram_address;
	wire vram_ready;
	VRAM vram(
		.i_clock(CLOCK_50_B5B),
	
		.i_video_address(vga_address),
		.i_video_enable(vga_enable),
		.o_video_rdata(HDMI_TX_D),
		
		.i_request(vram_select && cpu_request),
		.i_rw(cpu_rw),
		.i_address(vram_address),
		.i_wdata(cpu_wdata),
		.o_ready(vram_ready)
	);
  
	// VGA signal generator
	VGA vga(
		.i_clock(clock),
		.o_hsync(HDMI_TX_HS),
		.o_vsync(HDMI_TX_VS),
		.o_data_enable(vga_enable),
		.o_vga_address(vga_address),
		.o_vga_clock(HDMI_TX_CLK)
	);
	
	assign HDMI_TX_DE = vga_enable;
	
	
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
	wire sram16_request;
	wire sram16_rw;
	wire [17:0] sram16_address;
	wire [15:0] sram16_wdata;
	wire [15:0] sram16_rdata;
	wire sram16_ready;
	SRAM_interface sram(
		.i_clock(clock),
		.i_request(sram16_request),
		.i_rw(sram16_rw),
		.i_address(sram16_address),
		.i_wdata(sram16_wdata),
		.o_rdata(sram16_rdata),
		.o_ready(sram16_ready),
		// ---
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
	Memory_16_to_32 sram32(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(sram32_select && cpu_request),
		.i_rw(cpu_rw),
		.i_address(sram32_address),
		.i_wdata(cpu_wdata),
		.o_rdata(sram32_rdata),
		.o_ready(sram32_ready),

		.o_ram_request(sram16_request),
		.o_ram_rw(sram16_rw),
		.o_ram_address(sram16_address),
		.o_ram_wdata(sram16_wdata),
		.i_ram_rdata(sram16_rdata),
		.i_ram_ready(sram16_ready)
	);

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
	
	// UART
	wire uart_select;
	wire [31:0] uart_rdata;
	wire uart_ready;
	wire uart_waiting;
	wire [1:0] uart_state;
	UART #(
		50000000,
		9600
	) uart(
		.i_reset(reset),
		.i_clock(CLOCK_50_B5B),
		.i_request(uart_select && cpu_request),
		.i_rw(cpu_rw),
		.i_wdata(cpu_wdata),
		.o_rdata(uart_rdata),
		.o_ready(uart_ready),
		.o_waiting(uart_waiting),
		.o_state(uart_state),
		// ---
		.UART_RX(UART_RX),
		.UART_TX(UART_TX)
	);
	
	/*
	// GPIO
	wire gpio_select;
	wire [31:0] gpio_address;
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
	*/

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

	// CPU
	wire cpu_rw;
	wire cpu_request;
	wire cpu_ready;
	wire [31:0] cpu_address;
	wire [31:0] cpu_rdata;
	wire [31:0] cpu_wdata;
	wire [2:0] cpu_state;
	CPU cpu(
		.i_reset(reset),
		.i_clock(clock),
		.o_rw(cpu_rw),
		.o_request(cpu_request),
		.i_ready(cpu_ready),
		.o_address(cpu_address),
		.i_data(cpu_rdata),
		.o_data(cpu_wdata),
		.o_state(cpu_state)
	);
	
	//=====================================

	assign rom_select = (cpu_address >= 32'h00000000 && cpu_address < 32'h00010000);
	assign rom_address = cpu_address - 32'h00000000;

	assign ram_select = (cpu_address >= 32'h00010000 && cpu_address < 32'h00020000);
	assign ram_address = cpu_address - 32'h00010000;

	assign sram32_select = (cpu_address >= 32'h10000000 && cpu_address < 32'h20000000);
	assign sram32_address = cpu_address - 32'h10000000;
	
	assign vram_select = (cpu_address >= 32'h40000000 && cpu_address < 32'h50000000);
	assign vram_address = cpu_address - 32'h40000000;
	
	assign led_select = (cpu_address >= 32'h50000000 && cpu_address < 32'h50000010);
	
	assign uart_select = (cpu_address >= 32'h50000010 && cpu_address < 32'h50000020);

	/*
	assign gpio_select = (cpu_address >= 32'h50000020 && cpu_address < 32'h50000030);
	assign gpio_address = cpu_address - 32'h50000020;
	*/
	
	assign i2c_select = (cpu_address >= 32'h50000030 && cpu_address < 32'h50000040);
	
	assign sd_select = (cpu_address >= 32'h50000040 && cpu_address < 32'h50000050);

	assign cpu_rdata =
		rom_select ? rom_rdata :
		ram_select ? ram_rdata :
		sram32_select ? sram32_rdata :
		uart_select ? uart_rdata :
		//gpio_select ? gpio_rdata :
		sd_select ? sd_rdata :
		i2c_select ? i2c_rdata :
		32'h00000000;
		
	assign cpu_ready =
		rom_select ? rom_ready :
		ram_select ? ram_ready :
		sram32_select ? sram32_ready :
		vram_select ? vram_ready :
		led_select ? led_ready :
		uart_select ? uart_ready :
		//gpio_select ? gpio_ready :
		sd_select ? sd_ready :
		i2c_select ? i2c_ready :
		1'b0;

	// 7:0
	assign LEDG = { SD_DAT, sd_select, cpu_state };

endmodule
