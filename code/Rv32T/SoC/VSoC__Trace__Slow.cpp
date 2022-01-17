// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "VSoC__Syms.h"


//======================

void VSoC::trace(VerilatedVcdC* tfp, int, int) {
    tfp->spTrace()->addInitCb(&traceInit, __VlSymsp);
    traceRegister(tfp->spTrace());
}

void VSoC::traceInit(void* userp, VerilatedVcd* tracep, uint32_t code) {
    // Callback from tracep->open()
    VSoC__Syms* __restrict vlSymsp = static_cast<VSoC__Syms*>(userp);
    if (!Verilated::calcUnusedSigs()) {
        VL_FATAL_MT(__FILE__, __LINE__, __FILE__,
                        "Turning on wave traces requires Verilated::traceEverOn(true) call before time 0.");
    }
    vlSymsp->__Vm_baseCode = code;
    tracep->module(vlSymsp->name());
    tracep->scopeEscape(' ');
    VSoC::traceInitTop(vlSymsp, tracep);
    tracep->scopeEscape('.');
}

//======================


void VSoC::traceInitTop(void* userp, VerilatedVcd* tracep) {
    VSoC__Syms* __restrict vlSymsp = static_cast<VSoC__Syms*>(userp);
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    {
        vlTOPp->traceInitSub0(userp, tracep);
    }
}

