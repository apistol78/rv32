`include "CPU_Defines.sv"

`define ENABLE_DDR2LP
//`define ENABLE_HSMC_XCVR
//`define ENABLE_SMA
//`define ENABLE_REFCLK
`define ENABLE_GPIO

`timescale 1ns/1ns

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
`ifndef __VERILATOR__
      inout              SD_CMD,
      inout       [3:0]  SD_DAT,
`else
      input              SD_CMD_in,
      input       [3:0]  SD_DAT_in,
      output             SD_CMD_out,
      output      [3:0]  SD_DAT_out,
`endif

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

`define FREQUENCY 100000000

`define SOC_ENABLE_SDRAM
// `define SOC_ENABLE_SDRAM_L2CACHE
`define SOC_ENABLE_VGA
`define SOC_ENABLE_UART
// `define SOC_ENABLE_GPIO
`define SOC_ENABLE_I2C
`define SOC_ENABLE_SD
`define SOC_ENABLE_AUDIO

	// Since we want to share pins with HW
	// this clock will actually be simulated at 100 MHz.
	wire clock = CLOCK_125_p;
	wire reset = !CPU_RESET_n;
  
`ifdef SOC_ENABLE_VGA
	// Video signal generator
	wire vga_enable;
	wire [8:0] vga_pos_x;
	wire [8:0] vga_pos_y;
	VGA #(
		.PRESCALE(`FREQUENCY / 25000000)
	) vga(
		.i_clock(clock),
		.o_hsync(HDMI_TX_HS),
		.o_vsync(HDMI_TX_VS),
		.o_data_enable(vga_enable),
		.o_pos_x(vga_pos_x),
		.o_pos_y(vga_pos_y),
		.o_vga_clock(HDMI_TX_CLK)
	);
	
	assign HDMI_TX_DE = vga_enable;

	// Video memory.
	wire video_sram_request;
	wire video_sram_rw;
	wire [31:0] video_sram_address;
	wire [31:0] video_sram_wdata;
	wire [31:0] video_sram_rdata;
	wire video_sram_ready;
	BRAM_latency #(
		.WIDTH(32),
		.SIZE(320*200/4),
		.ADDR_LSH(2),
		.LATENCY(7)
	) video_sram(
		.i_clock(clock),
		.i_request(video_sram_request),
		.i_rw(video_sram_rw),
		.i_address(video_sram_address),
		.i_wdata(video_sram_wdata),
		.o_rdata(video_sram_rdata),
		.o_ready(video_sram_ready)
	);

	// Video framebuffer controller.
	wire vbus_request;
	wire [31:0] vbus_address;
	wire [31:0] vbus_wdata;
	wire vbus_ready;
	wire vbus_fifo_full;
	VideoBus video_bus(
		.i_clock(clock),
		
		// CPU interface.
		.i_cpu_request(vbus_request),
		.i_cpu_address(vbus_address),
		.i_cpu_wdata(vbus_wdata),
		.o_cpu_ready(vbus_ready),
		
		// Video signal interface.
		.i_video_request(vga_enable),
		.i_video_pos_x(vga_pos_x),
		.i_video_pos_y(vga_pos_y),
		.o_video_rdata(HDMI_TX_D),
		
		// Video RAM interface.
		.o_mem_request(video_sram_request),
		.o_mem_rw(video_sram_rw),
		.o_mem_address(video_sram_address),
		.o_mem_wdata(video_sram_wdata),
		.i_mem_rdata(video_sram_rdata),
		.i_mem_ready(video_sram_ready),
		
		.o_fifo_full(vbus_fifo_full)
	);

	// Multiplex access to framebuffer from CPU and GPU.
	wire vram_select;
	wire [31:0] vram_address;
	wire vram_ready;
	BusAccess #(
		.REGISTERED(1'b0)
	) cpu_gpu_vbus_mux(
		.i_reset(reset),
		.i_clock(clock),
		
		// Output to video framebuffer.
		.o_bus_rw(),
		.o_bus_request(vbus_request),
		.i_bus_ready(vbus_ready),
		.o_bus_address(vbus_address),
		.i_bus_rdata(),
		.o_bus_wdata(vbus_wdata),

		// NC
		.i_pa_request(1'b0),
		.o_pa_ready(),
		.i_pa_address(0),
		.o_pa_rdata(),

		// CPU port.
		.i_pb_rw(1'b1),
		.i_pb_request(vram_select && bus_request),
		.o_pb_ready(vram_ready),
		.i_pb_address(vram_address),
		.o_pb_rdata(),
		.i_pb_wdata(bus_wdata),

		// GPU port.
		.i_pc_rw(),
		.i_pc_request(gpu_fb_request),
		.o_pc_ready(gpu_fb_ready),
		.i_pc_address(gpu_fb_address),
		.o_pc_rdata(),
		.i_pc_wdata(gpu_fb_wdata)
	);

	// GPU
	wire gpu_select;
	wire [1:0] gpu_address;
	wire [31:0] gpu_rdata;
	wire gpu_ready;

	wire gpu_fb_request;
	wire gpu_fb_ready;
	wire [31:0] gpu_fb_address;
	wire [31:0] gpu_fb_wdata;

	GPU gpu(
		.i_reset(reset),
		.i_clock(clock),

		// Command interface.
		.i_request(gpu_select && bus_request),
		.i_rw(bus_rw),
		.i_address(gpu_address),
		.i_wdata(bus_wdata),
		.o_rdata(gpu_rdata),
		.o_ready(gpu_ready),

		// Framebuffer output.
		.o_fb_request(gpu_fb_request),
		.i_fb_ready(gpu_fb_ready),
		.o_fb_address(gpu_fb_address),
		.o_fb_wdata(gpu_fb_wdata)
	);

`endif
	
	// ROM
	wire rom_select;
	wire [31:0] rom_address;
	wire [31:0] rom_rdata;
	wire rom_ready;
	BROM rom(
		.i_clock(clock),
		.i_request(rom_select && bus_request),
		.i_address(rom_address),
		.o_rdata(rom_rdata),
		.o_ready(rom_ready)
	);

	// RAM
	wire ram_select;
	wire [31:0] ram_address;
	wire [31:0] ram_rdata;
	wire ram_ready;
	BRAM #(
		.WIDTH(32),
		.SIZE(32'h8000),
		.ADDR_LSH(2)
	) ram(
		.i_clock(clock),
		.i_request(ram_select && bus_request),
		.i_rw(bus_rw),
		.i_address(ram_address),
		.i_wdata(bus_wdata),
		.o_rdata(ram_rdata),
		.o_ready(ram_ready)
	);

`ifdef SOC_ENABLE_SDRAM
	// SDRAM
	wire sdram_select;
	wire [31:0] sdram_address;
	wire [31:0] sdram_rdata;
	wire sdram_ready;

	BRAM/*_latency*/ #(
		.WIDTH(32),
		.SIZE(32'h1000000 / 4),
		.ADDR_LSH(2)/*,
		.LATENCY(10)*/
	) sdram(
		.i_clock(clock),
		.i_request(l2cache_bus_request),
		.i_rw(l2cache_bus_rw),
		.i_address(l2cache_bus_address),
		.i_wdata(l2cache_bus_wdata),
		.o_rdata(l2cache_bus_rdata),
		.o_ready(l2cache_bus_ready)
	);

	wire l2cache_bus_rw;
	wire l2cache_bus_request;
	wire l2cache_bus_ready;
	wire [31:0] l2cache_bus_address;
	wire [31:0] l2cache_bus_rdata;
	wire [31:0] l2cache_bus_wdata;
`ifdef SOC_ENABLE_SDRAM_L2CACHE
	CPU_L2_Cache l2cache(
		.i_reset(reset),
		.i_clock(clock),

		.o_bus_rw(l2cache_bus_rw),
		.o_bus_request(l2cache_bus_request),
		.i_bus_ready(l2cache_bus_ready),
		.o_bus_address(l2cache_bus_address),
		.i_bus_rdata(l2cache_bus_rdata),
		.o_bus_wdata(l2cache_bus_wdata),

		.i_request(sdram_select && bus_request),
		.i_rw(bus_rw),
		.i_address(sdram_address),
		.i_wdata(bus_wdata),
		.o_rdata(sdram_rdata),
		.o_ready(sdram_ready)
	);
`else
	assign l2cache_bus_request = sdram_select && bus_request;
	assign l2cache_bus_rw = bus_rw;
	assign sdram_ready = l2cache_bus_ready;
	assign l2cache_bus_address = sdram_address;
	assign sdram_rdata = l2cache_bus_rdata;
	assign l2cache_bus_wdata = bus_wdata;
`endif

`endif

	// LEDS
	wire led_select;
	wire led_ready;
	LED led(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(led_select && bus_request),
		.i_wdata(bus_wdata),
		.o_ready(led_ready),
		.LEDR(LEDR)
	);
	
`ifdef SOC_ENABLE_UART
	// UART (USB)
	wire uart_0_select;
	wire [1:0] uart_0_address;
	wire [31:0] uart_0_rdata;
	wire uart_0_ready;
	UART #(
		.PRESCALE(`FREQUENCY / (115200 * 8))
	) uart_0(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(uart_0_select && bus_request),
		.i_rw(bus_rw),
		.i_address(uart_0_address),
		.i_wdata(bus_wdata),
		.o_rdata(uart_0_rdata),
		.o_ready(uart_0_ready),
		// ---
		.UART_RX(UART_RX),
		.UART_TX(UART_TX)
	);

	// UART (GPIO)
	wire uart_1_select;
	wire [1:0] uart_1_address;
	wire [31:0] uart_1_rdata;
	wire uart_1_ready;
	UART #(
		.PRESCALE(`FREQUENCY / (115200 * 8))
	) uart_1(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(uart_1_select && bus_request),
		.i_rw(bus_rw),
		.i_address(uart_1_address),
		.i_wdata(bus_wdata),
		.o_rdata(uart_1_rdata),
		.o_ready(uart_1_ready),
		// ---
		.UART_RX(GPIO[0]),
		.UART_TX(GPIO[1])
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
		.i_request(gpio_select && bus_request),
		.i_rw(bus_rw),
		.i_address(gpio_address),
		.i_wdata(bus_wdata),
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
		.i_request(i2c_select && bus_request),
		.i_rw(bus_rw),
		.i_wdata(bus_wdata),
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
		.i_request(sd_select && bus_request),
		.i_rw(bus_rw),
		.i_wdata(bus_wdata),
		.o_rdata(sd_rdata),
		.o_ready(sd_ready),
		// ---
		.SD_CLK(SD_CLK),
		.SD_CMD_in(SD_CMD_in),
		.SD_DAT_in(SD_DAT_in),
		.SD_CMD_out(SD_CMD_out),
		.SD_DAT_out(SD_DAT_out)
	);
