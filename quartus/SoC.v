//`define ENABLE_DDR2LP
//`define ENABLE_HSMC_XCVR
//`define ENABLE_SMA
//`define ENABLE_REFCLK
//`define ENABLE_GPIO

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
	reg [23:0] counter = 0;
	always @(posedge CLOCK_50_B5B) begin
		counter <= counter + 1;
	end
	
	wire clock = counter[10];
	*/
	
	
	/*
	reg clock = 1'b0;
	always @(posedge CLOCK_125_p) begin
		clock <= !clock;
	end
	*/

	
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
	SRAM_interface sram(
		.i_clock(clock),
		.i_enable(sram_enable),
		.i_rw(sram_rw),
		.i_address(sram_address),
		.i_wdata(sram_wdata),
		.o_rdata(sram_rdata),
		.o_ready(sram_ready),
		// ---
		.SRAM_A(SRAM_A),
		.SRAM_D(SRAM_D),
		.SRAM_CE_n(SRAM_CE_n),
		.SRAM_OE_n(SRAM_OE_n),
		.SRAM_WE_n(SRAM_WE_n)
	);
	
	// Mapped LEDS
	wire led_enable;
	wire led_rw;
	wire [31:0] led_address;
	wire [31:0] led_wdata;
	LED_Mapped led(
		.i_clock(clock),
		.i_enable(led_enable),
		.i_rw(led_rw),
		.i_address(led_address),
		.i_wdata(led_wdata),
		.o_leds(LEDR)
	);
	
	// UART
	wire uart_enable;
	wire uart_rw;
	wire [31:0] uart_wdata;
	wire uart_ready;
	UART #(
		50000000,
		9600
	) uart(
		.i_clock(CLOCK_50_B5B),
		.i_enable(uart_enable),
		.i_rw(uart_rw),
		.i_wdata(uart_wdata),
		.o_ready(uart_ready),
		// ---
		.UART_RX(UART_RX),
		.UART_TX(UART_TX)
	);

	// CPU
	wire cpu_rw;
	wire cpu_request;
	wire cpu_ready;
	wire [31:0] cpu_address;
	wire [31:0] cpu_rdata;
	wire [31:0] cpu_wdata;
	wire [31:0] cpu_pc;
	CPU cpu(
		.i_reset(reset),
		.i_clock(clock),
		.o_rw(cpu_rw),
		.o_request(cpu_request),
		.i_ready(cpu_ready),
		.o_address(cpu_address),
		.i_data(cpu_rdata),
		.o_data(cpu_wdata),
		.o_pc(cpu_pc)
	);

	assign LEDG = cpu_pc[7:0];
	
	//=====================================

	assign rom_enable = cpu_request && (cpu_address >= 32'h0000_0200 && cpu_address < 32'h0002_0000);
	assign rom_address = cpu_address - 32'h0000_0000;

	assign ram_enable = cpu_request && (cpu_address >= 32'h0002_0000 && cpu_address < 32'h0002_0000 + 32'h0000_8000);
	assign ram_rw = cpu_rw;
	assign ram_address = cpu_address - 32'h0002_0000;
	assign ram_wdata = cpu_wdata;

	assign sram_enable = cpu_request && (cpu_address >= 32'h0003_0000 && cpu_address < 32'h0003_0000 + 32'h0040_0000);
	assign sram_rw = cpu_rw;
	assign sram_address = cpu_address - 32'h0003_0000;
	assign sram_wdata = cpu_wdata;
	
	assign led_enable = cpu_request && (cpu_address >= 32'h1000_0000 && cpu_address < 32'h2000_0000);
	assign led_rw = cpu_rw;
	assign led_address = cpu_address - 32'h1000_0000;
	assign led_wdata = cpu_wdata;
	
	assign uart_enable = cpu_request && (cpu_address >= 32'h2000_0000 && cpu_address < 32'h3000_0000);
	assign uart_rw = cpu_rw;
	assign uart_wdata = cpu_wdata;
	
//	assign video_enable = cpu_request && (cpu_address >= 32'h10000000 && cpu_address < 32'h20000000);
//	assign video_rw = cpu_rw;
//	assign video_address = cpu_address - 32'h10000000;
//	assign video_wdata = cpu_wdata;

	assign cpu_rdata =
		rom_enable ? rom_rdata :
		ram_enable ? ram_rdata :
		sram_enable ? sram_rdata :
		32'h00000000;
		
	assign cpu_ready =
		rom_enable ? 1'b1 :
		ram_enable ? 1'b1 :
		sram_enable ? sram_ready :
		uart_enable ? uart_ready :
		1'b1;

endmodule