void VSoC::traceInitSub0(void* userp, VerilatedVcd* tracep) {
    VSoC__Syms* __restrict vlSymsp = static_cast<VSoC__Syms*>(userp);
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    const int c = vlSymsp->__Vm_baseCode;
    if (false && tracep && c) {}  // Prevent unused
    // Body
    {
        tracep->declBit(c+443,"ADC_CONVST", false,-1);
        tracep->declBit(c+444,"ADC_SCK", false,-1);
        tracep->declBit(c+445,"ADC_SDI", false,-1);
        tracep->declBit(c+446,"ADC_SDO", false,-1);
        tracep->declBit(c+447,"AUD_ADCDAT", false,-1);
        tracep->declBit(c+448,"AUD_ADCLRCK", false,-1);
        tracep->declBit(c+449,"AUD_BCLK", false,-1);
        tracep->declBit(c+450,"AUD_DACDAT", false,-1);
        tracep->declBit(c+451,"AUD_DACLRCK", false,-1);
        tracep->declBit(c+452,"AUD_XCK", false,-1);
        tracep->declBit(c+453,"CLOCK_125_p", false,-1);
        tracep->declBit(c+454,"CLOCK_50_B5B", false,-1);
        tracep->declBit(c+455,"CLOCK_50_B6A", false,-1);
        tracep->declBit(c+456,"CLOCK_50_B7A", false,-1);
        tracep->declBit(c+457,"CLOCK_50_B8A", false,-1);
        tracep->declBit(c+458,"CPU_RESET_n", false,-1);
        tracep->declBus(c+459,"DDR2LP_CA", false,-1, 9,0);
        tracep->declBus(c+460,"DDR2LP_CKE", false,-1, 1,0);
        tracep->declBit(c+461,"DDR2LP_CK_n", false,-1);
        tracep->declBit(c+462,"DDR2LP_CK_p", false,-1);
        tracep->declBus(c+463,"DDR2LP_CS_n", false,-1, 1,0);
        tracep->declBus(c+464,"DDR2LP_DM", false,-1, 3,0);
        tracep->declBus(c+465,"DDR2LP_DQ", false,-1, 31,0);
        tracep->declBus(c+466,"DDR2LP_DQS_n", false,-1, 3,0);
        tracep->declBus(c+467,"DDR2LP_DQS_p", false,-1, 3,0);
        tracep->declBit(c+468,"DDR2LP_OCT_RZQ", false,-1);
        tracep->declQuad(c+469,"GPIO", false,-1, 35,0);
        tracep->declBit(c+471,"HDMI_TX_CLK", false,-1);
        tracep->declBus(c+472,"HDMI_TX_D", false,-1, 23,0);
        tracep->declBit(c+473,"HDMI_TX_DE", false,-1);
        tracep->declBit(c+474,"HDMI_TX_HS", false,-1);
        tracep->declBit(c+475,"HDMI_TX_INT", false,-1);
        tracep->declBit(c+476,"HDMI_TX_VS", false,-1);
        tracep->declBus(c+477,"HEX0", false,-1, 6,0);
        tracep->declBus(c+478,"HEX1", false,-1, 6,0);
        tracep->declBit(c+479,"HSMC_CLKIN0", false,-1);
        tracep->declBus(c+480,"HSMC_CLKIN_n", false,-1, 2,1);
        tracep->declBus(c+481,"HSMC_CLKIN_p", false,-1, 2,1);
        tracep->declBit(c+482,"HSMC_CLKOUT0", false,-1);
        tracep->declBus(c+483,"HSMC_CLKOUT_n", false,-1, 2,1);
        tracep->declBus(c+484,"HSMC_CLKOUT_p", false,-1, 2,1);
        tracep->declBus(c+485,"HSMC_D", false,-1, 3,0);
        tracep->declBus(c+486,"HSMC_RX_n", false,-1, 16,0);
        tracep->declBus(c+487,"HSMC_RX_p", false,-1, 16,0);
        tracep->declBus(c+488,"HSMC_TX_n", false,-1, 16,0);
        tracep->declBus(c+489,"HSMC_TX_p", false,-1, 16,0);
        tracep->declBit(c+490,"I2C_SCL", false,-1);
        tracep->declBit(c+491,"I2C_SDA", false,-1);
        tracep->declBus(c+492,"KEY", false,-1, 3,0);
        tracep->declBus(c+493,"LEDG", false,-1, 7,0);
        tracep->declBus(c+494,"LEDR", false,-1, 9,0);
        tracep->declBit(c+495,"SD_CLK", false,-1);
        tracep->declBit(c+496,"SD_CMD", false,-1);
        tracep->declBus(c+497,"SD_DAT", false,-1, 3,0);
        tracep->declBus(c+498,"SRAM_A", false,-1, 17,0);
        tracep->declBit(c+499,"SRAM_CE_n", false,-1);
        tracep->declBus(c+500,"SRAM_D", false,-1, 15,0);
        tracep->declBit(c+501,"SRAM_LB_n", false,-1);
        tracep->declBit(c+502,"SRAM_OE_n", false,-1);
        tracep->declBit(c+503,"SRAM_UB_n", false,-1);
        tracep->declBit(c+504,"SRAM_WE_n", false,-1);
        tracep->declBus(c+505,"SW", false,-1, 9,0);
        tracep->declBit(c+506,"UART_RX", false,-1);
        tracep->declBit(c+507,"UART_TX", false,-1);
        tracep->declBit(c+443,"SoC ADC_CONVST", false,-1);
        tracep->declBit(c+444,"SoC ADC_SCK", false,-1);
        tracep->declBit(c+445,"SoC ADC_SDI", false,-1);
        tracep->declBit(c+446,"SoC ADC_SDO", false,-1);
        tracep->declBit(c+447,"SoC AUD_ADCDAT", false,-1);
        tracep->declBit(c+448,"SoC AUD_ADCLRCK", false,-1);
        tracep->declBit(c+449,"SoC AUD_BCLK", false,-1);
        tracep->declBit(c+450,"SoC AUD_DACDAT", false,-1);
        tracep->declBit(c+451,"SoC AUD_DACLRCK", false,-1);
        tracep->declBit(c+452,"SoC AUD_XCK", false,-1);
        tracep->declBit(c+453,"SoC CLOCK_125_p", false,-1);
        tracep->declBit(c+454,"SoC CLOCK_50_B5B", false,-1);
        tracep->declBit(c+455,"SoC CLOCK_50_B6A", false,-1);
        tracep->declBit(c+456,"SoC CLOCK_50_B7A", false,-1);
        tracep->declBit(c+457,"SoC CLOCK_50_B8A", false,-1);
        tracep->declBit(c+458,"SoC CPU_RESET_n", false,-1);
        tracep->declBus(c+459,"SoC DDR2LP_CA", false,-1, 9,0);
        tracep->declBus(c+460,"SoC DDR2LP_CKE", false,-1, 1,0);
        tracep->declBit(c+461,"SoC DDR2LP_CK_n", false,-1);
        tracep->declBit(c+462,"SoC DDR2LP_CK_p", false,-1);
        tracep->declBus(c+463,"SoC DDR2LP_CS_n", false,-1, 1,0);
        tracep->declBus(c+464,"SoC DDR2LP_DM", false,-1, 3,0);
        tracep->declBus(c+465,"SoC DDR2LP_DQ", false,-1, 31,0);
        tracep->declBus(c+466,"SoC DDR2LP_DQS_n", false,-1, 3,0);
        tracep->declBus(c+467,"SoC DDR2LP_DQS_p", false,-1, 3,0);
        tracep->declBit(c+468,"SoC DDR2LP_OCT_RZQ", false,-1);
        tracep->declQuad(c+469,"SoC GPIO", false,-1, 35,0);
        tracep->declBit(c+471,"SoC HDMI_TX_CLK", false,-1);
        tracep->declBus(c+472,"SoC HDMI_TX_D", false,-1, 23,0);
        tracep->declBit(c+473,"SoC HDMI_TX_DE", false,-1);
        tracep->declBit(c+474,"SoC HDMI_TX_HS", false,-1);
        tracep->declBit(c+475,"SoC HDMI_TX_INT", false,-1);
        tracep->declBit(c+476,"SoC HDMI_TX_VS", false,-1);
        tracep->declBus(c+477,"SoC HEX0", false,-1, 6,0);
        tracep->declBus(c+478,"SoC HEX1", false,-1, 6,0);
        tracep->declBit(c+479,"SoC HSMC_CLKIN0", false,-1);
        tracep->declBus(c+480,"SoC HSMC_CLKIN_n", false,-1, 2,1);
        tracep->declBus(c+481,"SoC HSMC_CLKIN_p", false,-1, 2,1);
        tracep->declBit(c+482,"SoC HSMC_CLKOUT0", false,-1);
        tracep->declBus(c+483,"SoC HSMC_CLKOUT_n", false,-1, 2,1);
        tracep->declBus(c+484,"SoC HSMC_CLKOUT_p", false,-1, 2,1);
        tracep->declBus(c+485,"SoC HSMC_D", false,-1, 3,0);
        tracep->declBus(c+486,"SoC HSMC_RX_n", false,-1, 16,0);
        tracep->declBus(c+487,"SoC HSMC_RX_p", false,-1, 16,0);
        tracep->declBus(c+488,"SoC HSMC_TX_n", false,-1, 16,0);
        tracep->declBus(c+489,"SoC HSMC_TX_p", false,-1, 16,0);
        tracep->declBit(c+490,"SoC I2C_SCL", false,-1);
        tracep->declBit(c+491,"SoC I2C_SDA", false,-1);
        tracep->declBus(c+492,"SoC KEY", false,-1, 3,0);
        tracep->declBus(c+493,"SoC LEDG", false,-1, 7,0);
        tracep->declBus(c+494,"SoC LEDR", false,-1, 9,0);
        tracep->declBit(c+495,"SoC SD_CLK", false,-1);
        tracep->declBit(c+496,"SoC SD_CMD", false,-1);
        tracep->declBus(c+497,"SoC SD_DAT", false,-1, 3,0);
        tracep->declBus(c+498,"SoC SRAM_A", false,-1, 17,0);
        tracep->declBit(c+499,"SoC SRAM_CE_n", false,-1);
        tracep->declBus(c+500,"SoC SRAM_D", false,-1, 15,0);
        tracep->declBit(c+501,"SoC SRAM_LB_n", false,-1);
        tracep->declBit(c+502,"SoC SRAM_OE_n", false,-1);
        tracep->declBit(c+503,"SoC SRAM_UB_n", false,-1);
        tracep->declBit(c+504,"SoC SRAM_WE_n", false,-1);
        tracep->declBus(c+505,"SoC SW", false,-1, 9,0);
        tracep->declBit(c+506,"SoC UART_RX", false,-1);
        tracep->declBit(c+507,"SoC UART_TX", false,-1);
        tracep->declBit(c+453,"SoC clock", false,-1);
        tracep->declBit(c+508,"SoC reset", false,-1);
        tracep->declBit(c+1,"SoC video_sram_request", false,-1);
        tracep->declBit(c+2,"SoC video_sram_rw", false,-1);
        tracep->declBus(c+3,"SoC video_sram_address", false,-1, 31,0);
        tracep->declBus(c+4,"SoC video_sram_wdata", false,-1, 31,0);
        tracep->declBus(c+5,"SoC video_sram_rdata", false,-1, 31,0);
        tracep->declBit(c+6,"SoC video_sram_ready", false,-1);
        tracep->declBit(c+142,"SoC vram_select", false,-1);
        tracep->declBus(c+143,"SoC vram_address", false,-1, 31,0);
        tracep->declBit(c+7,"SoC vram_ready", false,-1);
        tracep->declBit(c+8,"SoC vga_enable", false,-1);
        tracep->declBus(c+9,"SoC vga_address", false,-1, 15,0);
        tracep->declBit(c+144,"SoC rom_select", false,-1);
        tracep->declBus(c+145,"SoC rom_address", false,-1, 31,0);
        tracep->declBus(c+10,"SoC rom_rdata", false,-1, 31,0);
        tracep->declBit(c+11,"SoC rom_ready", false,-1);
        tracep->declBit(c+146,"SoC ram_select", false,-1);
        tracep->declBus(c+147,"SoC ram_address", false,-1, 31,0);
        tracep->declBus(c+12,"SoC ram_rdata", false,-1, 31,0);
        tracep->declBit(c+13,"SoC ram_ready", false,-1);
        tracep->declBit(c+148,"SoC sdram_select", false,-1);
        tracep->declBus(c+149,"SoC sdram_address", false,-1, 31,0);
        tracep->declBus(c+14,"SoC sdram_rdata", false,-1, 31,0);
        tracep->declBit(c+15,"SoC sdram_ready", false,-1);
        tracep->declBit(c+150,"SoC led_select", false,-1);
        tracep->declBit(c+16,"SoC led_ready", false,-1);
        tracep->declBit(c+151,"SoC uart_select", false,-1);
        tracep->declBus(c+152,"SoC uart_address", false,-1, 1,0);
        tracep->declBus(c+17,"SoC uart_rdata", false,-1, 31,0);
        tracep->declBit(c+509,"SoC uart_ready", false,-1);
        tracep->declBit(c+153,"SoC i2c_select", false,-1);
        tracep->declBus(c+18,"SoC i2c_rdata", false,-1, 31,0);
        tracep->declBit(c+19,"SoC i2c_ready", false,-1);
        tracep->declBit(c+154,"SoC sd_select", false,-1);
        tracep->declBus(c+20,"SoC sd_rdata", false,-1, 31,0);
        tracep->declBit(c+21,"SoC sd_ready", false,-1);
        tracep->declBit(c+155,"SoC timer_select", false,-1);
        tracep->declBus(c+152,"SoC timer_address", false,-1, 1,0);
        tracep->declBus(c+22,"SoC timer_rdata", false,-1, 31,0);
        tracep->declBit(c+512,"SoC timer_ready", false,-1);
        tracep->declBit(c+156,"SoC cpu_rw", false,-1);
        tracep->declBit(c+157,"SoC cpu_request", false,-1);
        tracep->declBit(c+158,"SoC cpu_ready", false,-1);
        tracep->declBus(c+145,"SoC cpu_address", false,-1, 31,0);
        tracep->declBus(c+159,"SoC cpu_rdata", false,-1, 31,0);
        tracep->declBus(c+160,"SoC cpu_wdata", false,-1, 31,0);
        tracep->declBus(c+23,"SoC cpu_retire_count", false,-1, 31,0);
        tracep->declBus(c+513,"SoC video_sram WIDTH", false,-1, 31,0);
        tracep->declBus(c+514,"SoC video_sram SIZE", false,-1, 31,0);
        tracep->declBus(c+515,"SoC video_sram ADDR_LSH", false,-1, 31,0);
        tracep->declBit(c+453,"SoC video_sram i_clock", false,-1);
        tracep->declBit(c+1,"SoC video_sram i_request", false,-1);
        tracep->declBit(c+2,"SoC video_sram i_rw", false,-1);
        tracep->declBus(c+3,"SoC video_sram i_address", false,-1, 31,0);
        tracep->declBus(c+4,"SoC video_sram i_wdata", false,-1, 31,0);
        tracep->declBus(c+5,"SoC video_sram o_rdata", false,-1, 31,0);
        tracep->declBit(c+6,"SoC video_sram o_ready", false,-1);
        tracep->declBit(c+453,"SoC video_bus i_clock", false,-1);
        tracep->declBit(c+161,"SoC video_bus i_cpu_request", false,-1);
        tracep->declBus(c+143,"SoC video_bus i_cpu_address", false,-1, 31,0);
        tracep->declBus(c+160,"SoC video_bus i_cpu_wdata", false,-1, 31,0);
        tracep->declBit(c+7,"SoC video_bus o_cpu_ready", false,-1);
        tracep->declBit(c+8,"SoC video_bus i_video_request", false,-1);
        tracep->declBus(c+9,"SoC video_bus i_video_address", false,-1, 15,0);
        tracep->declBus(c+24,"SoC video_bus o_video_rdata", false,-1, 31,0);
        tracep->declBit(c+1,"SoC video_bus o_mem_request", false,-1);
        tracep->declBit(c+2,"SoC video_bus o_mem_rw", false,-1);
        tracep->declBus(c+3,"SoC video_bus o_mem_address", false,-1, 31,0);
        tracep->declBus(c+4,"SoC video_bus o_mem_wdata", false,-1, 31,0);
        tracep->declBus(c+5,"SoC video_bus i_mem_rdata", false,-1, 31,0);
        tracep->declBit(c+6,"SoC video_bus i_mem_ready", false,-1);
        tracep->declBit(c+25,"SoC video_bus fifo_empty", false,-1);
        tracep->declBit(c+26,"SoC video_bus fifo_full", false,-1);
        tracep->declBit(c+27,"SoC video_bus fifo_write", false,-1);
        tracep->declQuad(c+28,"SoC video_bus fifo_wdata", false,-1, 63,0);
        tracep->declBit(c+30,"SoC video_bus fifo_read", false,-1);
        tracep->declQuad(c+31,"SoC video_bus fifo_rdata", false,-1, 63,0);
        tracep->declBus(c+33,"SoC video_bus state", false,-1, 2,0);
        tracep->declBit(c+34,"SoC video_bus last_video_address", false,-1);
        tracep->declBus(c+516,"SoC video_bus fifo DEPTH", false,-1, 31,0);
        tracep->declBit(c+453,"SoC video_bus fifo i_clock", false,-1);
        tracep->declBit(c+25,"SoC video_bus fifo o_empty", false,-1);
        tracep->declBit(c+26,"SoC video_bus fifo o_full", false,-1);
        tracep->declBit(c+27,"SoC video_bus fifo i_write", false,-1);
        tracep->declQuad(c+28,"SoC video_bus fifo i_wdata", false,-1, 63,0);
        tracep->declBit(c+30,"SoC video_bus fifo i_read", false,-1);
        tracep->declQuad(c+31,"SoC video_bus fifo o_rdata", false,-1, 63,0);
        tracep->declQuad(c+31,"SoC video_bus fifo rdata", false,-1, 63,0);
        tracep->declBus(c+35,"SoC video_bus fifo in", false,-1, 9,0);
        tracep->declBus(c+36,"SoC video_bus fifo out", false,-1, 9,0);
        tracep->declBit(c+37,"SoC video_bus fifo last_write", false,-1);
        tracep->declBit(c+38,"SoC video_bus fifo last_read", false,-1);
        tracep->declBus(c+517,"SoC vga PRESCALE", false,-1, 31,0);
        tracep->declBit(c+453,"SoC vga i_clock", false,-1);
        tracep->declBit(c+474,"SoC vga o_hsync", false,-1);
        tracep->declBit(c+476,"SoC vga o_vsync", false,-1);
        tracep->declBit(c+8,"SoC vga o_data_enable", false,-1);
        tracep->declBus(c+9,"SoC vga o_vga_address", false,-1, 15,0);
        tracep->declBit(c+471,"SoC vga o_vga_clock", false,-1);
        tracep->declBus(c+39,"SoC vga prescale", false,-1, 3,0);
        tracep->declBus(c+40,"SoC vga vga_h", false,-1, 9,0);
        tracep->declBus(c+41,"SoC vga vga_v", false,-1, 9,0);
        tracep->declBit(c+453,"SoC rom i_clock", false,-1);
        tracep->declBit(c+162,"SoC rom i_request", false,-1);
        tracep->declBus(c+145,"SoC rom i_address", false,-1, 31,0);
        tracep->declBus(c+10,"SoC rom o_rdata", false,-1, 31,0);
        tracep->declBit(c+11,"SoC rom o_ready", false,-1);
        tracep->declBus(c+513,"SoC ram WIDTH", false,-1, 31,0);
        tracep->declBus(c+518,"SoC ram SIZE", false,-1, 31,0);
        tracep->declBus(c+515,"SoC ram ADDR_LSH", false,-1, 31,0);
        tracep->declBit(c+453,"SoC ram i_clock", false,-1);
        tracep->declBit(c+163,"SoC ram i_request", false,-1);
        tracep->declBit(c+156,"SoC ram i_rw", false,-1);
        tracep->declBus(c+147,"SoC ram i_address", false,-1, 31,0);
        tracep->declBus(c+160,"SoC ram i_wdata", false,-1, 31,0);
        tracep->declBus(c+12,"SoC ram o_rdata", false,-1, 31,0);
        tracep->declBit(c+13,"SoC ram o_ready", false,-1);
        tracep->declBus(c+513,"SoC sdram WIDTH", false,-1, 31,0);
        tracep->declBus(c+519,"SoC sdram SIZE", false,-1, 31,0);
        tracep->declBus(c+515,"SoC sdram ADDR_LSH", false,-1, 31,0);
        tracep->declBit(c+453,"SoC sdram i_clock", false,-1);
        tracep->declBit(c+164,"SoC sdram i_request", false,-1);
        tracep->declBit(c+156,"SoC sdram i_rw", false,-1);
        tracep->declBus(c+149,"SoC sdram i_address", false,-1, 31,0);
        tracep->declBus(c+160,"SoC sdram i_wdata", false,-1, 31,0);
        tracep->declBus(c+14,"SoC sdram o_rdata", false,-1, 31,0);
        tracep->declBit(c+15,"SoC sdram o_ready", false,-1);
        tracep->declBit(c+508,"SoC led i_reset", false,-1);
        tracep->declBit(c+453,"SoC led i_clock", false,-1);
        tracep->declBit(c+165,"SoC led i_request", false,-1);
        tracep->declBus(c+160,"SoC led i_wdata", false,-1, 31,0);
        tracep->declBit(c+16,"SoC led o_ready", false,-1);
        tracep->declBus(c+494,"SoC led LEDR", false,-1, 9,0);
        tracep->declBus(c+105,"SoC led leds", false,-1, 9,0);
        tracep->declBus(c+520,"SoC uart PRESCALE", false,-1, 31,0);
        tracep->declBit(c+508,"SoC uart i_reset", false,-1);
        tracep->declBit(c+453,"SoC uart i_clock", false,-1);
        tracep->declBit(c+166,"SoC uart i_request", false,-1);
        tracep->declBit(c+156,"SoC uart i_rw", false,-1);
        tracep->declBus(c+152,"SoC uart i_address", false,-1, 1,0);
        tracep->declBus(c+160,"SoC uart i_wdata", false,-1, 31,0);
        tracep->declBus(c+17,"SoC uart o_rdata", false,-1, 31,0);
        tracep->declBit(c+509,"SoC uart o_ready", false,-1);
        tracep->declBit(c+506,"SoC uart UART_RX", false,-1);
        tracep->declBit(c+507,"SoC uart UART_TX", false,-1);
        tracep->declBit(c+167,"SoC uart rx_request", false,-1);
        tracep->declBit(c+510,"SoC uart rx_ready", false,-1);
        tracep->declBit(c+168,"SoC uart tx_request", false,-1);
        tracep->declBit(c+42,"SoC uart tx_ready", false,-1);
        tracep->declBus(c+520,"SoC uart rx PRESCALE", false,-1, 31,0);
        tracep->declBit(c+508,"SoC uart rx i_reset", false,-1);
        tracep->declBit(c+453,"SoC uart rx i_clock", false,-1);
        tracep->declBit(c+167,"SoC uart rx i_request", false,-1);
        tracep->declBus(c+152,"SoC uart rx i_address", false,-1, 1,0);
        tracep->declBus(c+17,"SoC uart rx o_rdata", false,-1, 31,0);
        tracep->declBit(c+510,"SoC uart rx o_ready", false,-1);
        tracep->declBit(c+506,"SoC uart rx UART_RX", false,-1);
        tracep->declBit(c+43,"SoC uart rx frame_error", false,-1);
        tracep->declBus(c+44,"SoC uart rx prescale", false,-1, 18,0);
        tracep->declBus(c+45,"SoC uart rx data", false,-1, 7,0);
        tracep->declBus(c+46,"SoC uart rx bidx", false,-1, 3,0);
        tracep->declBus(c+47,"SoC uart rx rds", false,-1, 3,0);
        tracep->declBit(c+48,"SoC uart rx rx", false,-1);
        tracep->declBit(c+49,"SoC uart rx rx_fifo_empty", false,-1);
        tracep->declBit(c+50,"SoC uart rx rx_fifo_write", false,-1);
        tracep->declBit(c+51,"SoC uart rx rx_fifo_read", false,-1);
        tracep->declBus(c+52,"SoC uart rx rx_fifo_rdata", false,-1, 7,0);
        tracep->declBus(c+517,"SoC uart rx rx_fifo DEPTH", false,-1, 31,0);
        tracep->declBit(c+453,"SoC uart rx rx_fifo i_clock", false,-1);
        tracep->declBit(c+49,"SoC uart rx rx_fifo o_empty", false,-1);
        tracep->declBit(c+53,"SoC uart rx rx_fifo o_full", false,-1);
        tracep->declBit(c+50,"SoC uart rx rx_fifo i_write", false,-1);
        tracep->declBus(c+45,"SoC uart rx rx_fifo i_wdata", false,-1, 7,0);
        tracep->declBit(c+51,"SoC uart rx rx_fifo i_read", false,-1);
        tracep->declBus(c+52,"SoC uart rx rx_fifo o_rdata", false,-1, 7,0);
        tracep->declBus(c+52,"SoC uart rx rx_fifo rdata", false,-1, 7,0);
        {int i; for (i=0; i<4; i++) {
                tracep->declBus(c+54+i*1,"SoC uart rx rx_fifo data", true,(i+0), 7,0);}}
        tracep->declBus(c+58,"SoC uart rx rx_fifo in", false,-1, 1,0);
        tracep->declBus(c+59,"SoC uart rx rx_fifo out", false,-1, 1,0);
        tracep->declBit(c+60,"SoC uart rx rx_fifo last_write", false,-1);
        tracep->declBit(c+61,"SoC uart rx rx_fifo last_read", false,-1);
        tracep->declBus(c+520,"SoC uart tx PRESCALE", false,-1, 31,0);
        tracep->declBit(c+508,"SoC uart tx i_reset", false,-1);
        tracep->declBit(c+453,"SoC uart tx i_clock", false,-1);
        tracep->declBit(c+168,"SoC uart tx i_request", false,-1);
        tracep->declBus(c+160,"SoC uart tx i_wdata", false,-1, 31,0);
        tracep->declBit(c+42,"SoC uart tx o_ready", false,-1);
        tracep->declBit(c+507,"SoC uart tx UART_TX", false,-1);
        tracep->declBus(c+62,"SoC uart tx prescale", false,-1, 18,0);
        tracep->declBus(c+63,"SoC uart tx data", false,-1, 8,0);
        tracep->declBus(c+64,"SoC uart tx bidx", false,-1, 3,0);
        tracep->declBit(c+453,"SoC i2c i_clock", false,-1);
        tracep->declBit(c+169,"SoC i2c i_request", false,-1);
        tracep->declBit(c+156,"SoC i2c i_rw", false,-1);
        tracep->declBus(c+160,"SoC i2c i_wdata", false,-1, 31,0);
        tracep->declBus(c+18,"SoC i2c o_rdata", false,-1, 31,0);
        tracep->declBit(c+19,"SoC i2c o_ready", false,-1);
        tracep->declBit(c+490,"SoC i2c I2C_SCL", false,-1);
        tracep->declBit(c+491,"SoC i2c I2C_SDA", false,-1);
        tracep->declBit(c+65,"SoC i2c scl", false,-1);
        tracep->declBit(c+66,"SoC i2c sda", false,-1);
        tracep->declBus(c+170,"SoC i2c mask", false,-1, 3,0);
        tracep->declBus(c+171,"SoC i2c value", false,-1, 3,0);
        tracep->declBit(c+508,"SoC sd i_reset", false,-1);
        tracep->declBit(c+453,"SoC sd i_clock", false,-1);
        tracep->declBit(c+172,"SoC sd i_request", false,-1);
        tracep->declBit(c+156,"SoC sd i_rw", false,-1);
        tracep->declBus(c+160,"SoC sd i_wdata", false,-1, 31,0);
        tracep->declBus(c+20,"SoC sd o_rdata", false,-1, 31,0);
        tracep->declBit(c+21,"SoC sd o_ready", false,-1);
        tracep->declBit(c+495,"SoC sd SD_CLK", false,-1);
        tracep->declBit(c+496,"SoC sd SD_CMD", false,-1);
        tracep->declBus(c+497,"SoC sd SD_DAT", false,-1, 3,0);
        tracep->declBus(c+521,"SoC sd DIR_IN", false,-1, 0,0);
        tracep->declBus(c+522,"SoC sd DIR_OUT", false,-1, 0,0);
        tracep->declBit(c+67,"SoC sd clk", false,-1);
        tracep->declBit(c+68,"SoC sd cdir", false,-1);
        tracep->declBit(c+69,"SoC sd ddir", false,-1);
        tracep->declBit(c+70,"SoC sd cmd", false,-1);
        tracep->declBus(c+71,"SoC sd dat", false,-1, 3,0);
        tracep->declBus(c+173,"SoC sd mask", false,-1, 7,0);
        tracep->declBus(c+174,"SoC sd write", false,-1, 7,0);
        tracep->declBit(c+496,"SoC sd cmd_in", false,-1);
        tracep->declBus(c+497,"SoC sd dat_in", false,-1, 3,0);
        tracep->declBus(c+523,"SoC timer FREQUENCY", false,-1, 31,0);
        tracep->declBit(c+508,"SoC timer i_reset", false,-1);
        tracep->declBit(c+453,"SoC timer i_clock", false,-1);
        tracep->declBit(c+175,"SoC timer i_request", false,-1);
        tracep->declBus(c+152,"SoC timer i_address", false,-1, 1,0);
        tracep->declBus(c+22,"SoC timer o_rdata", false,-1, 31,0);
        tracep->declBit(c+512,"SoC timer o_ready", false,-1);
        tracep->declBus(c+23,"SoC timer i_retire_count", false,-1, 31,0);
        tracep->declBus(c+524,"SoC timer PRESCALE", false,-1, 31,0);
        tracep->declBus(c+525,"SoC timer PRESCALE_WIDTH", false,-1, 31,0);
        tracep->declBus(c+72,"SoC timer prescale", false,-1, 16,0);
        tracep->declBus(c+73,"SoC timer cycles", false,-1, 31,0);
        tracep->declBus(c+74,"SoC timer ms", false,-1, 31,0);
        tracep->declBit(c+508,"SoC cpu i_reset", false,-1);
        tracep->declBit(c+453,"SoC cpu i_clock", false,-1);
        tracep->declBit(c+156,"SoC cpu o_bus_rw", false,-1);
        tracep->declBit(c+157,"SoC cpu o_bus_request", false,-1);
        tracep->declBit(c+158,"SoC cpu i_bus_ready", false,-1);
        tracep->declBus(c+145,"SoC cpu o_bus_address", false,-1, 31,0);
        tracep->declBus(c+159,"SoC cpu i_bus_rdata", false,-1, 31,0);
        tracep->declBus(c+160,"SoC cpu o_bus_wdata", false,-1, 31,0);
        tracep->declBus(c+23,"SoC cpu o_retire_count", false,-1, 31,0);
        tracep->declBit(c+176,"SoC cpu bus_busy", false,-1);
        tracep->declBit(c+177,"SoC cpu bus_pa_request", false,-1);
        tracep->declBit(c+178,"SoC cpu bus_pa_ready", false,-1);
        tracep->declBus(c+179,"SoC cpu bus_pa_address", false,-1, 31,0);
        tracep->declBus(c+159,"SoC cpu bus_pa_rdata", false,-1, 31,0);
        tracep->declBit(c+75,"SoC cpu bus_pb_rw", false,-1);
        tracep->declBit(c+180,"SoC cpu bus_pb_request", false,-1);
        tracep->declBit(c+181,"SoC cpu bus_pb_ready", false,-1);
        tracep->declBus(c+76,"SoC cpu bus_pb_address", false,-1, 31,0);
        tracep->declBus(c+159,"SoC cpu bus_pb_rdata", false,-1, 31,0);
        tracep->declBus(c+77,"SoC cpu bus_pb_wdata", false,-1, 31,0);
        tracep->declBus(c+106,"SoC cpu rs1", false,-1, 31,0);
        tracep->declBus(c+107,"SoC cpu rs2", false,-1, 31,0);
        tracep->declBus(c+182,"SoC cpu fetch_inst_rs1", false,-1, 4,0);
        tracep->declBus(c+183,"SoC cpu fetch_inst_rs2", false,-1, 4,0);
        tracep->declBus(c+184,"SoC cpu fetch_tag", false,-1, 3,0);
        tracep->declBus(c+185,"SoC cpu fetch_instruction", false,-1, 31,0);
        tracep->declBus(c+186,"SoC cpu fetch_pc", false,-1, 31,0);
        tracep->declBus(c+187,"SoC cpu decode_tag", false,-1, 3,0);
        tracep->declBus(c+188,"SoC cpu decode_instruction", false,-1, 31,0);
        tracep->declBus(c+189,"SoC cpu decode_pc", false,-1, 31,0);
        tracep->declBus(c+190,"SoC cpu decode_inst_rs1", false,-1, 4,0);
        tracep->declBus(c+191,"SoC cpu decode_inst_rs2", false,-1, 4,0);
        tracep->declBus(c+192,"SoC cpu decode_inst_rd", false,-1, 4,0);
        tracep->declBus(c+193,"SoC cpu decode_imm", false,-1, 31,0);
        tracep->declBit(c+194,"SoC cpu decode_arithmetic", false,-1);
        tracep->declBit(c+195,"SoC cpu decode_jump", false,-1);
        tracep->declBit(c+196,"SoC cpu decode_jump_conditional", false,-1);
        tracep->declBus(c+197,"SoC cpu decode_alu_operation", false,-1, 3,0);
        tracep->declBus(c+198,"SoC cpu decode_alu_operand1", false,-1, 2,0);
        tracep->declBus(c+199,"SoC cpu decode_alu_operand2", false,-1, 2,0);
        tracep->declBit(c+200,"SoC cpu decode_memory_read", false,-1);
        tracep->declBit(c+201,"SoC cpu decode_memory_write", false,-1);
        tracep->declBus(c+202,"SoC cpu decode_memory_width", false,-1, 2,0);
        tracep->declBit(c+203,"SoC cpu decode_memory_signed", false,-1);
        tracep->declBus(c+204,"SoC cpu decode_op", false,-1, 4,0);
        tracep->declBus(c+423,"SoC cpu fwd_rs1", false,-1, 31,0);
        tracep->declBus(c+424,"SoC cpu fwd_rs2", false,-1, 31,0);
        tracep->declBus(c+205,"SoC cpu execute_tag", false,-1, 3,0);
        tracep->declBus(c+206,"SoC cpu execute_inst_rd", false,-1, 4,0);
        tracep->declBus(c+207,"SoC cpu execute_rd", false,-1, 31,0);
        tracep->declBus(c+208,"SoC cpu execute_pc_next", false,-1, 31,0);
        tracep->declBit(c+209,"SoC cpu execute_mem_read", false,-1);
        tracep->declBit(c+210,"SoC cpu execute_mem_write", false,-1);
        tracep->declBus(c+211,"SoC cpu execute_mem_width", false,-1, 2,0);
        tracep->declBit(c+212,"SoC cpu execute_mem_signed", false,-1);
        tracep->declBus(c+213,"SoC cpu execute_mem_address", false,-1, 31,0);
        tracep->declBus(c+526,"SoC cpu execute_mem_wdata", false,-1, 31,0);
        tracep->declBit(c+214,"SoC cpu execute_stall", false,-1);
        tracep->declBus(c+215,"SoC cpu memory_tag", false,-1, 3,0);
        tracep->declBus(c+216,"SoC cpu memory_inst_rd", false,-1, 4,0);
        tracep->declBus(c+217,"SoC cpu memory_rd", false,-1, 31,0);
        tracep->declBus(c+218,"SoC cpu memory_pc_next", false,-1, 31,0);
        tracep->declBit(c+219,"SoC cpu memory_stall", false,-1);
        tracep->declBus(c+220,"SoC cpu writeback_tag", false,-1, 3,0);
        tracep->declBus(c+78,"SoC cpu writeback_inst_rd", false,-1, 4,0);
        tracep->declBus(c+79,"SoC cpu writeback_rd", false,-1, 31,0);
        tracep->declBus(c+221,"SoC cpu writeback_pc_next", false,-1, 31,0);
        tracep->declBus(c+80,"SoC cpu retire_tag", false,-1, 3,0);
        tracep->declBit(c+508,"SoC cpu bus i_reset", false,-1);
        tracep->declBit(c+453,"SoC cpu bus i_clock", false,-1);
        tracep->declBit(c+176,"SoC cpu bus o_bus_busy", false,-1);
        tracep->declBit(c+156,"SoC cpu bus o_bus_rw", false,-1);
        tracep->declBit(c+157,"SoC cpu bus o_bus_request", false,-1);
        tracep->declBit(c+158,"SoC cpu bus i_bus_ready", false,-1);
        tracep->declBus(c+145,"SoC cpu bus o_bus_address", false,-1, 31,0);
        tracep->declBus(c+159,"SoC cpu bus i_bus_rdata", false,-1, 31,0);
        tracep->declBus(c+160,"SoC cpu bus o_bus_wdata", false,-1, 31,0);
        tracep->declBit(c+527,"SoC cpu bus i_pa_rw", false,-1);
        tracep->declBit(c+177,"SoC cpu bus i_pa_request", false,-1);
        tracep->declBit(c+178,"SoC cpu bus o_pa_ready", false,-1);
        tracep->declBus(c+179,"SoC cpu bus i_pa_address", false,-1, 31,0);
        tracep->declBus(c+159,"SoC cpu bus o_pa_rdata", false,-1, 31,0);
        tracep->declBus(c+528,"SoC cpu bus i_pa_wdata", false,-1, 31,0);
        tracep->declBit(c+75,"SoC cpu bus i_pb_rw", false,-1);
        tracep->declBit(c+180,"SoC cpu bus i_pb_request", false,-1);
        tracep->declBit(c+181,"SoC cpu bus o_pb_ready", false,-1);
        tracep->declBus(c+76,"SoC cpu bus i_pb_address", false,-1, 31,0);
        tracep->declBus(c+159,"SoC cpu bus o_pb_rdata", false,-1, 31,0);
        tracep->declBus(c+77,"SoC cpu bus i_pb_wdata", false,-1, 31,0);
        tracep->declBit(c+508,"SoC cpu registers i_reset", false,-1);
        tracep->declBit(c+453,"SoC cpu registers i_clock", false,-1);
        tracep->declBus(c+184,"SoC cpu registers i_read_tag", false,-1, 3,0);
        tracep->declBus(c+182,"SoC cpu registers i_read_rs1_idx", false,-1, 4,0);
        tracep->declBus(c+183,"SoC cpu registers i_read_rs2_idx", false,-1, 4,0);
        tracep->declBus(c+106,"SoC cpu registers o_rs1", false,-1, 31,0);
        tracep->declBus(c+107,"SoC cpu registers o_rs2", false,-1, 31,0);
        tracep->declBus(c+215,"SoC cpu registers i_write_tag", false,-1, 3,0);
        tracep->declBus(c+216,"SoC cpu registers i_write_rd_idx", false,-1, 4,0);
        tracep->declBus(c+217,"SoC cpu registers i_rd", false,-1, 31,0);
        tracep->declBus(c+108,"SoC cpu registers read_tag", false,-1, 3,0);
        tracep->declBus(c+109,"SoC cpu registers write_tag", false,-1, 3,0);
        {int i; for (i=0; i<32; i++) {
                tracep->declBus(c+110+i*1,"SoC cpu registers r", true,(i+0), 31,0);}}
        tracep->declBit(c+508,"SoC cpu fetch i_reset", false,-1);
        tracep->declBit(c+453,"SoC cpu fetch i_clock", false,-1);
        tracep->declBit(c+222,"SoC cpu fetch i_stall", false,-1);
        tracep->declBit(c+176,"SoC cpu fetch i_bus_busy", false,-1);
        tracep->declBit(c+177,"SoC cpu fetch o_bus_request", false,-1);
        tracep->declBit(c+178,"SoC cpu fetch i_bus_ready", false,-1);
        tracep->declBus(c+179,"SoC cpu fetch o_bus_address", false,-1, 31,0);
        tracep->declBus(c+159,"SoC cpu fetch i_bus_rdata", false,-1, 31,0);
        tracep->declBus(c+220,"SoC cpu fetch i_tag", false,-1, 3,0);
        tracep->declBus(c+221,"SoC cpu fetch i_pc_next", false,-1, 31,0);
        tracep->declBus(c+184,"SoC cpu fetch o_tag", false,-1, 3,0);
        tracep->declBus(c+185,"SoC cpu fetch o_instruction", false,-1, 31,0);
        tracep->declBus(c+186,"SoC cpu fetch o_pc", false,-1, 31,0);
        tracep->declBus(c+223,"SoC cpu fetch state", false,-1, 2,0);
        tracep->declBus(c+224,"SoC cpu fetch pc", false,-1, 31,0);
        tracep->declBus(c+225,"SoC cpu fetch icache_output_pc", false,-1, 31,0);
        tracep->declBus(c+226,"SoC cpu fetch icache_rdata", false,-1, 31,0);
        tracep->declBit(c+227,"SoC cpu fetch is_ADD", false,-1);
        tracep->declBit(c+228,"SoC cpu fetch is_ADDI", false,-1);
        tracep->declBit(c+229,"SoC cpu fetch is_AND", false,-1);
        tracep->declBit(c+230,"SoC cpu fetch is_ANDI", false,-1);
        tracep->declBit(c+231,"SoC cpu fetch is_AUIPC", false,-1);
        tracep->declBit(c+232,"SoC cpu fetch is_BEQ", false,-1);
        tracep->declBit(c+233,"SoC cpu fetch is_BGE", false,-1);
        tracep->declBit(c+234,"SoC cpu fetch is_BGEU", false,-1);
        tracep->declBit(c+235,"SoC cpu fetch is_BLT", false,-1);
        tracep->declBit(c+236,"SoC cpu fetch is_BLTU", false,-1);
        tracep->declBit(c+237,"SoC cpu fetch is_BNE", false,-1);
        tracep->declBit(c+238,"SoC cpu fetch is_DIV", false,-1);
        tracep->declBit(c+239,"SoC cpu fetch is_DIVU", false,-1);
        tracep->declBit(c+240,"SoC cpu fetch is_JAL", false,-1);
        tracep->declBit(c+241,"SoC cpu fetch is_JALR", false,-1);
        tracep->declBit(c+242,"SoC cpu fetch is_LB", false,-1);
        tracep->declBit(c+243,"SoC cpu fetch is_LBU", false,-1);
        tracep->declBit(c+244,"SoC cpu fetch is_LH", false,-1);
        tracep->declBit(c+245,"SoC cpu fetch is_LHU", false,-1);
        tracep->declBit(c+246,"SoC cpu fetch is_LUI", false,-1);
        tracep->declBit(c+247,"SoC cpu fetch is_LW", false,-1);
        tracep->declBit(c+248,"SoC cpu fetch is_MUL", false,-1);
        tracep->declBit(c+249,"SoC cpu fetch is_MULH", false,-1);
        tracep->declBit(c+250,"SoC cpu fetch is_MULHU", false,-1);
        tracep->declBit(c+251,"SoC cpu fetch is_OR", false,-1);
        tracep->declBit(c+252,"SoC cpu fetch is_ORI", false,-1);
        tracep->declBit(c+253,"SoC cpu fetch is_REM", false,-1);
        tracep->declBit(c+254,"SoC cpu fetch is_REMU", false,-1);
        tracep->declBit(c+255,"SoC cpu fetch is_SB", false,-1);
        tracep->declBit(c+256,"SoC cpu fetch is_SH", false,-1);
        tracep->declBit(c+257,"SoC cpu fetch is_SLL", false,-1);
        tracep->declBit(c+258,"SoC cpu fetch is_SLLI", false,-1);
        tracep->declBit(c+259,"SoC cpu fetch is_SLT", false,-1);
        tracep->declBit(c+260,"SoC cpu fetch is_SLTI", false,-1);
        tracep->declBit(c+261,"SoC cpu fetch is_SLTIU", false,-1);
        tracep->declBit(c+262,"SoC cpu fetch is_SLTU", false,-1);
        tracep->declBit(c+263,"SoC cpu fetch is_SRA", false,-1);
        tracep->declBit(c+264,"SoC cpu fetch is_SRAI", false,-1);
        tracep->declBit(c+265,"SoC cpu fetch is_SRL", false,-1);
        tracep->declBit(c+266,"SoC cpu fetch is_SRLI", false,-1);
        tracep->declBit(c+267,"SoC cpu fetch is_SUB", false,-1);
        tracep->declBit(c+268,"SoC cpu fetch is_SW", false,-1);
        tracep->declBit(c+269,"SoC cpu fetch is_XOR", false,-1);
        tracep->declBit(c+270,"SoC cpu fetch is_XORI", false,-1);
        tracep->declBit(c+271,"SoC cpu fetch is_B", false,-1);
        tracep->declBit(c+272,"SoC cpu fetch is_I", false,-1);
        tracep->declBit(c+240,"SoC cpu fetch is_J", false,-1);
        tracep->declBit(c+273,"SoC cpu fetch is_R", false,-1);
        tracep->declBit(c+274,"SoC cpu fetch is_S", false,-1);
        tracep->declBit(c+275,"SoC cpu fetch is_U", false,-1);
        tracep->declBit(c+271,"SoC cpu fetch is_JUMP_CONDITIONAL", false,-1);
        tracep->declBit(c+276,"SoC cpu fetch is_MEMORY", false,-1);
        tracep->declBit(c+277,"SoC cpu fetch is_JUMP", false,-1);
        tracep->declBit(c+278,"SoC cpu fetch is_ARITHMETIC", false,-1);
        tracep->declBit(c+279,"SoC cpu fetch is_COMPLEX", false,-1);
        tracep->declBit(c+508,"SoC cpu fetch icache i_reset", false,-1);
        tracep->declBit(c+453,"SoC cpu fetch icache i_clock", false,-1);
        tracep->declBus(c+224,"SoC cpu fetch icache i_input_pc", false,-1, 31,0);
        tracep->declBus(c+225,"SoC cpu fetch icache o_output_pc", false,-1, 31,0);
        tracep->declBus(c+226,"SoC cpu fetch icache o_rdata", false,-1, 31,0);
        tracep->declBit(c+176,"SoC cpu fetch icache i_bus_busy", false,-1);
        tracep->declBit(c+177,"SoC cpu fetch icache o_bus_request", false,-1);
        tracep->declBit(c+178,"SoC cpu fetch icache i_bus_ready", false,-1);
        tracep->declBus(c+179,"SoC cpu fetch icache o_bus_address", false,-1, 31,0);
        tracep->declBus(c+159,"SoC cpu fetch icache i_bus_rdata", false,-1, 31,0);
        tracep->declBus(c+529,"SoC cpu fetch icache SIZE", false,-1, 31,0);
        tracep->declBus(c+516,"SoC cpu fetch icache RANGE", false,-1, 31,0);
        tracep->declBus(c+280,"SoC cpu fetch icache state", false,-1, 3,0);
        tracep->declArray(c+281,"SoC cpu fetch icache valid", false,-1, 1023,0);
        tracep->declBit(c+313,"SoC cpu fetch icache cache_rw", false,-1);
        tracep->declBus(c+314,"SoC cpu fetch icache cache_label", false,-1, 9,0);
        tracep->declQuad(c+315,"SoC cpu fetch icache cache_wdata", false,-1, 63,0);
        tracep->declQuad(c+317,"SoC cpu fetch icache cache_rdata", false,-1, 63,0);
        tracep->declBus(c+319,"SoC cpu fetch icache prefetch_input_address", false,-1, 31,0);
        tracep->declBus(c+320,"SoC cpu fetch icache prefetch_output_address", false,-1, 31,0);
        tracep->declBus(c+321,"SoC cpu fetch icache prefetch_rdata", false,-1, 31,0);
        tracep->declBus(c+530,"SoC cpu fetch icache cache WIDTH", false,-1, 31,0);
        tracep->declBus(c+516,"SoC cpu fetch icache cache SIZE", false,-1, 31,0);
        tracep->declBus(c+531,"SoC cpu fetch icache cache ADDR_LSH", false,-1, 31,0);
        tracep->declBit(c+453,"SoC cpu fetch icache cache i_clock", false,-1);
        tracep->declBit(c+512,"SoC cpu fetch icache cache i_request", false,-1);
        tracep->declBit(c+313,"SoC cpu fetch icache cache i_rw", false,-1);
        tracep->declBus(c+322,"SoC cpu fetch icache cache i_address", false,-1, 31,0);
        tracep->declQuad(c+315,"SoC cpu fetch icache cache i_wdata", false,-1, 63,0);
        tracep->declQuad(c+317,"SoC cpu fetch icache cache o_rdata", false,-1, 63,0);
        tracep->declBit(c+81,"SoC cpu fetch icache cache o_ready", false,-1);
        tracep->declBit(c+508,"SoC cpu fetch icache prefetch i_reset", false,-1);
        tracep->declBit(c+453,"SoC cpu fetch icache prefetch i_clock", false,-1);
        tracep->declBus(c+319,"SoC cpu fetch icache prefetch i_address", false,-1, 31,0);
        tracep->declBus(c+320,"SoC cpu fetch icache prefetch o_address", false,-1, 31,0);
        tracep->declBus(c+321,"SoC cpu fetch icache prefetch o_rdata", false,-1, 31,0);
        tracep->declBit(c+176,"SoC cpu fetch icache prefetch i_bus_busy", false,-1);
        tracep->declBit(c+177,"SoC cpu fetch icache prefetch o_bus_request", false,-1);
        tracep->declBit(c+178,"SoC cpu fetch icache prefetch i_bus_ready", false,-1);
        tracep->declBus(c+179,"SoC cpu fetch icache prefetch o_bus_address", false,-1, 31,0);
        tracep->declBus(c+159,"SoC cpu fetch icache prefetch i_bus_rdata", false,-1, 31,0);
        tracep->declBus(c+323,"SoC cpu fetch icache prefetch state", false,-1, 3,0);
        tracep->declBus(c+320,"SoC cpu fetch icache prefetch address", false,-1, 31,0);
        tracep->declBus(c+321,"SoC cpu fetch icache prefetch prefetch", false,-1, 31,0);
        tracep->declBit(c+508,"SoC cpu decode i_reset", false,-1);
        tracep->declBit(c+453,"SoC cpu decode i_clock", false,-1);
        tracep->declBit(c+222,"SoC cpu decode i_stall", false,-1);
        tracep->declBus(c+184,"SoC cpu decode i_tag", false,-1, 3,0);
        tracep->declBus(c+185,"SoC cpu decode i_instruction", false,-1, 31,0);
        tracep->declBus(c+186,"SoC cpu decode i_pc", false,-1, 31,0);
        tracep->declBus(c+187,"SoC cpu decode o_tag", false,-1, 3,0);
        tracep->declBus(c+188,"SoC cpu decode o_instruction", false,-1, 31,0);
        tracep->declBus(c+189,"SoC cpu decode o_pc", false,-1, 31,0);
        tracep->declBus(c+190,"SoC cpu decode o_inst_rs1", false,-1, 4,0);
        tracep->declBus(c+191,"SoC cpu decode o_inst_rs2", false,-1, 4,0);
        tracep->declBus(c+192,"SoC cpu decode o_inst_rd", false,-1, 4,0);
        tracep->declBus(c+193,"SoC cpu decode o_imm", false,-1, 31,0);
        tracep->declBit(c+194,"SoC cpu decode o_arithmetic", false,-1);
        tracep->declBit(c+195,"SoC cpu decode o_jump", false,-1);
        tracep->declBit(c+196,"SoC cpu decode o_jump_conditional", false,-1);
        tracep->declBus(c+197,"SoC cpu decode o_alu_operation", false,-1, 3,0);
        tracep->declBus(c+198,"SoC cpu decode o_alu_operand1", false,-1, 2,0);
        tracep->declBus(c+199,"SoC cpu decode o_alu_operand2", false,-1, 2,0);
        tracep->declBit(c+200,"SoC cpu decode o_memory_read", false,-1);
        tracep->declBit(c+201,"SoC cpu decode o_memory_write", false,-1);
        tracep->declBus(c+202,"SoC cpu decode o_memory_width", false,-1, 2,0);
        tracep->declBit(c+203,"SoC cpu decode o_memory_signed", false,-1);
        tracep->declBus(c+204,"SoC cpu decode o_op", false,-1, 4,0);
        tracep->declBus(c+532,"SoC cpu decode OP_DIV", false,-1, 2,0);
        tracep->declBus(c+533,"SoC cpu decode OP_DIVU", false,-1, 2,0);
        tracep->declBus(c+534,"SoC cpu decode OP_MUL", false,-1, 2,0);
        tracep->declBus(c+535,"SoC cpu decode OP_MULH", false,-1, 2,0);
        tracep->declBus(c+536,"SoC cpu decode OP_MULHU", false,-1, 2,0);
        tracep->declBus(c+537,"SoC cpu decode OP_REM", false,-1, 2,0);
        tracep->declBus(c+538,"SoC cpu decode OP_REMU", false,-1, 2,0);
        tracep->declBit(c+324,"SoC cpu decode is_ADD", false,-1);
        tracep->declBit(c+325,"SoC cpu decode is_ADDI", false,-1);
        tracep->declBit(c+326,"SoC cpu decode is_AND", false,-1);
        tracep->declBit(c+327,"SoC cpu decode is_ANDI", false,-1);
        tracep->declBit(c+328,"SoC cpu decode is_AUIPC", false,-1);
        tracep->declBit(c+329,"SoC cpu decode is_BEQ", false,-1);
        tracep->declBit(c+330,"SoC cpu decode is_BGE", false,-1);
        tracep->declBit(c+331,"SoC cpu decode is_BGEU", false,-1);
        tracep->declBit(c+332,"SoC cpu decode is_BLT", false,-1);
        tracep->declBit(c+333,"SoC cpu decode is_BLTU", false,-1);
        tracep->declBit(c+334,"SoC cpu decode is_BNE", false,-1);
        tracep->declBit(c+335,"SoC cpu decode is_DIV", false,-1);
        tracep->declBit(c+336,"SoC cpu decode is_DIVU", false,-1);
        tracep->declBit(c+337,"SoC cpu decode is_JAL", false,-1);
        tracep->declBit(c+338,"SoC cpu decode is_JALR", false,-1);
        tracep->declBit(c+339,"SoC cpu decode is_LB", false,-1);
        tracep->declBit(c+340,"SoC cpu decode is_LBU", false,-1);
        tracep->declBit(c+341,"SoC cpu decode is_LH", false,-1);
        tracep->declBit(c+342,"SoC cpu decode is_LHU", false,-1);
        tracep->declBit(c+343,"SoC cpu decode is_LUI", false,-1);
        tracep->declBit(c+344,"SoC cpu decode is_LW", false,-1);
        tracep->declBit(c+345,"SoC cpu decode is_MUL", false,-1);
        tracep->declBit(c+346,"SoC cpu decode is_MULH", false,-1);
        tracep->declBit(c+347,"SoC cpu decode is_MULHU", false,-1);
        tracep->declBit(c+348,"SoC cpu decode is_OR", false,-1);
        tracep->declBit(c+349,"SoC cpu decode is_ORI", false,-1);
        tracep->declBit(c+350,"SoC cpu decode is_REM", false,-1);
        tracep->declBit(c+351,"SoC cpu decode is_REMU", false,-1);
        tracep->declBit(c+352,"SoC cpu decode is_SB", false,-1);
        tracep->declBit(c+353,"SoC cpu decode is_SH", false,-1);
        tracep->declBit(c+354,"SoC cpu decode is_SLL", false,-1);
        tracep->declBit(c+355,"SoC cpu decode is_SLLI", false,-1);
        tracep->declBit(c+356,"SoC cpu decode is_SLT", false,-1);
        tracep->declBit(c+357,"SoC cpu decode is_SLTI", false,-1);
        tracep->declBit(c+358,"SoC cpu decode is_SLTIU", false,-1);
        tracep->declBit(c+359,"SoC cpu decode is_SLTU", false,-1);
        tracep->declBit(c+360,"SoC cpu decode is_SRA", false,-1);
        tracep->declBit(c+361,"SoC cpu decode is_SRAI", false,-1);
        tracep->declBit(c+362,"SoC cpu decode is_SRL", false,-1);
        tracep->declBit(c+363,"SoC cpu decode is_SRLI", false,-1);
        tracep->declBit(c+364,"SoC cpu decode is_SUB", false,-1);
        tracep->declBit(c+365,"SoC cpu decode is_SW", false,-1);
        tracep->declBit(c+366,"SoC cpu decode is_XOR", false,-1);
        tracep->declBit(c+367,"SoC cpu decode is_XORI", false,-1);
        tracep->declBit(c+368,"SoC cpu decode is_B", false,-1);
        tracep->declBit(c+369,"SoC cpu decode is_I", false,-1);
        tracep->declBit(c+337,"SoC cpu decode is_J", false,-1);
        tracep->declBit(c+370,"SoC cpu decode is_R", false,-1);
        tracep->declBit(c+371,"SoC cpu decode is_S", false,-1);
        tracep->declBit(c+372,"SoC cpu decode is_U", false,-1);
        tracep->declBit(c+373,"SoC cpu decode is_JUMP_CONDITIONAL", false,-1);
        tracep->declBit(c+374,"SoC cpu decode is_MEMORY", false,-1);
        tracep->declBit(c+375,"SoC cpu decode is_JUMP", false,-1);
        tracep->declBit(c+376,"SoC cpu decode is_ARITHMETIC", false,-1);
        tracep->declBit(c+377,"SoC cpu decode is_COMPLEX", false,-1);
        tracep->declBus(c+378,"SoC cpu decode alu_operation", false,-1, 3,0);
        tracep->declBus(c+379,"SoC cpu decode alu_operand1", false,-1, 2,0);
        tracep->declBus(c+380,"SoC cpu decode alu_operand2", false,-1, 2,0);
        tracep->declBit(c+381,"SoC cpu decode memory_read", false,-1);
        tracep->declBit(c+382,"SoC cpu decode memory_write", false,-1);
        tracep->declBus(c+383,"SoC cpu decode memory_width", false,-1, 2,0);
        tracep->declBit(c+384,"SoC cpu decode memory_signed", false,-1);
        tracep->declBus(c+385,"SoC cpu decode inst_B_imm", false,-1, 31,0);
        tracep->declBus(c+386,"SoC cpu decode inst_I_imm", false,-1, 31,0);
        tracep->declBus(c+387,"SoC cpu decode inst_J_imm", false,-1, 31,0);
        tracep->declBus(c+388,"SoC cpu decode inst_S_imm", false,-1, 31,0);
        tracep->declBus(c+389,"SoC cpu decode inst_U_imm", false,-1, 31,0);
        tracep->declBus(c+390,"SoC cpu decode inst_R_imm", false,-1, 31,0);
        tracep->declBit(c+391,"SoC cpu decode have_RS1", false,-1);
        tracep->declBit(c+392,"SoC cpu decode have_RS2", false,-1);
        tracep->declBit(c+393,"SoC cpu decode have_RD", false,-1);
        tracep->declBit(c+508,"SoC cpu execute i_reset", false,-1);
        tracep->declBit(c+453,"SoC cpu execute i_clock", false,-1);
        tracep->declBit(c+219,"SoC cpu execute i_stall", false,-1);
        tracep->declBus(c+187,"SoC cpu execute i_tag", false,-1, 3,0);
        tracep->declBus(c+189,"SoC cpu execute i_pc", false,-1, 31,0);
        tracep->declBus(c+188,"SoC cpu execute i_instruction", false,-1, 31,0);
        tracep->declBus(c+423,"SoC cpu execute i_rs1", false,-1, 31,0);
        tracep->declBus(c+424,"SoC cpu execute i_rs2", false,-1, 31,0);
        tracep->declBus(c+192,"SoC cpu execute i_inst_rd", false,-1, 4,0);
        tracep->declBus(c+193,"SoC cpu execute i_imm", false,-1, 31,0);
        tracep->declBit(c+194,"SoC cpu execute i_arithmetic", false,-1);
        tracep->declBit(c+195,"SoC cpu execute i_jump", false,-1);
        tracep->declBit(c+196,"SoC cpu execute i_jump_conditional", false,-1);
        tracep->declBus(c+197,"SoC cpu execute i_alu_operation", false,-1, 3,0);
        tracep->declBus(c+198,"SoC cpu execute i_alu_operand1", false,-1, 2,0);
        tracep->declBus(c+199,"SoC cpu execute i_alu_operand2", false,-1, 2,0);
        tracep->declBit(c+200,"SoC cpu execute i_memory_read", false,-1);
        tracep->declBit(c+201,"SoC cpu execute i_memory_write", false,-1);
        tracep->declBus(c+202,"SoC cpu execute i_memory_width", false,-1, 2,0);
        tracep->declBit(c+203,"SoC cpu execute i_memory_signed", false,-1);
        tracep->declBus(c+204,"SoC cpu execute i_op", false,-1, 4,0);
        tracep->declBus(c+205,"SoC cpu execute o_tag", false,-1, 3,0);
        tracep->declBus(c+206,"SoC cpu execute o_inst_rd", false,-1, 4,0);
        tracep->declBus(c+207,"SoC cpu execute o_rd", false,-1, 31,0);
        tracep->declBus(c+208,"SoC cpu execute o_pc_next", false,-1, 31,0);
        tracep->declBit(c+209,"SoC cpu execute o_mem_read", false,-1);
        tracep->declBit(c+210,"SoC cpu execute o_mem_write", false,-1);
        tracep->declBus(c+211,"SoC cpu execute o_mem_width", false,-1, 2,0);
        tracep->declBit(c+212,"SoC cpu execute o_mem_signed", false,-1);
        tracep->declBus(c+213,"SoC cpu execute o_mem_address", false,-1, 31,0);
        tracep->declBit(c+214,"SoC cpu execute o_stall", false,-1);
        tracep->declBus(c+532,"SoC cpu execute OP_DIV", false,-1, 2,0);
        tracep->declBus(c+533,"SoC cpu execute OP_DIVU", false,-1, 2,0);
        tracep->declBus(c+534,"SoC cpu execute OP_MUL", false,-1, 2,0);
        tracep->declBus(c+535,"SoC cpu execute OP_MULH", false,-1, 2,0);
        tracep->declBus(c+536,"SoC cpu execute OP_MULHU", false,-1, 2,0);
        tracep->declBus(c+537,"SoC cpu execute OP_REM", false,-1, 2,0);
        tracep->declBus(c+538,"SoC cpu execute OP_REMU", false,-1, 2,0);
        tracep->declBus(c+394,"SoC cpu execute cycle", false,-1, 4,0);
        tracep->declBus(c+425,"SoC cpu execute alu_operand1", false,-1, 31,0);
        tracep->declBus(c+426,"SoC cpu execute alu_operand2", false,-1, 31,0);
        tracep->declBus(c+427,"SoC cpu execute alu_result", false,-1, 31,0);
        tracep->declBit(c+511,"SoC cpu execute alu_compare_result", false,-1);
        tracep->declBit(c+395,"SoC cpu execute mul_signed", false,-1);
        tracep->declBus(c+396,"SoC cpu execute mul_op1", false,-1, 31,0);
        tracep->declBus(c+397,"SoC cpu execute mul_op2", false,-1, 31,0);
        tracep->declQuad(c+398,"SoC cpu execute mul_result", false,-1, 63,0);
        tracep->declBit(c+400,"SoC cpu execute div_signed", false,-1);
        tracep->declBus(c+401,"SoC cpu execute div_numerator", false,-1, 31,0);
        tracep->declBus(c+402,"SoC cpu execute div_denominator", false,-1, 31,0);
        tracep->declBus(c+403,"SoC cpu execute div_result", false,-1, 31,0);
        tracep->declBus(c+404,"SoC cpu execute div_remainder", false,-1, 31,0);
        tracep->declBus(c+405,"SoC cpu execute jump_conditional_target", false,-1, 31,0);
        tracep->declBus(c+197,"SoC cpu execute alu i_op", false,-1, 3,0);
        tracep->declBus(c+425,"SoC cpu execute alu i_op1", false,-1, 31,0);
        tracep->declBus(c+426,"SoC cpu execute alu i_op2", false,-1, 31,0);
        tracep->declBus(c+427,"SoC cpu execute alu o_result", false,-1, 31,0);
        tracep->declBit(c+511,"SoC cpu execute alu o_compare_result", false,-1);
        tracep->declBus(c+428,"SoC cpu execute alu signed_sum", false,-1, 31,0);
        tracep->declBus(c+429,"SoC cpu execute alu unsigned_sum", false,-1, 31,0);
        tracep->declBus(c+430,"SoC cpu execute alu signed_diff", false,-1, 31,0);
        tracep->declBus(c+431,"SoC cpu execute alu and_result", false,-1, 31,0);
        tracep->declBus(c+432,"SoC cpu execute alu or_result", false,-1, 31,0);
        tracep->declBus(c+433,"SoC cpu execute alu xor_result", false,-1, 31,0);
        tracep->declBus(c+434,"SoC cpu execute alu shl_result", false,-1, 31,0);
        tracep->declBus(c+435,"SoC cpu execute alu shr_result", false,-1, 31,0);
        tracep->declBus(c+436,"SoC cpu execute alu ashr_result", false,-1, 31,0);
        tracep->declBit(c+437,"SoC cpu execute alu signed_lt_result", false,-1);
        tracep->declBit(c+438,"SoC cpu execute alu unsigned_lt_result", false,-1);
        tracep->declBit(c+439,"SoC cpu execute alu equal_result", false,-1);
        tracep->declBit(c+440,"SoC cpu execute alu not_equal_result", false,-1);
        tracep->declBit(c+441,"SoC cpu execute alu signed_get_result", false,-1);
        tracep->declBit(c+442,"SoC cpu execute alu unsigned_get_result", false,-1);
        tracep->declBit(c+453,"SoC cpu execute multiply i_clock", false,-1);
        tracep->declBit(c+395,"SoC cpu execute multiply i_signed", false,-1);
        tracep->declBus(c+396,"SoC cpu execute multiply i_op1", false,-1, 31,0);
        tracep->declBus(c+397,"SoC cpu execute multiply i_op2", false,-1, 31,0);
        tracep->declQuad(c+398,"SoC cpu execute multiply o_result", false,-1, 63,0);
        tracep->declQuad(c+406,"SoC cpu execute multiply p1", false,-1, 63,0);
        tracep->declQuad(c+398,"SoC cpu execute multiply p2", false,-1, 63,0);
        tracep->declBit(c+453,"SoC cpu execute divide i_clock", false,-1);
        tracep->declBit(c+400,"SoC cpu execute divide i_signed", false,-1);
        tracep->declBus(c+401,"SoC cpu execute divide i_numerator", false,-1, 31,0);
        tracep->declBus(c+402,"SoC cpu execute divide i_denominator", false,-1, 31,0);
        tracep->declBus(c+403,"SoC cpu execute divide o_result", false,-1, 31,0);
        tracep->declBus(c+404,"SoC cpu execute divide o_remainder", false,-1, 31,0);
        tracep->declBus(c+408,"SoC cpu execute divide p1_result", false,-1, 31,0);
        tracep->declBus(c+409,"SoC cpu execute divide p1_remainder", false,-1, 31,0);
        tracep->declBus(c+403,"SoC cpu execute divide p2_result", false,-1, 31,0);
        tracep->declBus(c+404,"SoC cpu execute divide p2_remainder", false,-1, 31,0);
        tracep->declBit(c+508,"SoC cpu memory i_reset", false,-1);
        tracep->declBit(c+453,"SoC cpu memory i_clock", false,-1);
        tracep->declBit(c+176,"SoC cpu memory i_bus_busy", false,-1);
        tracep->declBit(c+75,"SoC cpu memory o_bus_rw", false,-1);
        tracep->declBit(c+180,"SoC cpu memory o_bus_request", false,-1);
        tracep->declBit(c+181,"SoC cpu memory i_bus_ready", false,-1);
        tracep->declBus(c+76,"SoC cpu memory o_bus_address", false,-1, 31,0);
        tracep->declBus(c+159,"SoC cpu memory i_bus_rdata", false,-1, 31,0);
        tracep->declBus(c+77,"SoC cpu memory o_bus_wdata", false,-1, 31,0);
        tracep->declBus(c+205,"SoC cpu memory i_tag", false,-1, 3,0);
        tracep->declBus(c+206,"SoC cpu memory i_inst_rd", false,-1, 4,0);
        tracep->declBus(c+207,"SoC cpu memory i_rd", false,-1, 31,0);
        tracep->declBus(c+208,"SoC cpu memory i_pc_next", false,-1, 31,0);
        tracep->declBit(c+209,"SoC cpu memory i_mem_read", false,-1);
        tracep->declBit(c+210,"SoC cpu memory i_mem_write", false,-1);
        tracep->declBus(c+211,"SoC cpu memory i_mem_width", false,-1, 2,0);
        tracep->declBit(c+212,"SoC cpu memory i_mem_signed", false,-1);
        tracep->declBus(c+213,"SoC cpu memory i_mem_address", false,-1, 31,0);
        tracep->declBus(c+215,"SoC cpu memory o_tag", false,-1, 3,0);
        tracep->declBus(c+216,"SoC cpu memory o_inst_rd", false,-1, 4,0);
        tracep->declBus(c+217,"SoC cpu memory o_rd", false,-1, 31,0);
        tracep->declBus(c+218,"SoC cpu memory o_pc_next", false,-1, 31,0);
        tracep->declBit(c+219,"SoC cpu memory o_stall", false,-1);
        tracep->declBus(c+539,"SoC cpu memory STATE_RMW_READ", false,-1, 31,0);
        tracep->declBus(c+515,"SoC cpu memory STATE_RMW_RST_REQUEST", false,-1, 31,0);
        tracep->declBus(c+540,"SoC cpu memory STATE_RMW_WAIT_WRITE", false,-1, 31,0);
        tracep->declBus(c+410,"SoC cpu memory state", false,-1, 4,0);
        tracep->declBit(c+411,"SoC cpu memory dcache_rw", false,-1);
        tracep->declBit(c+412,"SoC cpu memory dcache_request", false,-1);
        tracep->declBit(c+413,"SoC cpu memory dcache_ready", false,-1);
        tracep->declBus(c+414,"SoC cpu memory dcache_address", false,-1, 31,0);
        tracep->declBus(c+415,"SoC cpu memory dcache_rdata", false,-1, 31,0);
        tracep->declBus(c+416,"SoC cpu memory dcache_wdata", false,-1, 31,0);
        tracep->declBus(c+417,"SoC cpu memory address_byte_index", false,-1, 1,0);
        tracep->declBus(c+418,"SoC cpu memory bus_rdata_byte", false,-1, 7,0);
        tracep->declBus(c+419,"SoC cpu memory bus_rdata_half", false,-1, 15,0);
        tracep->declBit(c+508,"SoC cpu memory dcache i_reset", false,-1);
        tracep->declBit(c+453,"SoC cpu memory dcache i_clock", false,-1);
        tracep->declBit(c+176,"SoC cpu memory dcache i_bus_busy", false,-1);
        tracep->declBit(c+75,"SoC cpu memory dcache o_bus_rw", false,-1);
        tracep->declBit(c+180,"SoC cpu memory dcache o_bus_request", false,-1);
        tracep->declBit(c+181,"SoC cpu memory dcache i_bus_ready", false,-1);
        tracep->declBus(c+76,"SoC cpu memory dcache o_bus_address", false,-1, 31,0);
        tracep->declBus(c+159,"SoC cpu memory dcache i_bus_rdata", false,-1, 31,0);
        tracep->declBus(c+77,"SoC cpu memory dcache o_bus_wdata", false,-1, 31,0);
        tracep->declBit(c+411,"SoC cpu memory dcache i_rw", false,-1);
        tracep->declBit(c+412,"SoC cpu memory dcache i_request", false,-1);
        tracep->declBit(c+413,"SoC cpu memory dcache o_ready", false,-1);
        tracep->declBus(c+414,"SoC cpu memory dcache i_address", false,-1, 31,0);
        tracep->declBus(c+415,"SoC cpu memory dcache o_rdata", false,-1, 31,0);
        tracep->declBus(c+416,"SoC cpu memory dcache i_wdata", false,-1, 31,0);
        tracep->declBus(c+541,"SoC cpu memory dcache SIZE", false,-1, 31,0);
        tracep->declBus(c+542,"SoC cpu memory dcache RANGE", false,-1, 31,0);
        tracep->declBus(c+531,"SoC cpu memory dcache ST_IDLE", false,-1, 31,0);
        tracep->declBus(c+539,"SoC cpu memory dcache ST_RD_BUS", false,-1, 31,0);
        tracep->declBus(c+515,"SoC cpu memory dcache ST_RD_BUS_WB", false,-1, 31,0);
        tracep->declBus(c+540,"SoC cpu memory dcache ST_RD_BUS_UNCACHEABLE", false,-1, 31,0);
        tracep->declBus(c+517,"SoC cpu memory dcache ST_WR_BUS", false,-1, 31,0);
        tracep->declBus(c+543,"SoC cpu memory dcache ST_WAIT_END", false,-1, 31,0);
        tracep->declBus(c+82,"SoC cpu memory dcache state", false,-1, 7,0);
        tracep->declArray(c+83,"SoC cpu memory dcache valid", false,-1, 255,0);
        tracep->declArray(c+91,"SoC cpu memory dcache dirty", false,-1, 255,0);
        tracep->declBit(c+99,"SoC cpu memory dcache cache_rw", false,-1);
        tracep->declBus(c+420,"SoC cpu memory dcache cache_label", false,-1, 7,0);
        tracep->declQuad(c+100,"SoC cpu memory dcache cache_wdata", false,-1, 63,0);
        tracep->declQuad(c+102,"SoC cpu memory dcache cache_rdata", false,-1, 63,0);
        tracep->declBit(c+421,"SoC cpu memory dcache is_cacheable", false,-1);
        tracep->declBus(c+530,"SoC cpu memory dcache cache WIDTH", false,-1, 31,0);
        tracep->declBus(c+542,"SoC cpu memory dcache cache SIZE", false,-1, 31,0);
        tracep->declBus(c+531,"SoC cpu memory dcache cache ADDR_LSH", false,-1, 31,0);
        tracep->declBit(c+453,"SoC cpu memory dcache cache i_clock", false,-1);
        tracep->declBit(c+512,"SoC cpu memory dcache cache i_request", false,-1);
        tracep->declBit(c+99,"SoC cpu memory dcache cache i_rw", false,-1);
        tracep->declBus(c+422,"SoC cpu memory dcache cache i_address", false,-1, 31,0);
        tracep->declQuad(c+100,"SoC cpu memory dcache cache i_wdata", false,-1, 63,0);
        tracep->declQuad(c+102,"SoC cpu memory dcache cache o_rdata", false,-1, 63,0);
        tracep->declBit(c+104,"SoC cpu memory dcache cache o_ready", false,-1);
        tracep->declBit(c+508,"SoC cpu writeback i_reset", false,-1);
        tracep->declBit(c+453,"SoC cpu writeback i_clock", false,-1);
        tracep->declBus(c+215,"SoC cpu writeback i_tag", false,-1, 3,0);
        tracep->declBus(c+216,"SoC cpu writeback i_inst_rd", false,-1, 4,0);
        tracep->declBus(c+217,"SoC cpu writeback i_rd", false,-1, 31,0);
        tracep->declBus(c+218,"SoC cpu writeback i_pc_next", false,-1, 31,0);
        tracep->declBus(c+220,"SoC cpu writeback o_tag", false,-1, 3,0);
        tracep->declBus(c+78,"SoC cpu writeback o_inst_rd", false,-1, 4,0);
        tracep->declBus(c+79,"SoC cpu writeback o_rd", false,-1, 31,0);
        tracep->declBus(c+221,"SoC cpu writeback o_pc_next", false,-1, 31,0);
    }
}