`endif

`ifdef SOC_ENABLE_AUDIO
	// Audio
	wire audio_pwm_output_busy;
	wire [15:0] audio_pwm_output_sample;
	AUDIO_pwm_output audio_pwm_output(
		.i_clock(clock),
		.o_busy(audio_pwm_output_busy),
		.i_sample(audio_pwm_output_sample),
		.o_pwm(GPIO[0])
	);

	wire audio_select;
	wire audio_ready;
	AUDIO_controller audio_controller(
		.i_reset(reset),
		.i_clock(clock),

		.i_request(audio_select && bus_request),
		.i_wdata(bus_wdata[15:0]),
		.o_ready(audio_ready),

		.i_output_busy(audio_pwm_output_busy),
		.o_output_sample(audio_pwm_output_sample)
	);
`endif

	// DMA
	wire dma_select;
	wire [1:0] dma_address;
	wire [31:0] dma_rdata;
	wire dma_ready;

	wire dma_bus_rw;
	wire dma_bus_request;
	wire dma_bus_ready;
	wire [31:0] dma_bus_address;
	wire [31:0] dma_bus_rdata;
	wire [31:0] dma_bus_wdata;

	DMA dma(
		.i_reset(reset),
		.i_clock(clock),

		// CPU
		.i_request(dma_select && bus_request),
		.i_rw(bus_rw),
		.i_address(dma_address),
		.i_wdata(bus_wdata),
		.o_rdata(dma_rdata),
		.o_ready(dma_ready),

		// System
		.i_stall(vbus_fifo_full),
		
		// Bus
		.o_bus_rw(dma_bus_rw),
		.o_bus_request(dma_bus_request),
		.i_bus_ready(dma_bus_ready),
		.o_bus_address(dma_bus_address),
		.i_bus_rdata(dma_bus_rdata),
		.o_bus_wdata(dma_bus_wdata)
	);

	// Timer
	wire timer_select;
	wire [2:0] timer_address;
	wire [31:0] timer_rdata;
	wire timer_ready;
	wire timer_interrupt;
	Timer #(
		.FREQUENCY(`FREQUENCY)
	) timer(
		.i_reset(reset),
		.i_clock(clock),
		.i_request(timer_select && bus_request),
		.i_rw(bus_rw),
		.i_address(timer_address),
		.i_wdata(bus_wdata),
		.o_rdata(timer_rdata),
		.o_ready(timer_ready),
		.o_interrupt(timer_interrupt)
	);

	// PLIC
	wire plic_interrupt;
	wire plic_select;
	wire [23:0] plic_address;
	wire [31:0] plic_rdata;
	wire plic_ready;
	PLIC plic(
		.i_reset(reset),
		.i_clock(clock),

		.i_interrupt_0(0),
		.i_interrupt_1(0),
		.i_interrupt_2(0),
		.i_interrupt_3(0),

		.o_interrupt(plic_interrupt),

		.i_request(plic_select && bus_request),
		.i_rw(bus_rw),
		.i_address(plic_address),
		.i_wdata(bus_wdata),
		.o_rdata(plic_rdata),
		.o_ready(plic_ready)
	);
	
	//====================================================

	// Single port bus.
	wire bus_rw;
	wire bus_request;
	wire bus_ready;
	wire [31:0] bus_address;
	wire [31:0] bus_rdata;
	wire [31:0] bus_wdata;

	wire bus_pa_request;
	wire bus_pa_ready;
	wire [31:0] bus_pa_address;
	wire [31:0] bus_pa_rdata;

	wire bus_pb_rw;
	wire bus_pb_request;
	wire bus_pb_ready;
	wire [31:0] bus_pb_address;
	wire [31:0] bus_pb_rdata;
	wire [31:0] bus_pb_wdata;

	BusAccess #(
		.REGISTERED(1)
	) bus(
		.i_reset(reset),
		.i_clock(clock),

		// Bus
		.o_bus_rw(bus_rw),
		.o_bus_request(bus_request),
		.i_bus_ready(bus_ready),
		.o_bus_address(bus_address),
		.i_bus_rdata(bus_rdata),
		.o_bus_wdata(bus_wdata),

		// Port A (Instruction bus)
		.i_pa_request(cpu_ibus_request),
		.o_pa_ready(cpu_ibus_ready),
		.i_pa_address(cpu_ibus_address),
		.o_pa_rdata(cpu_ibus_rdata),

		// Port B (Data bus)
		.i_pb_rw(cpu_dbus_rw),
		.i_pb_request(cpu_dbus_request),
		.o_pb_ready(cpu_dbus_ready),
		.i_pb_address(cpu_dbus_address),
		.o_pb_rdata(cpu_dbus_rdata),
		.i_pb_wdata(cpu_dbus_wdata),

		// Port C (DMA)
		.i_pc_rw(dma_bus_rw),
		.i_pc_request(dma_bus_request),
		.o_pc_ready(dma_bus_ready),
		.i_pc_address(dma_bus_address),
		.o_pc_rdata(dma_bus_rdata),
		.i_pc_wdata(dma_bus_wdata)
	);

	// CPU
	wire cpu_ibus_request;
	wire cpu_ibus_ready;
	wire [31:0] cpu_ibus_address;
	wire [31:0] cpu_ibus_rdata;
	wire cpu_dbus_rw;
	wire cpu_dbus_request;
	wire cpu_dbus_ready;
	wire [31:0] cpu_dbus_address;
	wire [31:0] cpu_dbus_rdata;
	wire [31:0] cpu_dbus_wdata;
	wire cpu_pipeline_disable;
	wire [`TAG_SIZE] cpu_execute_debug_tag;
	wire [`TAG_SIZE] cpu_writeback_debug_tag;
	wire cpu_fault;

	CPU cpu(
        .i_reset(reset),
		.i_clock(clock),

		// Control
		.i_timer_interrupt(timer_interrupt),
		.i_external_interrupt(plic_interrupt),

		// Instruction bus
		.o_ibus_request(cpu_ibus_request),
		.i_ibus_ready(cpu_ibus_ready),
		.o_ibus_address(cpu_ibus_address),
		.i_ibus_rdata(cpu_ibus_rdata),
		
		// Data bus
		.o_dbus_rw(cpu_dbus_rw),
		.o_dbus_request(cpu_dbus_request),
		.i_dbus_ready(cpu_dbus_ready),
		.o_dbus_address(cpu_dbus_address),
		.i_dbus_rdata(cpu_dbus_rdata),
		.o_dbus_wdata(cpu_dbus_wdata),

		// Debug
		.i_pipeline_disable(cpu_pipeline_disable),
		.o_execute_debug_tag(cpu_execute_debug_tag),
		.o_writeback_debug_tag(cpu_writeback_debug_tag),
		.o_fault(cpu_fault)
	);

	//=====================================

	assign rom_select = bus_address[31:28] == 4'h0;
	assign rom_address = { 4'h0, bus_address[27:0] };

	assign ram_select = bus_address[31:28] == 4'h1;
	assign ram_address = { 4'h0, bus_address[27:0] };

`ifdef SOC_ENABLE_SDRAM
	assign sdram_select = bus_address[31:28] == 4'h2;
	assign sdram_address = { 4'h0, bus_address[27:0] };
`endif

`ifdef SOC_ENABLE_VGA
	assign vram_select = bus_address[31:28] == 4'h3;
	assign vram_address = { 4'h0, bus_address[27:0] };
`endif

	assign led_select = bus_address[31:28] == 4'h4;

`ifdef SOC_ENABLE_UART
	assign uart_0_select = bus_address[31:24] == 8'h50;
	assign uart_0_address = bus_address[3:2];
	assign uart_1_select = bus_address[31:24] == 8'h51;
	assign uart_1_address = bus_address[3:2];
`endif

`ifdef SOC_ENABLE_GPIO
	assign gpio_select = bus_address[31:28] == 4'h6;
	assign gpio_address = bus_address[3:2];
`endif

`ifdef SOC_ENABLE_I2C
	assign i2c_select = bus_address[31:28] == 4'h7;
`endif

`ifdef SOC_ENABLE_SD
	assign sd_select = bus_address[31:28] == 4'h8;
`endif

`ifdef SOC_ENABLE_AUDIO
	assign audio_select = bus_address[31:28] == 4'hd;
`endif

	assign dma_select = bus_address[31:28] == 4'h9;
	assign dma_address = bus_address[3:2];

	assign timer_select = bus_address[31:28] == 4'ha;
	assign timer_address = bus_address[4:2];

	assign plic_select = bus_address[31:28] == 4'hb;
	assign plic_address = bus_address[23:0];

	assign gpu_select = bus_address[31:28] == 4'hc;
	assign gpu_address = bus_address[3:2];

	//=====================================

	assign bus_rdata =
		rom_select ? rom_rdata			:
		ram_select ? ram_rdata			:
`ifdef SOC_ENABLE_SRAM
		sram_select ? sram_rdata		:
`endif
`ifdef SOC_ENABLE_SDRAM
		sdram_select ? sdram_rdata		:
`endif
`ifdef SOC_ENABLE_UART
		uart_0_select ? uart_0_rdata	:
		uart_1_select ? uart_1_rdata	:
`endif
`ifdef SOC_ENABLE_GPIO
		gpio_select ? gpio_rdata		:
`endif
`ifdef SOC_ENABLE_I2C
		i2c_select ? i2c_rdata			:
`endif
`ifdef SOC_ENABLE_SD
		sd_select ? sd_rdata			:
`endif
		dma_select ? dma_rdata			:
		timer_select ? timer_rdata		:
		plic_select ? plic_rdata		:
		gpu_select ? gpu_rdata			:
		32'h00000000;
		
	assign bus_ready =
		rom_ready				|
		ram_ready				|
`ifdef SOC_ENABLE_SDRAM		
		sdram_ready				|
`endif		
`ifdef SOC_ENABLE_VGA		
		vram_ready				|
`endif		
		led_ready				|
`ifdef SOC_ENABLE_UART		
		uart_0_ready			|
		uart_1_ready			|
`endif		
`ifdef SOC_ENABLE_GPIO		
		gpio_ready				|
`endif		
`ifdef SOC_ENABLE_SD		
		sd_ready				|
`endif		
`ifdef SOC_ENABLE_I2C		
		i2c_ready				|
`endif
`ifdef SOC_ENABLE_AUDIO
		audio_ready				|
`endif
		dma_ready				|
		timer_ready				|
		plic_ready				|
		gpu_ready				|
		1'b0;

	//=====================================

	wire bus_valid_select =
		rom_select				|
		ram_select				|
`ifdef SOC_ENABLE_SDRAM		
		sdram_select			|
`endif		
`ifdef SOC_ENABLE_VGA		
		vram_select				|
`endif		
		led_select				|
`ifdef SOC_ENABLE_UART		
		uart_0_select			|
		uart_1_select			|
`endif		
`ifdef SOC_ENABLE_GPIO		
		gpio_select				|
`endif		
`ifdef SOC_ENABLE_SD		
		sd_select				|
`endif		
`ifdef SOC_ENABLE_I2C		
		i2c_select				|
`endif
`ifdef SOC_ENABLE_AUDIO
		audio_select			|
`endif
		dma_select				|
		timer_select			|
		plic_select;

	reg debug_bus_fault = 0;
	reg [31:0] debug_bus_fault_address = 0;
	reg [1:0] debug_bus_fault_type = 0;

	reg [`TAG_SIZE] execute_debug_tag = 0;
	reg [`TAG_SIZE] writeback_debug_tag = 0;

	reg debug_request = 0;
	reg [13:0] debug_select = 0;

	always_comb begin
		debug_bus_fault = !bus_valid_select && bus_request;
		debug_bus_fault_address = bus_address;
		debug_bus_fault_type =
			cpu_ibus_request ? 2'd1 :
			cpu_dbus_request ? 2'd2 :
			dma_bus_request ? 2'd3 :
			2'd0;

		execute_debug_tag = cpu_execute_debug_tag;
		writeback_debug_tag = cpu_writeback_debug_tag;

		debug_request = bus_request;
		debug_select =
		{
			rom_select,
			ram_select,
			sdram_select,
			vram_select,
			led_select,
			uart_0_select,
			uart_1_select,
			1'b0, // gpio_select,
			sd_select,
			i2c_select,
			dma_select,
			timer_select,
			plic_select,
			gpu_select
		};
	end

endmodule