void VSoC::traceRegister(VerilatedVcd* tracep) {
    // Body
    {
        tracep->addFullCb(&traceFullTop0, __VlSymsp);
        tracep->addChgCb(&traceChgTop0, __VlSymsp);
        tracep->addCleanupCb(&traceCleanup, __VlSymsp);
    }
}

void VSoC::traceFullTop0(void* userp, VerilatedVcd* tracep) {
    VSoC__Syms* __restrict vlSymsp = static_cast<VSoC__Syms*>(userp);
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    {
        vlTOPp->traceFullSub0(userp, tracep);
    }
}

void VSoC::traceFullSub0(void* userp, VerilatedVcd* tracep) {
    VSoC__Syms* __restrict vlSymsp = static_cast<VSoC__Syms*>(userp);
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    vluint32_t* const oldp = tracep->oldp(vlSymsp->__Vm_baseCode);
    if (false && oldp) {}  // Prevent unused
    // Body
    {
        tracep->fullBit(oldp+1,(vlTOPp->SoC__DOT__video_sram_request));
        tracep->fullBit(oldp+2,(vlTOPp->SoC__DOT__video_sram_rw));
        tracep->fullIData(oldp+3,(vlTOPp->SoC__DOT__video_sram_address),32);
        tracep->fullIData(oldp+4,(vlTOPp->SoC__DOT__video_sram_wdata),32);
        tracep->fullIData(oldp+5,(vlTOPp->SoC__DOT__video_sram_rdata),32);
        tracep->fullBit(oldp+6,(vlTOPp->SoC__DOT__video_sram_ready));
        tracep->fullBit(oldp+7,(vlTOPp->SoC__DOT__vram_ready));
        tracep->fullBit(oldp+8,(vlTOPp->SoC__DOT__vga_enable));
        tracep->fullSData(oldp+9,(vlTOPp->SoC__DOT__vga_address),16);
        tracep->fullIData(oldp+10,(vlTOPp->SoC__DOT__rom_rdata),32);
        tracep->fullBit(oldp+11,(vlTOPp->SoC__DOT__rom_ready));
        tracep->fullIData(oldp+12,(vlTOPp->SoC__DOT__ram_rdata),32);
        tracep->fullBit(oldp+13,(vlTOPp->SoC__DOT__ram_ready));
        tracep->fullIData(oldp+14,(vlTOPp->SoC__DOT__sdram_rdata),32);
        tracep->fullBit(oldp+15,(vlTOPp->SoC__DOT__sdram_ready));
        tracep->fullBit(oldp+16,(vlTOPp->SoC__DOT__led_ready));
        tracep->fullIData(oldp+17,(vlTOPp->SoC__DOT__uart_rdata),32);
        tracep->fullIData(oldp+18,(vlTOPp->SoC__DOT__i2c_rdata),32);
        tracep->fullBit(oldp+19,(vlTOPp->SoC__DOT__i2c_ready));
        tracep->fullIData(oldp+20,(vlTOPp->SoC__DOT__sd_rdata),32);
        tracep->fullBit(oldp+21,(vlTOPp->SoC__DOT__sd_ready));
        tracep->fullIData(oldp+22,(vlTOPp->SoC__DOT__timer_rdata),32);
        tracep->fullIData(oldp+23,(vlTOPp->SoC__DOT__cpu_retire_count),32);
        tracep->fullIData(oldp+24,(vlTOPp->SoC__DOT____Vcellout__video_bus__o_video_rdata),32);
        tracep->fullBit(oldp+25,(((IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__in) 
                                  == (IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__out))));
        tracep->fullBit(oldp+26,(((0x3ffU & ((IData)(1U) 
                                             + (IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__in))) 
                                  == (IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__out))));
        tracep->fullBit(oldp+27,(vlTOPp->SoC__DOT__video_bus__DOT__fifo_write));
        tracep->fullQData(oldp+28,(vlTOPp->SoC__DOT__video_bus__DOT__fifo_wdata),64);
        tracep->fullBit(oldp+30,(vlTOPp->SoC__DOT__video_bus__DOT__fifo_read));
        tracep->fullQData(oldp+31,(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__rdata),64);
        tracep->fullCData(oldp+33,(vlTOPp->SoC__DOT__video_bus__DOT__state),3);
        tracep->fullBit(oldp+34,(vlTOPp->SoC__DOT__video_bus__DOT__last_video_address));
        tracep->fullSData(oldp+35,(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__in),10);
        tracep->fullSData(oldp+36,(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__out),10);
        tracep->fullBit(oldp+37,(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__last_write));
        tracep->fullBit(oldp+38,(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__last_read));
        tracep->fullCData(oldp+39,(vlTOPp->SoC__DOT__vga__DOT__prescale),4);
        tracep->fullSData(oldp+40,(vlTOPp->SoC__DOT__vga__DOT__vga_h),10);
        tracep->fullSData(oldp+41,(vlTOPp->SoC__DOT__vga__DOT__vga_v),10);
        tracep->fullBit(oldp+42,(vlTOPp->SoC__DOT__uart__DOT__tx_ready));
        tracep->fullBit(oldp+43,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__frame_error));
        tracep->fullIData(oldp+44,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__prescale),19);
        tracep->fullCData(oldp+45,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__data),8);
        tracep->fullCData(oldp+46,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__bidx),4);
        tracep->fullCData(oldp+47,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds),4);
        tracep->fullBit(oldp+48,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx));
        tracep->fullBit(oldp+49,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_empty));
        tracep->fullBit(oldp+50,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_write));
        tracep->fullBit(oldp+51,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_read));
        tracep->fullCData(oldp+52,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__rdata),8);
        tracep->fullBit(oldp+53,(((3U & ((IData)(1U) 
                                         + (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__in))) 
                                  == (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__out))));
        tracep->fullCData(oldp+54,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data[0]),8);
        tracep->fullCData(oldp+55,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data[1]),8);
        tracep->fullCData(oldp+56,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data[2]),8);
        tracep->fullCData(oldp+57,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data[3]),8);
        tracep->fullCData(oldp+58,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__in),2);
        tracep->fullCData(oldp+59,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__out),2);
        tracep->fullBit(oldp+60,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__last_write));
        tracep->fullBit(oldp+61,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__last_read));
        tracep->fullIData(oldp+62,(vlTOPp->SoC__DOT__uart__DOT__tx__DOT__prescale),19);
        tracep->fullSData(oldp+63,(vlTOPp->SoC__DOT__uart__DOT__tx__DOT__data),9);
        tracep->fullCData(oldp+64,(vlTOPp->SoC__DOT__uart__DOT__tx__DOT__bidx),4);
        tracep->fullBit(oldp+65,(vlTOPp->SoC__DOT__i2c__DOT__scl));
        tracep->fullBit(oldp+66,(vlTOPp->SoC__DOT__i2c__DOT__sda));
        tracep->fullBit(oldp+67,(vlTOPp->SoC__DOT__sd__DOT__clk));
        tracep->fullBit(oldp+68,(vlTOPp->SoC__DOT__sd__DOT__cdir));
        tracep->fullBit(oldp+69,(vlTOPp->SoC__DOT__sd__DOT__ddir));
        tracep->fullBit(oldp+70,(vlTOPp->SoC__DOT__sd__DOT__cmd));
        tracep->fullCData(oldp+71,(vlTOPp->SoC__DOT__sd__DOT__dat),4);
        tracep->fullIData(oldp+72,(vlTOPp->SoC__DOT__timer__DOT__prescale),17);
        tracep->fullIData(oldp+73,(vlTOPp->SoC__DOT__timer__DOT__cycles),32);
        tracep->fullIData(oldp+74,(vlTOPp->SoC__DOT__timer__DOT__ms),32);
        tracep->fullBit(oldp+75,(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rw));
        tracep->fullIData(oldp+76,(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_address),32);
        tracep->fullIData(oldp+77,(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_wdata),32);
        tracep->fullCData(oldp+78,(vlTOPp->SoC__DOT__cpu__DOT__writeback_inst_rd),5);
        tracep->fullIData(oldp+79,(vlTOPp->SoC__DOT__cpu__DOT__writeback_rd),32);
        tracep->fullCData(oldp+80,(vlTOPp->SoC__DOT__cpu__DOT__retire_tag),4);
        tracep->fullBit(oldp+81,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__o_ready));
        tracep->fullCData(oldp+82,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state),8);
        tracep->fullWData(oldp+83,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid),256);
        tracep->fullWData(oldp+91,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty),256);
        tracep->fullBit(oldp+99,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw));
        tracep->fullQData(oldp+100,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_wdata),64);
        tracep->fullQData(oldp+102,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rdata),64);
        tracep->fullBit(oldp+104,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__o_ready));
        tracep->fullSData(oldp+105,(vlTOPp->SoC__DOT__led__DOT__leds),10);
        tracep->fullIData(oldp+106,(vlTOPp->SoC__DOT__cpu__DOT__rs1),32);
        tracep->fullIData(oldp+107,(vlTOPp->SoC__DOT__cpu__DOT__rs2),32);
        tracep->fullCData(oldp+108,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__read_tag),4);
        tracep->fullCData(oldp+109,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__write_tag),4);
        tracep->fullIData(oldp+110,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0]),32);
        tracep->fullIData(oldp+111,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[1]),32);
        tracep->fullIData(oldp+112,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[2]),32);
        tracep->fullIData(oldp+113,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[3]),32);
        tracep->fullIData(oldp+114,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[4]),32);
        tracep->fullIData(oldp+115,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[5]),32);
        tracep->fullIData(oldp+116,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[6]),32);
        tracep->fullIData(oldp+117,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[7]),32);
        tracep->fullIData(oldp+118,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[8]),32);
        tracep->fullIData(oldp+119,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[9]),32);
        tracep->fullIData(oldp+120,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[10]),32);
        tracep->fullIData(oldp+121,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[11]),32);
        tracep->fullIData(oldp+122,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[12]),32);
        tracep->fullIData(oldp+123,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[13]),32);
        tracep->fullIData(oldp+124,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[14]),32);
        tracep->fullIData(oldp+125,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[15]),32);
        tracep->fullIData(oldp+126,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[16]),32);
        tracep->fullIData(oldp+127,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[17]),32);
        tracep->fullIData(oldp+128,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[18]),32);
        tracep->fullIData(oldp+129,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[19]),32);
        tracep->fullIData(oldp+130,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[20]),32);
        tracep->fullIData(oldp+131,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[21]),32);
        tracep->fullIData(oldp+132,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[22]),32);
        tracep->fullIData(oldp+133,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[23]),32);
        tracep->fullIData(oldp+134,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[24]),32);
        tracep->fullIData(oldp+135,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[25]),32);
        tracep->fullIData(oldp+136,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[26]),32);
        tracep->fullIData(oldp+137,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[27]),32);
        tracep->fullIData(oldp+138,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[28]),32);
        tracep->fullIData(oldp+139,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[29]),32);
        tracep->fullIData(oldp+140,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[30]),32);
        tracep->fullIData(oldp+141,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[31]),32);
        tracep->fullBit(oldp+142,(vlTOPp->SoC__DOT__vram_select));
        tracep->fullIData(oldp+143,((vlTOPp->SoC__DOT__cpu_address 
                                     - (IData)(0x40000000U))),32);
        tracep->fullBit(oldp+144,((0x10000U > vlTOPp->SoC__DOT__cpu_address)));
        tracep->fullIData(oldp+145,(vlTOPp->SoC__DOT__cpu_address),32);
        tracep->fullBit(oldp+146,(vlTOPp->SoC__DOT__ram_select));
        tracep->fullIData(oldp+147,((vlTOPp->SoC__DOT__cpu_address 
                                     - (IData)(0x10000U))),32);
        tracep->fullBit(oldp+148,(vlTOPp->SoC__DOT__sdram_select));
        tracep->fullIData(oldp+149,((vlTOPp->SoC__DOT__cpu_address 
                                     - (IData)(0x20000000U))),32);
        tracep->fullBit(oldp+150,(vlTOPp->SoC__DOT__led_select));
        tracep->fullBit(oldp+151,(vlTOPp->SoC__DOT__uart_select));
        tracep->fullCData(oldp+152,((3U & (vlTOPp->SoC__DOT__cpu_address 
                                           >> 2U))),2);
        tracep->fullBit(oldp+153,(vlTOPp->SoC__DOT__i2c_select));
        tracep->fullBit(oldp+154,(vlTOPp->SoC__DOT__sd_select));
        tracep->fullBit(oldp+155,(vlTOPp->SoC__DOT__timer_select));
        tracep->fullBit(oldp+156,(vlTOPp->SoC__DOT__cpu_rw));
        tracep->fullBit(oldp+157,(vlTOPp->SoC__DOT__cpu_request));
        tracep->fullBit(oldp+158,(vlTOPp->SoC__DOT__cpu_ready));
        tracep->fullIData(oldp+159,(vlTOPp->SoC__DOT__cpu_rdata),32);
        tracep->fullIData(oldp+160,(vlTOPp->SoC__DOT__cpu_wdata),32);
        tracep->fullBit(oldp+161,(vlTOPp->SoC__DOT____Vcellinp__video_bus__i_cpu_request));
        tracep->fullBit(oldp+162,(vlTOPp->SoC__DOT____Vcellinp__rom__i_request));
        tracep->fullBit(oldp+163,(vlTOPp->SoC__DOT____Vcellinp__ram__i_request));
        tracep->fullBit(oldp+164,(vlTOPp->SoC__DOT____Vcellinp__sdram__i_request));
        tracep->fullBit(oldp+165,(vlTOPp->SoC__DOT____Vcellinp__led__i_request));
        tracep->fullBit(oldp+166,(vlTOPp->SoC__DOT____Vcellinp__uart__i_request));
        tracep->fullBit(oldp+167,(vlTOPp->SoC__DOT__uart__DOT__rx_request));
        tracep->fullBit(oldp+168,(vlTOPp->SoC__DOT__uart__DOT__tx_request));
        tracep->fullBit(oldp+169,(vlTOPp->SoC__DOT____Vcellinp__i2c__i_request));
        tracep->fullCData(oldp+170,((0xfU & (vlTOPp->SoC__DOT__cpu_wdata 
                                             >> 4U))),4);
        tracep->fullCData(oldp+171,((0xfU & vlTOPp->SoC__DOT__cpu_wdata)),4);
        tracep->fullBit(oldp+172,(vlTOPp->SoC__DOT____Vcellinp__sd__i_request));
        tracep->fullCData(oldp+173,((0xffU & (vlTOPp->SoC__DOT__cpu_wdata 
                                              >> 8U))),8);
        tracep->fullCData(oldp+174,((0xffU & vlTOPp->SoC__DOT__cpu_wdata)),8);
        tracep->fullBit(oldp+175,(((IData)(vlTOPp->SoC__DOT__timer_select) 
                                   & (IData)(vlTOPp->SoC__DOT__cpu_request))));
        tracep->fullBit(oldp+176,(vlTOPp->SoC__DOT__cpu__DOT__bus_busy));
        tracep->fullBit(oldp+177,(vlTOPp->SoC__DOT__cpu__DOT__bus_pa_request));
        tracep->fullBit(oldp+178,(((~ (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request)) 
                                   & (IData)(vlTOPp->SoC__DOT__cpu_ready))));
        tracep->fullIData(oldp+179,(vlTOPp->SoC__DOT__cpu__DOT__bus_pa_address),32);
        tracep->fullBit(oldp+180,(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request));
        tracep->fullBit(oldp+181,(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_ready));
        tracep->fullCData(oldp+182,((0x1fU & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                              >> 0xfU))),5);
        tracep->fullCData(oldp+183,((0x1fU & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                              >> 0x14U))),5);
        tracep->fullCData(oldp+184,(vlTOPp->SoC__DOT__cpu__DOT__fetch_tag),4);
        tracep->fullIData(oldp+185,(vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction),32);
        tracep->fullIData(oldp+186,(vlTOPp->SoC__DOT__cpu__DOT__fetch_pc),32);
        tracep->fullCData(oldp+187,(vlTOPp->SoC__DOT__cpu__DOT__decode_tag),4);
        tracep->fullIData(oldp+188,(vlTOPp->SoC__DOT__cpu__DOT__decode_instruction),32);
        tracep->fullIData(oldp+189,(vlTOPp->SoC__DOT__cpu__DOT__decode_pc),32);
        tracep->fullCData(oldp+190,(vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rs1),5);
        tracep->fullCData(oldp+191,(vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rs2),5);
        tracep->fullCData(oldp+192,(vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rd),5);
        tracep->fullIData(oldp+193,(vlTOPp->SoC__DOT__cpu__DOT__decode_imm),32);
        tracep->fullBit(oldp+194,(vlTOPp->SoC__DOT__cpu__DOT__decode_arithmetic));
        tracep->fullBit(oldp+195,(vlTOPp->SoC__DOT__cpu__DOT__decode_jump));
        tracep->fullBit(oldp+196,(vlTOPp->SoC__DOT__cpu__DOT__decode_jump_conditional));
        tracep->fullCData(oldp+197,(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation),4);
        tracep->fullCData(oldp+198,(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand1),3);
        tracep->fullCData(oldp+199,(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand2),3);
        tracep->fullBit(oldp+200,(vlTOPp->SoC__DOT__cpu__DOT__decode_memory_read));
        tracep->fullBit(oldp+201,(vlTOPp->SoC__DOT__cpu__DOT__decode_memory_write));
        tracep->fullCData(oldp+202,(vlTOPp->SoC__DOT__cpu__DOT__decode_memory_width),3);
        tracep->fullBit(oldp+203,(vlTOPp->SoC__DOT__cpu__DOT__decode_memory_signed));
        tracep->fullCData(oldp+204,(vlTOPp->SoC__DOT__cpu__DOT__decode_op),5);
        tracep->fullCData(oldp+205,(vlTOPp->SoC__DOT__cpu__DOT__execute_tag),4);
        tracep->fullCData(oldp+206,(vlTOPp->SoC__DOT__cpu__DOT__execute_inst_rd),5);
        tracep->fullIData(oldp+207,(vlTOPp->SoC__DOT__cpu__DOT__execute_rd),32);
        tracep->fullIData(oldp+208,(vlTOPp->SoC__DOT__cpu__DOT__execute_pc_next),32);
        tracep->fullBit(oldp+209,(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_read));
        tracep->fullBit(oldp+210,(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_write));
        tracep->fullCData(oldp+211,(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_width),3);
        tracep->fullBit(oldp+212,(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_signed));
        tracep->fullIData(oldp+213,(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address),32);
        tracep->fullBit(oldp+214,(vlTOPp->SoC__DOT__cpu__DOT__execute_stall));
        tracep->fullCData(oldp+215,(vlTOPp->SoC__DOT__cpu__DOT__memory_tag),4);
        tracep->fullCData(oldp+216,(vlTOPp->SoC__DOT__cpu__DOT__memory_inst_rd),5);
        tracep->fullIData(oldp+217,(vlTOPp->SoC__DOT__cpu__DOT__memory_rd),32);
        tracep->fullIData(oldp+218,(vlTOPp->SoC__DOT__cpu__DOT__memory_pc_next),32);
        tracep->fullBit(oldp+219,(vlTOPp->SoC__DOT__cpu__DOT__memory_stall));
        tracep->fullCData(oldp+220,(vlTOPp->SoC__DOT__cpu__DOT__writeback_tag),4);
        tracep->fullIData(oldp+221,(vlTOPp->SoC__DOT__cpu__DOT__writeback_pc_next),32);
        tracep->fullBit(oldp+222,(((IData)(vlTOPp->SoC__DOT__cpu__DOT__memory_stall) 
                                   | (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_stall))));
        tracep->fullCData(oldp+223,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__state),3);
        tracep->fullIData(oldp+224,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc),32);
        tracep->fullIData(oldp+225,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_output_pc),32);
        tracep->fullIData(oldp+226,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata),32);
        tracep->fullBit(oldp+227,((0x33U == (0xfe00707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+228,((0x13U == (0x707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+229,((0x7033U == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+230,((0x7013U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+231,((0x17U == (0x7fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+232,((0x63U == (0x707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+233,((0x5063U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+234,((0x7063U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+235,((0x4063U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+236,((0x6063U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+237,((0x1063U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+238,((0x2004033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+239,((0x2005033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+240,((0x6fU == (0x7fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+241,((0x67U == (0x707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+242,((3U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+243,((0x4003U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+244,((0x1003U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+245,((0x5003U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+246,((0x37U == (0x7fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+247,((0x2003U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+248,((0x2000033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+249,((0x2001033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+250,((0x2003033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+251,((0x6033U == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+252,((0x6013U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+253,((0x2006033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+254,((0x2007033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+255,((0x23U == (0x707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+256,((0x1023U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+257,((0x1033U == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+258,((0x1013U == (0xfc00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+259,((0x2033U == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+260,((0x2013U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+261,((0x3013U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+262,((0x3033U == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+263,((0x40005033U == (0xfe00707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+264,((0x40005013U == (0xfc00707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+265,((0x5033U == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+266,((0x5013U == (0xfc00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+267,((0x40000033U == (0xfe00707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+268,((0x2023U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+269,((0x4033U == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+270,((0x4013U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
        tracep->fullBit(oldp+271,(((((((0x63U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                                       | (0x5063U == 
                                          (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                      | (0x7063U == 
                                         (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                     | (0x4063U == 
                                        (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                    | (0x6063U == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                   | (0x1063U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)))));
        tracep->fullBit(oldp+272,(((((((((((((0x13U 
                                              == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                                             | (0x7013U 
                                                == 
                                                (0x707fU 
                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                            | (0x67U 
                                               == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                           | (3U == 
                                              (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                          | (0x4003U 
                                             == (0x707fU 
                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                         | (0x1003U 
                                            == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                        | (0x5003U 
                                           == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                       | (0x2003U == 
                                          (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                      | (0x6013U == 
                                         (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                     | (0x2013U == 
                                        (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                    | (0x3013U == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                   | (0x4013U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)))));
        tracep->fullBit(oldp+273,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__is_R));
        tracep->fullBit(oldp+274,((((0x23U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                                    | (0x1023U == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                   | (0x2023U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)))));
        tracep->fullBit(oldp+275,(((0x17U == (0x7fU 
                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                                   | (0x37U == (0x7fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)))));
        tracep->fullBit(oldp+276,(((((((((3U == (0x707fU 
                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                                         | (0x4003U 
                                            == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                        | (0x1003U 
                                           == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                       | (0x5003U == 
                                          (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                      | (0x2003U == 
                                         (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                     | (0x23U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                    | (0x1023U == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                   | (0x2023U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)))));
        tracep->fullBit(oldp+277,(((0x6fU == (0x7fU 
                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                                   | (0x67U == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)))));
        tracep->fullBit(oldp+278,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__is_ARITHMETIC));
        tracep->fullBit(oldp+279,((((((((0x2004033U 
                                         == (0xfe00707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                                        | (0x2005033U 
                                           == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                       | (0x2000033U 
                                          == (0xfe00707fU 
                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                      | (0x2001033U 
                                         == (0xfe00707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                     | (0x2003033U 
                                        == (0xfe00707fU 
                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                    | (0x2006033U == 
                                       (0xfe00707fU 
                                        & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                   | (0x2007033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)))));
        tracep->fullCData(oldp+280,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state),4);
        tracep->fullWData(oldp+281,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid),1024);
        tracep->fullBit(oldp+313,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw));
        tracep->fullSData(oldp+314,((0xffU & (vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc 
                                              >> 2U))),10);
        tracep->fullQData(oldp+315,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_wdata),64);
        tracep->fullQData(oldp+317,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rdata),64);
        tracep->fullIData(oldp+319,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_input_address),32);
        tracep->fullIData(oldp+320,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__address),32);
        tracep->fullIData(oldp+321,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__prefetch),32);
        tracep->fullIData(oldp+322,((0xffU & (vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc 
                                              >> 2U))),32);
        tracep->fullCData(oldp+323,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state),4);
        tracep->fullBit(oldp+324,((0x33U == (0xfe00707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+325,((0x13U == (0x707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+326,((0x7033U == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+327,((0x7013U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+328,((0x17U == (0x7fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+329,((0x63U == (0x707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+330,((0x5063U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+331,((0x7063U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+332,((0x4063U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+333,((0x6063U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+334,((0x1063U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+335,((0x2004033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+336,((0x2005033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+337,((0x6fU == (0x7fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+338,((0x67U == (0x707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+339,((3U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+340,((0x4003U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+341,((0x1003U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+342,((0x5003U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+343,((0x37U == (0x7fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+344,((0x2003U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+345,((0x2000033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+346,((0x2001033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+347,((0x2003033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+348,((0x6033U == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+349,((0x6013U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+350,((0x2006033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+351,((0x2007033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+352,((0x23U == (0x707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+353,((0x1023U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+354,((0x1033U == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+355,((0x1013U == (0xfc00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+356,((0x2033U == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+357,((0x2013U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+358,((0x3013U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+359,((0x3033U == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+360,((0x40005033U == (0xfe00707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+361,((0x40005013U == (0xfc00707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+362,((0x5033U == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+363,((0x5013U == (0xfc00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+364,((0x40000033U == (0xfe00707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+365,((0x2023U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+366,((0x4033U == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+367,((0x4013U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
        tracep->fullBit(oldp+368,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_B));
        tracep->fullBit(oldp+369,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_I));
        tracep->fullBit(oldp+370,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_R));
        tracep->fullBit(oldp+371,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_S));
        tracep->fullBit(oldp+372,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_U));
        tracep->fullBit(oldp+373,(((((((0x63U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                                       | (0x5063U == 
                                          (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                      | (0x7063U == 
                                         (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                     | (0x4063U == 
                                        (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                    | (0x6063U == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                   | (0x1063U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)))));
        tracep->fullBit(oldp+374,(((((((((3U == (0x707fU 
                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                                         | (0x4003U 
                                            == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                        | (0x1003U 
                                           == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                       | (0x5003U == 
                                          (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                      | (0x2003U == 
                                         (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                     | (0x23U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                    | (0x1023U == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                   | (0x2023U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)))));
        tracep->fullBit(oldp+375,(((0x6fU == (0x7fU 
                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                                   | (0x67U == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)))));
        tracep->fullBit(oldp+376,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_ARITHMETIC));
        tracep->fullBit(oldp+377,((((((((0x2004033U 
                                         == (0xfe00707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                                        | (0x2005033U 
                                           == (0xfe00707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                       | (0x2000033U 
                                          == (0xfe00707fU 
                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                      | (0x2001033U 
                                         == (0xfe00707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                     | (0x2003033U 
                                        == (0xfe00707fU 
                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                    | (0x2006033U == 
                                       (0xfe00707fU 
                                        & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                   | (0x2007033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)))));
        tracep->fullCData(oldp+378,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__alu_operation),4);
        tracep->fullCData(oldp+379,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__alu_operand1),3);
        tracep->fullCData(oldp+380,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__alu_operand2),3);
        tracep->fullBit(oldp+381,((((((3U == (0x707fU 
                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                                      | (0x4003U == 
                                         (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                     | (0x1003U == 
                                        (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                    | (0x5003U == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                   | (0x2003U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)))));
        tracep->fullBit(oldp+382,((((0x23U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                                    | (0x1023U == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                   | (0x2023U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)))));
        tracep->fullCData(oldp+383,(((3U == (0x707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                      ? 1U : ((0x4003U 
                                               == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                               ? 1U
                                               : ((0x1003U 
                                                   == 
                                                   (0x707fU 
                                                    & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                   ? 2U
                                                   : 
                                                  ((0x5003U 
                                                    == 
                                                    (0x707fU 
                                                     & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                    ? 2U
                                                    : 
                                                   ((0x2003U 
                                                     == 
                                                     (0x707fU 
                                                      & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                     ? 4U
                                                     : 
                                                    ((0x23U 
                                                      == 
                                                      (0x707fU 
                                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                      ? 1U
                                                      : 
                                                     ((0x1023U 
                                                       == 
                                                       (0x707fU 
                                                        & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                       ? 2U
                                                       : 
                                                      ((0x2023U 
                                                        == 
                                                        (0x707fU 
                                                         & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                        ? 4U
                                                        : 0U))))))))),3);
        tracep->fullBit(oldp+384,(((3U == (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                                   | (0x1003U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)))));
        tracep->fullIData(oldp+385,(((0xfffff000U & 
                                      ((- (IData)((1U 
                                                   & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                      >> 0x1fU)))) 
                                       << 0xcU)) | 
                                     ((0x800U & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                 << 4U)) 
                                      | ((0x7e0U & 
                                          (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                           >> 0x14U)) 
                                         | (0x1eU & 
                                            (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                             >> 7U)))))),32);
        tracep->fullIData(oldp+386,(((0xfffff800U & 
                                      ((- (IData)((1U 
                                                   & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                      >> 0x1fU)))) 
                                       << 0xbU)) | 
                                     (0x7ffU & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                >> 0x14U)))),32);
        tracep->fullIData(oldp+387,(((0xfff00000U & 
                                      ((- (IData)((1U 
                                                   & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                      >> 0x1fU)))) 
                                       << 0x14U)) | 
                                     ((0xff000U & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction) 
                                      | ((0x800U & 
                                          (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                           >> 9U)) 
                                         | (0x7feU 
                                            & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                               >> 0x14U)))))),32);
        tracep->fullIData(oldp+388,(((0xfffff800U & 
                                      ((- (IData)((1U 
                                                   & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                      >> 0x1fU)))) 
                                       << 0xbU)) | 
                                     ((0x7e0U & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                 >> 0x14U)) 
                                      | (0x1fU & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                  >> 7U))))),32);
        tracep->fullIData(oldp+389,((0xfffff000U & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)),32);
        tracep->fullIData(oldp+390,((0x3fU & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                              >> 0x14U))),32);
        tracep->fullBit(oldp+391,(((((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_B) 
                                     | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_I)) 
                                    | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_R)) 
                                   | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_S))));
        tracep->fullBit(oldp+392,((((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_B) 
                                    | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_R)) 
                                   | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_S))));
        tracep->fullBit(oldp+393,(((((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_I) 
                                     | (0x6fU == (0x7fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                    | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_R)) 
                                   | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_U))));
        tracep->fullCData(oldp+394,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle),5);
        tracep->fullBit(oldp+395,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_signed));
        tracep->fullIData(oldp+396,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_op1),32);
        tracep->fullIData(oldp+397,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_op2),32);
        tracep->fullQData(oldp+398,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p2),64);
        tracep->fullBit(oldp+400,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_signed));
        tracep->fullIData(oldp+401,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_numerator),32);
        tracep->fullIData(oldp+402,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_denominator),32);
        tracep->fullIData(oldp+403,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p2_result),32);
        tracep->fullIData(oldp+404,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p2_remainder),32);
        tracep->fullIData(oldp+405,((vlTOPp->SoC__DOT__cpu__DOT__decode_pc 
                                     + vlTOPp->SoC__DOT__cpu__DOT__decode_imm)),32);
        tracep->fullQData(oldp+406,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p1),64);
        tracep->fullIData(oldp+408,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p1_result),32);
        tracep->fullIData(oldp+409,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p1_remainder),32);
        tracep->fullCData(oldp+410,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__state),5);
        tracep->fullBit(oldp+411,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rw));
        tracep->fullBit(oldp+412,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_request));
        tracep->fullBit(oldp+413,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_ready));
        tracep->fullIData(oldp+414,((0xfffffffcU & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address)),32);
        tracep->fullIData(oldp+415,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata),32);
        tracep->fullIData(oldp+416,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_wdata),32);
        tracep->fullCData(oldp+417,((3U & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address)),2);
        tracep->fullCData(oldp+418,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_byte),8);
        tracep->fullSData(oldp+419,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_half),16);
        tracep->fullCData(oldp+420,((0xffU & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                              >> 2U))),8);
        tracep->fullBit(oldp+421,((0x40000000U > (0xfffffffcU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address))));
        tracep->fullIData(oldp+422,((0xffU & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                              >> 2U))),32);
        tracep->fullIData(oldp+423,(vlTOPp->SoC__DOT__cpu__DOT__fwd_rs1),32);
        tracep->fullIData(oldp+424,(vlTOPp->SoC__DOT__cpu__DOT__fwd_rs2),32);
        tracep->fullIData(oldp+425,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1),32);
        tracep->fullIData(oldp+426,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2),32);
        tracep->fullIData(oldp+427,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_result),32);
        tracep->fullIData(oldp+428,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                     + vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)),32);
        tracep->fullIData(oldp+429,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                     + vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)),32);
        tracep->fullIData(oldp+430,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                     - vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)),32);
        tracep->fullIData(oldp+431,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                     & vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)),32);
        tracep->fullIData(oldp+432,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                     | vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)),32);
        tracep->fullIData(oldp+433,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                     ^ vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)),32);
        tracep->fullIData(oldp+434,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                     << (0x1fU & vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2))),32);
        tracep->fullIData(oldp+435,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                     >> (0x1fU & vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2))),32);
        tracep->fullIData(oldp+436,(VL_SHIFTRS_III(32,32,5, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1, 
                                                   (0x1fU 
                                                    & vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2))),32);
        tracep->fullBit(oldp+437,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_lt_result));
        tracep->fullBit(oldp+438,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__unsigned_lt_result));
        tracep->fullBit(oldp+439,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__equal_result));
        tracep->fullBit(oldp+440,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__not_equal_result));
        tracep->fullBit(oldp+441,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_get_result));
        tracep->fullBit(oldp+442,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__unsigned_get_result));
        tracep->fullBit(oldp+443,(vlTOPp->ADC_CONVST));
        tracep->fullBit(oldp+444,(vlTOPp->ADC_SCK));
        tracep->fullBit(oldp+445,(vlTOPp->ADC_SDI));
        tracep->fullBit(oldp+446,(vlTOPp->ADC_SDO));
        tracep->fullBit(oldp+447,(vlTOPp->AUD_ADCDAT));
        tracep->fullBit(oldp+448,(vlTOPp->AUD_ADCLRCK));
        tracep->fullBit(oldp+449,(vlTOPp->AUD_BCLK));
        tracep->fullBit(oldp+450,(vlTOPp->AUD_DACDAT));
        tracep->fullBit(oldp+451,(vlTOPp->AUD_DACLRCK));
        tracep->fullBit(oldp+452,(vlTOPp->AUD_XCK));
        tracep->fullBit(oldp+453,(vlTOPp->CLOCK_125_p));
        tracep->fullBit(oldp+454,(vlTOPp->CLOCK_50_B5B));
        tracep->fullBit(oldp+455,(vlTOPp->CLOCK_50_B6A));
        tracep->fullBit(oldp+456,(vlTOPp->CLOCK_50_B7A));
        tracep->fullBit(oldp+457,(vlTOPp->CLOCK_50_B8A));
        tracep->fullBit(oldp+458,(vlTOPp->CPU_RESET_n));
        tracep->fullSData(oldp+459,(vlTOPp->DDR2LP_CA),10);
        tracep->fullCData(oldp+460,(vlTOPp->DDR2LP_CKE),2);
        tracep->fullBit(oldp+461,(vlTOPp->DDR2LP_CK_n));
        tracep->fullBit(oldp+462,(vlTOPp->DDR2LP_CK_p));
        tracep->fullCData(oldp+463,(vlTOPp->DDR2LP_CS_n),2);
        tracep->fullCData(oldp+464,(vlTOPp->DDR2LP_DM),4);
        tracep->fullIData(oldp+465,(vlTOPp->DDR2LP_DQ),32);
        tracep->fullCData(oldp+466,(vlTOPp->DDR2LP_DQS_n),4);
        tracep->fullCData(oldp+467,(vlTOPp->DDR2LP_DQS_p),4);
        tracep->fullBit(oldp+468,(vlTOPp->DDR2LP_OCT_RZQ));
        tracep->fullQData(oldp+469,(vlTOPp->GPIO),36);
        tracep->fullBit(oldp+471,(vlTOPp->HDMI_TX_CLK));
        tracep->fullIData(oldp+472,(vlTOPp->HDMI_TX_D),24);
        tracep->fullBit(oldp+473,(vlTOPp->HDMI_TX_DE));
        tracep->fullBit(oldp+474,(vlTOPp->HDMI_TX_HS));
        tracep->fullBit(oldp+475,(vlTOPp->HDMI_TX_INT));
        tracep->fullBit(oldp+476,(vlTOPp->HDMI_TX_VS));
        tracep->fullCData(oldp+477,(vlTOPp->HEX0),7);
        tracep->fullCData(oldp+478,(vlTOPp->HEX1),7);
        tracep->fullBit(oldp+479,(vlTOPp->HSMC_CLKIN0));
        tracep->fullCData(oldp+480,(vlTOPp->HSMC_CLKIN_n),2);
        tracep->fullCData(oldp+481,(vlTOPp->HSMC_CLKIN_p),2);
        tracep->fullBit(oldp+482,(vlTOPp->HSMC_CLKOUT0));
        tracep->fullCData(oldp+483,(vlTOPp->HSMC_CLKOUT_n),2);
        tracep->fullCData(oldp+484,(vlTOPp->HSMC_CLKOUT_p),2);
        tracep->fullCData(oldp+485,(vlTOPp->HSMC_D),4);
        tracep->fullIData(oldp+486,(vlTOPp->HSMC_RX_n),17);
        tracep->fullIData(oldp+487,(vlTOPp->HSMC_RX_p),17);
        tracep->fullIData(oldp+488,(vlTOPp->HSMC_TX_n),17);
        tracep->fullIData(oldp+489,(vlTOPp->HSMC_TX_p),17);
        tracep->fullBit(oldp+490,(vlTOPp->I2C_SCL));
        tracep->fullBit(oldp+491,(vlTOPp->I2C_SDA));
        tracep->fullCData(oldp+492,(vlTOPp->KEY),4);
        tracep->fullCData(oldp+493,(vlTOPp->LEDG),8);
        tracep->fullSData(oldp+494,(vlTOPp->LEDR),10);
        tracep->fullBit(oldp+495,(vlTOPp->SD_CLK));
        tracep->fullBit(oldp+496,(vlTOPp->SD_CMD));
        tracep->fullCData(oldp+497,(vlTOPp->SD_DAT),4);
        tracep->fullIData(oldp+498,(vlTOPp->SRAM_A),18);
        tracep->fullBit(oldp+499,(vlTOPp->SRAM_CE_n));
        tracep->fullSData(oldp+500,(vlTOPp->SRAM_D),16);
        tracep->fullBit(oldp+501,(vlTOPp->SRAM_LB_n));
        tracep->fullBit(oldp+502,(vlTOPp->SRAM_OE_n));
        tracep->fullBit(oldp+503,(vlTOPp->SRAM_UB_n));
        tracep->fullBit(oldp+504,(vlTOPp->SRAM_WE_n));
        tracep->fullSData(oldp+505,(vlTOPp->SW),10);
        tracep->fullBit(oldp+506,(vlTOPp->UART_RX));
        tracep->fullBit(oldp+507,(vlTOPp->UART_TX));
        tracep->fullBit(oldp+508,(vlTOPp->SoC__DOT__reset));
        tracep->fullBit(oldp+509,(((IData)(vlTOPp->SoC__DOT__uart__DOT__rx_request)
                                    ? ((5U == (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds)) 
                                       & (IData)(vlTOPp->SoC__DOT__uart__DOT__rx_request))
                                    : ((IData)(vlTOPp->SoC__DOT__uart__DOT__tx_request) 
                                       & (IData)(vlTOPp->SoC__DOT__uart__DOT__tx_ready)))));
        tracep->fullBit(oldp+510,(((5U == (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds)) 
                                   & (IData)(vlTOPp->SoC__DOT__uart__DOT__rx_request))));
        tracep->fullBit(oldp+511,((1U & ((0xaU == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                          ? (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_lt_result)
                                          : ((0xbU 
                                              == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                              ? (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__unsigned_lt_result)
                                              : ((0xcU 
                                                  == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                                  ? (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__equal_result)
                                                  : 
                                                 ((0xdU 
                                                   == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                                   ? (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__not_equal_result)
                                                   : 
                                                  ((0xeU 
                                                    == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                                    ? (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_get_result)
                                                    : 
                                                   ((0xfU 
                                                     == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                                     ? (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__unsigned_get_result)
                                                     : 0U)))))))));
        tracep->fullBit(oldp+512,(1U));
        tracep->fullIData(oldp+513,(0x20U),32);
        tracep->fullIData(oldp+514,(0xfa00U),32);
        tracep->fullIData(oldp+515,(2U),32);
        tracep->fullIData(oldp+516,(0x400U),32);
        tracep->fullIData(oldp+517,(4U),32);
        tracep->fullIData(oldp+518,(0x4000U),32);
        tracep->fullIData(oldp+519,(0x2000000U),32);
        tracep->fullIData(oldp+520,(0x6cU),32);
        tracep->fullBit(oldp+521,(0U));
        tracep->fullBit(oldp+522,(1U));
        tracep->fullIData(oldp+523,(0x5f5e100U),32);
        tracep->fullIData(oldp+524,(0x186a0U),32);
        tracep->fullIData(oldp+525,(0x11U),32);
        tracep->fullIData(oldp+526,(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_wdata),32);
        tracep->fullBit(oldp+527,(0U));
        tracep->fullIData(oldp+528,(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__i_pa_wdata),32);
        tracep->fullIData(oldp+529,(0xaU),32);
        tracep->fullIData(oldp+530,(0x40U),32);
        tracep->fullIData(oldp+531,(0U),32);
        tracep->fullCData(oldp+532,(1U),3);
        tracep->fullCData(oldp+533,(2U),3);
        tracep->fullCData(oldp+534,(3U),3);
        tracep->fullCData(oldp+535,(4U),3);
        tracep->fullCData(oldp+536,(5U),3);
        tracep->fullCData(oldp+537,(6U),3);
        tracep->fullCData(oldp+538,(7U),3);
        tracep->fullIData(oldp+539,(1U),32);
        tracep->fullIData(oldp+540,(3U),32);
        tracep->fullIData(oldp+541,(8U),32);
        tracep->fullIData(oldp+542,(0x100U),32);
        tracep->fullIData(oldp+543,(5U),32);
    }
}
