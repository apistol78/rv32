// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VSoC.h for the primary calling header

#include "VSoC.h"
#include "VSoC__Syms.h"

//==========

VL_CTOR_IMP(VSoC) {
    VSoC__Syms* __restrict vlSymsp = __VlSymsp = new VSoC__Syms(this, name());
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    _ctor_var_reset();
}

void VSoC::__Vconfigure(VSoC__Syms* vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
    if (false && this->__VlSymsp) {}  // Prevent unused
    Verilated::timeunit(-9);
    Verilated::timeprecision(-9);
}

VSoC::~VSoC() {
    VL_DO_CLEAR(delete __VlSymsp, __VlSymsp = NULL);
}

void VSoC::_initial__TOP__1(VSoC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoC::_initial__TOP__1\n"); );
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    WData/*223:0*/ __Vtemp1[7];
    // Body
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__o_ready = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__o_ready = 0U;
    vlTOPp->AUD_ADCLRCK = 0U;
    vlTOPp->AUD_DACLRCK = 0U;
    vlTOPp->SRAM_D = 0U;
    vlTOPp->HSMC_D = 0U;
    vlTOPp->HSMC_RX_n = 0U;
    vlTOPp->HSMC_RX_p = 0U;
    vlTOPp->HSMC_TX_n = 0U;
    vlTOPp->HSMC_TX_p = 0U;
    vlTOPp->DDR2LP_DQS_p = 0U;
    vlTOPp->DDR2LP_DQ = 0U;
    vlTOPp->AUD_BCLK = 0U;
    vlTOPp->DDR2LP_DQS_n = 0U;
    vlTOPp->GPIO = 0ULL;
    vlTOPp->HDMI_TX_CLK = 0U;
    vlTOPp->SoC__DOT__led__DOT__leds = 0x2aaU;
    vlTOPp->SoC__DOT__sd_rdata = 0U;
    vlTOPp->SoC__DOT__rom_ready = 0U;
    vlTOPp->SoC__DOT__ram_ready = 0U;
    vlTOPp->SoC__DOT__sdram_ready = 0U;
    vlTOPp->SoC__DOT__led_ready = 0U;
    __Vtemp1[0U] = 0x766d656dU;
    __Vtemp1[1U] = 0x6172652eU;
    __Vtemp1[2U] = 0x69726d77U;
    __Vtemp1[3U] = 0x72652f46U;
    __Vtemp1[4U] = 0x726d7761U;
    __Vtemp1[5U] = 0x652f4669U;
    __Vtemp1[6U] = 0x636f64U;
    VL_READMEM_N(true, 32, 401, 0, VL_CVT_PACK_STR_NW(7, __Vtemp1)
                 , vlTOPp->SoC__DOT__rom__DOT__data
                 , 0, ~0ULL);
    vlTOPp->SoC__DOT__vga__DOT__prescale = 0U;
    vlTOPp->SoC__DOT__i2c__DOT__sda = 1U;
    vlTOPp->SoC__DOT__timer__DOT__prescale = 0U;
    vlTOPp->SoC__DOT__timer__DOT__cycles = 0U;
    vlTOPp->SoC__DOT__timer__DOT__ms = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__retire_tag = 0U;
    vlTOPp->SoC__DOT__cpu_retire_count = 0U;
    vlTOPp->SoC__DOT__i2c__DOT__scl = 1U;
    vlTOPp->SoC__DOT__sd__DOT__clk = 0U;
    vlTOPp->SoC__DOT__sd__DOT__cdir = 0U;
    vlTOPp->SoC__DOT__sd__DOT__ddir = 0U;
    vlTOPp->SoC__DOT__uart__DOT__tx_ready = 0U;
    vlTOPp->SoC__DOT__uart__DOT__tx__DOT__prescale = 0U;
    vlTOPp->SoC__DOT__uart__DOT__tx__DOT__data = 0U;
    vlTOPp->SoC__DOT__uart__DOT__tx__DOT__bidx = 0U;
    vlTOPp->UART_TX = 1U;
    vlTOPp->SoC__DOT__cpu__DOT__rs1 = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__rs2 = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__read_tag = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__write_tag = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[1U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[2U] = 0x103fcU;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[3U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[4U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[5U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[6U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[7U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[8U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[9U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0xaU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0xbU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0xcU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0xdU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0xeU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0xfU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x10U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x11U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x12U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x13U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x14U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x15U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x16U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x17U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x18U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x19U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x1aU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x1bU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x1cU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x1dU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x1eU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0x1fU] = 0U;
    vlTOPp->SoC__DOT__video_sram_ready = 0U;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_write = 0U;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_read = 0U;
    vlTOPp->HDMI_TX_HS = 1U;
    vlTOPp->HDMI_TX_VS = 1U;
    vlTOPp->SoC__DOT__vga_address = 0U;
    vlTOPp->SoC__DOT__vga_enable = 0U;
    vlTOPp->HDMI_TX_CLK = 0U;
    vlTOPp->SoC__DOT__vga__DOT__vga_h = 0U;
    vlTOPp->SoC__DOT__vga__DOT__vga_v = 0U;
    vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__rdata = 0ULL;
    vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__in = 0U;
    vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__out = 0U;
    vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__last_write = 0U;
    vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__last_read = 0U;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__rdata = 0U;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__in = 0U;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__out = 0U;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__last_write = 0U;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__last_read = 0U;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__frame_error = 0U;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__prescale = 0U;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__data = 0U;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__bidx = 0U;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds = 0U;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx = 0U;
    vlTOPp->SoC__DOT__uart_rdata = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__state = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rw = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_request = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_wdata = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory_tag = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory_inst_rd = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory_rd = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory_pc_next = 0U;
    vlTOPp->SoC__DOT__vram_ready = 0U;
    vlTOPp->SoC__DOT____Vcellout__video_bus__o_video_rdata = 0U;
    vlTOPp->SoC__DOT__video_sram_request = 0U;
    vlTOPp->SoC__DOT__video_sram_rw = 0U;
    vlTOPp->SoC__DOT__video_sram_address = 0U;
    vlTOPp->SoC__DOT__video_sram_wdata = 0U;
    vlTOPp->SoC__DOT__video_bus__DOT__fifo_write = 0U;
    vlTOPp->SoC__DOT__video_bus__DOT__fifo_read = 0U;
    vlTOPp->SoC__DOT__video_bus__DOT__state = 0U;
    vlTOPp->SoC__DOT__video_bus__DOT__last_video_address = 1U;
    vlTOPp->SoC__DOT__cpu__DOT__execute_inst_rd = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__execute_pc_next = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__execute_mem_read = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__execute_mem_write = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__execute_mem_width = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__execute_mem_signed = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__execute_tag = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__decode_tag = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__decode_op = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__writeback_tag = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__writeback_pc_next = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_output_pc = 0xffffffffU;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_input_address = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[1U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[2U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[3U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[4U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[5U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[6U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[7U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[8U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[9U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xaU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xbU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xcU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xdU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xeU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xfU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x10U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x11U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x12U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x13U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x14U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x15U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x16U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x17U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x18U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x19U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1aU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1bU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1cU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1dU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1eU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1fU] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_wdata = 0ULL;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__state = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch_tag = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch_pc = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__bus_pa_request = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__bus_pa_address = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__address = 0xffffffffU;
    vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rw = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__bus_pb_address = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__bus_pb_wdata = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[0U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[1U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[2U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[3U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[4U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[5U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[6U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[7U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[0U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[1U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[2U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[3U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[4U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[5U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[6U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[7U] = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_wdata = 0ULL;
}

void VSoC::_settle__TOP__4(VSoC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoC::_settle__TOP__4\n"); );
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->I2C_SDA = 0U;
    vlTOPp->SoC__DOT__reset = (1U & (~ (IData)(vlTOPp->CPU_RESET_n)));
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__is_R = 
        ((((((((((((((((((((0x33U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                           | (0x7033U == (0xfe00707fU 
                                          & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                          | (0x2004033U == (0xfe00707fU 
                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                         | (0x2005033U == (0xfe00707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                        | (0x2000033U == (0xfe00707fU 
                                          & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                       | (0x2001033U == (0xfe00707fU 
                                         & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                      | (0x2003033U == (0xfe00707fU 
                                        & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                     | (0x6033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                    | (0x2006033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                   | (0x2007033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                  | (0x1033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                 | (0x1013U == (0xfc00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                | (0x2033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
               | (0x3033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
              | (0x40005033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
             | (0x40005013U == (0xfc00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
            | (0x5033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
           | (0x5013U == (0xfc00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
          | (0x40000033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
         | (0x4033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)));
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__is_ARITHMETIC 
        = (((((((((((((((((((((0x33U == (0xfe00707fU 
                                         & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                              | (0x13U == (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                             | (0x7033U == (0xfe00707fU 
                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                            | (0x7013U == (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                           | (0x17U == (0x7fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                          | (0x37U == (0x7fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                         | (0x6033U == (0xfe00707fU 
                                        & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                        | (0x6013U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                       | (0x1033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                      | (0x1013U == (0xfc00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                     | (0x2033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                    | (0x2013U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                   | (0x3013U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                  | (0x3033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                 | (0x40005033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                | (0x40005013U == (0xfc00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
               | (0x5033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
              | (0x5013U == (0xfc00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
             | (0x40000033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
            | (0x4033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
           | (0x4013U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)));
    vlTOPp->LEDR = vlTOPp->SoC__DOT__led__DOT__leds;
    vlTOPp->I2C_SCL = vlTOPp->SoC__DOT__i2c__DOT__scl;
    vlTOPp->SD_CLK = vlTOPp->SoC__DOT__sd__DOT__clk;
    vlTOPp->SD_CMD = (((((IData)(vlTOPp->SoC__DOT__sd__DOT__cdir) 
                         & (IData)(vlTOPp->SoC__DOT__sd__DOT__cmd)) 
                        & (IData)(vlTOPp->SoC__DOT__sd__DOT__cdir)) 
                       & (IData)(vlTOPp->SoC__DOT__sd__DOT__cdir)) 
                      & (IData)(vlTOPp->SoC__DOT__sd__DOT__cdir));
    vlTOPp->SD_DAT = (((((IData)(vlTOPp->SoC__DOT__sd__DOT__ddir)
                          ? (IData)(vlTOPp->SoC__DOT__sd__DOT__dat)
                          : 0U) & ((IData)(vlTOPp->SoC__DOT__sd__DOT__ddir)
                                    ? 0xfU : 0U)) & 
                       ((IData)(vlTOPp->SoC__DOT__sd__DOT__ddir)
                         ? 0xfU : 0U)) & ((IData)(vlTOPp->SoC__DOT__sd__DOT__ddir)
                                           ? 0xfU : 0U));
    vlTOPp->HDMI_TX_DE = vlTOPp->SoC__DOT__vga_enable;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_empty 
        = ((IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__in) 
           == (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__out));
    vlTOPp->HDMI_TX_D = (0xffffffU & vlTOPp->SoC__DOT____Vcellout__video_bus__o_video_rdata);
    vlTOPp->SoC__DOT__cpu__DOT__fwd_rs1 = ((0U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rs1))
                                            ? 0U : 
                                           ((((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rs1) 
                                              == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_inst_rd)) 
                                             & (~ (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_read)))
                                             ? vlTOPp->SoC__DOT__cpu__DOT__execute_rd
                                             : (((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rs1) 
                                                 == (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory_inst_rd))
                                                 ? vlTOPp->SoC__DOT__cpu__DOT__memory_rd
                                                 : 
                                                (((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rs1) 
                                                  == (IData)(vlTOPp->SoC__DOT__cpu__DOT__writeback_inst_rd))
                                                  ? vlTOPp->SoC__DOT__cpu__DOT__writeback_rd
                                                  : vlTOPp->SoC__DOT__cpu__DOT__rs1))));
    vlTOPp->SoC__DOT__cpu__DOT__fwd_rs2 = ((0U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rs2))
                                            ? 0U : 
                                           ((((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rs2) 
                                              == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_inst_rd)) 
                                             & (~ (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_read)))
                                             ? vlTOPp->SoC__DOT__cpu__DOT__execute_rd
                                             : (((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rs2) 
                                                 == (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory_inst_rd))
                                                 ? vlTOPp->SoC__DOT__cpu__DOT__memory_rd
                                                 : 
                                                (((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rs2) 
                                                  == (IData)(vlTOPp->SoC__DOT__cpu__DOT__writeback_inst_rd))
                                                  ? vlTOPp->SoC__DOT__cpu__DOT__writeback_rd
                                                  : vlTOPp->SoC__DOT__cpu__DOT__rs2))));
    vlTOPp->SoC__DOT__cpu__DOT__memory_stall = (((IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_tag) 
                                                 != (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory_tag)) 
                                                & ((IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_read) 
                                                   | (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_write)));
    vlTOPp->SoC__DOT__cpu__DOT__execute_stall = (((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_tag) 
                                                  != (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_tag)) 
                                                 & (0U 
                                                    != (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle)));
    vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_B = 
        ((((((0x63U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
             | (0x5063U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
            | (0x7063U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
           | (0x4063U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
          | (0x6063U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
         | (0x1063U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)));
    vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_I = 
        ((((((((((((0x13U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                   | (0x7013U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                  | (0x67U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                 | (3U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                | (0x4003U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
               | (0x1003U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
              | (0x5003U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
             | (0x2003U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
            | (0x6013U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
           | (0x2013U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
          | (0x3013U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
         | (0x4013U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)));
    vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_R = 
        ((((((((((((((((((((0x33U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                           | (0x7033U == (0xfe00707fU 
                                          & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                          | (0x2004033U == (0xfe00707fU 
                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                         | (0x2005033U == (0xfe00707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                        | (0x2000033U == (0xfe00707fU 
                                          & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                       | (0x2001033U == (0xfe00707fU 
                                         & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                      | (0x2003033U == (0xfe00707fU 
                                        & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                     | (0x6033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                    | (0x2006033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                   | (0x2007033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                  | (0x1033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                 | (0x1013U == (0xfc00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                | (0x2033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
               | (0x3033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
              | (0x40005033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
             | (0x40005013U == (0xfc00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
            | (0x5033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
           | (0x5013U == (0xfc00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
          | (0x40000033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
         | (0x4033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)));
    vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_S = 
        (((0x23U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
          | (0x1023U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
         | (0x2023U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)));
    vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_U = 
        ((0x17U == (0x7fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
         | (0x37U == (0x7fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)));
    vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_ARITHMETIC 
        = (((((((((((((((((((((0x33U == (0xfe00707fU 
                                         & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                              | (0x13U == (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                             | (0x7033U == (0xfe00707fU 
                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                            | (0x7013U == (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                           | (0x17U == (0x7fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                          | (0x37U == (0x7fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                         | (0x6033U == (0xfe00707fU 
                                        & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                        | (0x6013U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                       | (0x1033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                      | (0x1013U == (0xfc00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                     | (0x2033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                    | (0x2013U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                   | (0x3013U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                  | (0x3033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                 | (0x40005033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                | (0x40005013U == (0xfc00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
               | (0x5033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
              | (0x5013U == (0xfc00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
             | (0x40000033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
            | (0x4033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
           | (0x4013U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)));
    if ((0x33U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) {
        vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__alu_operation = 1U;
        vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__alu_operand1 = 1U;
        vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__alu_operand2 = 2U;
    } else {
        vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__alu_operation 
            = ((0x13U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                ? 1U : ((0x7033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                         ? 4U : ((0x7013U == (0x707fU 
                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                  ? 4U : ((0x17U == 
                                           (0x7fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                           ? 1U : (
                                                   (0x63U 
                                                    == 
                                                    (0x707fU 
                                                     & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                    ? 0xcU
                                                    : 
                                                   ((0x5063U 
                                                     == 
                                                     (0x707fU 
                                                      & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                     ? 0xeU
                                                     : 
                                                    ((0x7063U 
                                                      == 
                                                      (0x707fU 
                                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                      ? 0xfU
                                                      : 
                                                     ((0x4063U 
                                                       == 
                                                       (0x707fU 
                                                        & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                       ? 0xaU
                                                       : 
                                                      ((0x6063U 
                                                        == 
                                                        (0x707fU 
                                                         & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                        ? 0xbU
                                                        : 
                                                       ((0x1063U 
                                                         == 
                                                         (0x707fU 
                                                          & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                         ? 0xdU
                                                         : 
                                                        ((0x6fU 
                                                          == 
                                                          (0x7fU 
                                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                          ? 1U
                                                          : 
                                                         ((0x67U 
                                                           == 
                                                           (0x707fU 
                                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                           ? 1U
                                                           : 
                                                          ((3U 
                                                            == 
                                                            (0x707fU 
                                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                            ? 1U
                                                            : 
                                                           ((0x4003U 
                                                             == 
                                                             (0x707fU 
                                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                             ? 1U
                                                             : 
                                                            ((0x1003U 
                                                              == 
                                                              (0x707fU 
                                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                              ? 1U
                                                              : 
                                                             ((0x5003U 
                                                               == 
                                                               (0x707fU 
                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                               ? 1U
                                                               : 
                                                              ((0x37U 
                                                                == 
                                                                (0x7fU 
                                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                ? 2U
                                                                : 
                                                               ((0x2003U 
                                                                 == 
                                                                 (0x707fU 
                                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                 ? 1U
                                                                 : 
                                                                ((0x6033U 
                                                                  == 
                                                                  (0xfe00707fU 
                                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                  ? 5U
                                                                  : 
                                                                 ((0x6013U 
                                                                   == 
                                                                   (0x707fU 
                                                                    & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                   ? 5U
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
                                                                     ? 1U
                                                                     : 
                                                                    ((0x1033U 
                                                                      == 
                                                                      (0xfe00707fU 
                                                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                      ? 7U
                                                                      : 
                                                                     ((0x1013U 
                                                                       == 
                                                                       (0xfc00707fU 
                                                                        & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                       ? 7U
                                                                       : 
                                                                      ((0x2033U 
                                                                        == 
                                                                        (0xfe00707fU 
                                                                         & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                        ? 0xaU
                                                                        : 
                                                                       ((0x2013U 
                                                                         == 
                                                                         (0x707fU 
                                                                          & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                         ? 0xaU
                                                                         : 
                                                                        ((0x3013U 
                                                                          == 
                                                                          (0x707fU 
                                                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                          ? 0xbU
                                                                          : 
                                                                         ((0x3033U 
                                                                           == 
                                                                           (0xfe00707fU 
                                                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                           ? 0xbU
                                                                           : 
                                                                          ((0x40005033U 
                                                                            == 
                                                                            (0xfe00707fU 
                                                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                            ? 9U
                                                                            : 
                                                                           ((0x40005013U 
                                                                             == 
                                                                             (0xfc00707fU 
                                                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                             ? 9U
                                                                             : 
                                                                            ((0x5033U 
                                                                              == 
                                                                              (0xfe00707fU 
                                                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                              ? 8U
                                                                              : 
                                                                             ((0x5013U 
                                                                               == 
                                                                               (0xfc00707fU 
                                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                               ? 8U
                                                                               : 
                                                                              ((0x40000033U 
                                                                                == 
                                                                                (0xfe00707fU 
                                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                                ? 3U
                                                                                : 
                                                                               ((0x2023U 
                                                                                == 
                                                                                (0x707fU 
                                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                                 ? 1U
                                                                                 : 
                                                                                ((0x4033U 
                                                                                == 
                                                                                (0xfe00707fU 
                                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                                 ? 6U
                                                                                 : 
                                                                                ((0x4013U 
                                                                                == 
                                                                                (0x707fU 
                                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                                 ? 6U
                                                                                 : 0U))))))))))))))))))))))))))))))))))));
        vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__alu_operand1 
            = ((0x13U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                ? 1U : ((0x7033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                         ? 1U : ((0x7013U == (0x707fU 
                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                  ? 1U : ((0x17U == 
                                           (0x7fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                           ? 3U : (
                                                   (0x63U 
                                                    == 
                                                    (0x707fU 
                                                     & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                    ? 1U
                                                    : 
                                                   ((0x5063U 
                                                     == 
                                                     (0x707fU 
                                                      & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                     ? 1U
                                                     : 
                                                    ((0x7063U 
                                                      == 
                                                      (0x707fU 
                                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                      ? 1U
                                                      : 
                                                     ((0x4063U 
                                                       == 
                                                       (0x707fU 
                                                        & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                       ? 1U
                                                       : 
                                                      ((0x6063U 
                                                        == 
                                                        (0x707fU 
                                                         & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                        ? 1U
                                                        : 
                                                       ((0x1063U 
                                                         == 
                                                         (0x707fU 
                                                          & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                         ? 1U
                                                         : 
                                                        ((0x6fU 
                                                          == 
                                                          (0x7fU 
                                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                          ? 3U
                                                          : 
                                                         ((0x67U 
                                                           == 
                                                           (0x707fU 
                                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                           ? 1U
                                                           : 
                                                          ((3U 
                                                            == 
                                                            (0x707fU 
                                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                            ? 1U
                                                            : 
                                                           ((0x4003U 
                                                             == 
                                                             (0x707fU 
                                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                             ? 1U
                                                             : 
                                                            ((0x1003U 
                                                              == 
                                                              (0x707fU 
                                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                              ? 1U
                                                              : 
                                                             ((0x5003U 
                                                               == 
                                                               (0x707fU 
                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                               ? 1U
                                                               : 
                                                              ((0x37U 
                                                                == 
                                                                (0x7fU 
                                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                ? 4U
                                                                : 
                                                               ((0x2003U 
                                                                 == 
                                                                 (0x707fU 
                                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                 ? 1U
                                                                 : 
                                                                ((0x6033U 
                                                                  == 
                                                                  (0xfe00707fU 
                                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                  ? 1U
                                                                  : 
                                                                 ((0x6013U 
                                                                   == 
                                                                   (0x707fU 
                                                                    & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                   ? 1U
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
                                                                     ? 1U
                                                                     : 
                                                                    ((0x1033U 
                                                                      == 
                                                                      (0xfe00707fU 
                                                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                      ? 1U
                                                                      : 
                                                                     ((0x1013U 
                                                                       == 
                                                                       (0xfc00707fU 
                                                                        & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                       ? 1U
                                                                       : 
                                                                      ((0x2033U 
                                                                        == 
                                                                        (0xfe00707fU 
                                                                         & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                        ? 1U
                                                                        : 
                                                                       ((0x2013U 
                                                                         == 
                                                                         (0x707fU 
                                                                          & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                         ? 1U
                                                                         : 
                                                                        ((0x3013U 
                                                                          == 
                                                                          (0x707fU 
                                                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                          ? 1U
                                                                          : 
                                                                         ((0x3033U 
                                                                           == 
                                                                           (0xfe00707fU 
                                                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                           ? 1U
                                                                           : 
                                                                          ((0x40005033U 
                                                                            == 
                                                                            (0xfe00707fU 
                                                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                            ? 1U
                                                                            : 
                                                                           ((0x40005013U 
                                                                             == 
                                                                             (0xfc00707fU 
                                                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                             ? 1U
                                                                             : 
                                                                            ((0x5033U 
                                                                              == 
                                                                              (0xfe00707fU 
                                                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                              ? 1U
                                                                              : 
                                                                             ((0x5013U 
                                                                               == 
                                                                               (0xfc00707fU 
                                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                               ? 1U
                                                                               : 
                                                                              ((0x40000033U 
                                                                                == 
                                                                                (0xfe00707fU 
                                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                                ? 1U
                                                                                : 
                                                                               ((0x2023U 
                                                                                == 
                                                                                (0x707fU 
                                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                                 ? 1U
                                                                                 : 
                                                                                ((0x4033U 
                                                                                == 
                                                                                (0xfe00707fU 
                                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                                 ? 1U
                                                                                 : 
                                                                                ((0x4013U 
                                                                                == 
                                                                                (0x707fU 
                                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                                 ? 1U
                                                                                 : 0U))))))))))))))))))))))))))))))))))));
        vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__alu_operand2 
            = ((0x13U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                ? 4U : ((0x7033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                         ? 2U : ((0x7013U == (0x707fU 
                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                  ? 4U : ((0x17U == 
                                           (0x7fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                           ? 4U : (
                                                   (0x63U 
                                                    == 
                                                    (0x707fU 
                                                     & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                    ? 2U
                                                    : 
                                                   ((0x5063U 
                                                     == 
                                                     (0x707fU 
                                                      & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                     ? 2U
                                                     : 
                                                    ((0x7063U 
                                                      == 
                                                      (0x707fU 
                                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                      ? 2U
                                                      : 
                                                     ((0x4063U 
                                                       == 
                                                       (0x707fU 
                                                        & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                       ? 2U
                                                       : 
                                                      ((0x6063U 
                                                        == 
                                                        (0x707fU 
                                                         & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                        ? 2U
                                                        : 
                                                       ((0x1063U 
                                                         == 
                                                         (0x707fU 
                                                          & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                         ? 2U
                                                         : 
                                                        ((0x6fU 
                                                          == 
                                                          (0x7fU 
                                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                          ? 4U
                                                          : 
                                                         ((0x67U 
                                                           == 
                                                           (0x707fU 
                                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                           ? 4U
                                                           : 
                                                          ((3U 
                                                            == 
                                                            (0x707fU 
                                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                            ? 4U
                                                            : 
                                                           ((0x4003U 
                                                             == 
                                                             (0x707fU 
                                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                             ? 4U
                                                             : 
                                                            ((0x1003U 
                                                              == 
                                                              (0x707fU 
                                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                              ? 4U
                                                              : 
                                                             ((0x5003U 
                                                               == 
                                                               (0x707fU 
                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                               ? 4U
                                                               : 
                                                              ((0x37U 
                                                                == 
                                                                (0x7fU 
                                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                ? 0U
                                                                : 
                                                               ((0x2003U 
                                                                 == 
                                                                 (0x707fU 
                                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                 ? 4U
                                                                 : 
                                                                ((0x6033U 
                                                                  == 
                                                                  (0xfe00707fU 
                                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                  ? 2U
                                                                  : 
                                                                 ((0x6013U 
                                                                   == 
                                                                   (0x707fU 
                                                                    & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                   ? 4U
                                                                   : 
                                                                  ((0x23U 
                                                                    == 
                                                                    (0x707fU 
                                                                     & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                    ? 4U
                                                                    : 
                                                                   ((0x1023U 
                                                                     == 
                                                                     (0x707fU 
                                                                      & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                     ? 4U
                                                                     : 
                                                                    ((0x1033U 
                                                                      == 
                                                                      (0xfe00707fU 
                                                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                      ? 2U
                                                                      : 
                                                                     ((0x1013U 
                                                                       == 
                                                                       (0xfc00707fU 
                                                                        & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                       ? 4U
                                                                       : 
                                                                      ((0x2033U 
                                                                        == 
                                                                        (0xfe00707fU 
                                                                         & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                        ? 2U
                                                                        : 
                                                                       ((0x2013U 
                                                                         == 
                                                                         (0x707fU 
                                                                          & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                         ? 4U
                                                                         : 
                                                                        ((0x3013U 
                                                                          == 
                                                                          (0x707fU 
                                                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                          ? 4U
                                                                          : 
                                                                         ((0x3033U 
                                                                           == 
                                                                           (0xfe00707fU 
                                                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                           ? 2U
                                                                           : 
                                                                          ((0x40005033U 
                                                                            == 
                                                                            (0xfe00707fU 
                                                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                            ? 2U
                                                                            : 
                                                                           ((0x40005013U 
                                                                             == 
                                                                             (0xfc00707fU 
                                                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                             ? 4U
                                                                             : 
                                                                            ((0x5033U 
                                                                              == 
                                                                              (0xfe00707fU 
                                                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                              ? 2U
                                                                              : 
                                                                             ((0x5013U 
                                                                               == 
                                                                               (0xfc00707fU 
                                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                               ? 4U
                                                                               : 
                                                                              ((0x40000033U 
                                                                                == 
                                                                                (0xfe00707fU 
                                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                                ? 2U
                                                                                : 
                                                                               ((0x2023U 
                                                                                == 
                                                                                (0x707fU 
                                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                                 ? 4U
                                                                                 : 
                                                                                ((0x4033U 
                                                                                == 
                                                                                (0xfe00707fU 
                                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                                 ? 2U
                                                                                 : 
                                                                                ((0x4013U 
                                                                                == 
                                                                                (0x707fU 
                                                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                                                 ? 4U
                                                                                 : 0U))))))))))))))))))))))))))))))))))));
    }
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_ready 
        = ((IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_request) 
           & (5U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state)));
    if ((0x1fU >= (0x18U & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                            << 3U)))) {
        vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_byte 
            = (0xffU & (vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata 
                        >> (0x18U & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                     << 3U))));
        vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_half 
            = (0xffffU & (vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata 
                          >> (0x18U & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                       << 3U))));
    } else {
        vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_byte = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_half = 0U;
    }
    vlTOPp->SoC__DOT__cpu__DOT__bus_busy = ((IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pa_request) 
                                            | (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request));
    if (vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request) {
        vlTOPp->SoC__DOT__cpu_wdata = vlTOPp->SoC__DOT__cpu__DOT__bus_pb_wdata;
        vlTOPp->SoC__DOT__cpu_rw = ((IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rw) 
                                    & 1U);
    } else {
        vlTOPp->SoC__DOT__cpu_wdata = ((IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pa_request)
                                        ? vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__i_pa_wdata
                                        : 0U);
        vlTOPp->SoC__DOT__cpu_rw = 0U;
    }
    vlTOPp->SoC__DOT__cpu_request = ((IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pa_request) 
                                     | (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request));
    vlTOPp->SoC__DOT__cpu_address = ((IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request)
                                      ? vlTOPp->SoC__DOT__cpu__DOT__bus_pb_address
                                      : ((IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pa_request)
                                          ? vlTOPp->SoC__DOT__cpu__DOT__bus_pa_address
                                          : 0U));
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
        = ((0U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand1))
            ? 0U : ((1U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand1))
                     ? vlTOPp->SoC__DOT__cpu__DOT__fwd_rs1
                     : ((2U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand1))
                         ? vlTOPp->SoC__DOT__cpu__DOT__fwd_rs2
                         : ((3U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand1))
                             ? vlTOPp->SoC__DOT__cpu__DOT__decode_pc
                             : ((4U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand1))
                                 ? vlTOPp->SoC__DOT__cpu__DOT__decode_imm
                                 : 0U)))));
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2 
        = ((0U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand2))
            ? 0U : ((1U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand2))
                     ? vlTOPp->SoC__DOT__cpu__DOT__fwd_rs1
                     : ((2U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand2))
                         ? vlTOPp->SoC__DOT__cpu__DOT__fwd_rs2
                         : ((3U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand2))
                             ? vlTOPp->SoC__DOT__cpu__DOT__decode_pc
                             : ((4U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand2))
                                 ? vlTOPp->SoC__DOT__cpu__DOT__decode_imm
                                 : 0U)))));
    vlTOPp->SoC__DOT____Vcellinp__rom__i_request = 
        ((0x10000U > vlTOPp->SoC__DOT__cpu_address) 
         & (IData)(vlTOPp->SoC__DOT__cpu_request));
    vlTOPp->SoC__DOT__vram_select = ((0x40000000U <= vlTOPp->SoC__DOT__cpu_address) 
                                     & (0x50000000U 
                                        > vlTOPp->SoC__DOT__cpu_address));
    vlTOPp->SoC__DOT__led_select = ((0x50000000U <= vlTOPp->SoC__DOT__cpu_address) 
                                    & (0x50000010U 
                                       > vlTOPp->SoC__DOT__cpu_address));
    vlTOPp->SoC__DOT__timer_select = ((0x50000050U 
                                       <= vlTOPp->SoC__DOT__cpu_address) 
                                      & (0x50000060U 
                                         > vlTOPp->SoC__DOT__cpu_address));
    vlTOPp->SoC__DOT__ram_select = ((0x10000U <= vlTOPp->SoC__DOT__cpu_address) 
                                    & (0x20000U > vlTOPp->SoC__DOT__cpu_address));
    vlTOPp->SoC__DOT__sdram_select = ((0x20000000U 
                                       <= vlTOPp->SoC__DOT__cpu_address) 
                                      & (0x40000000U 
                                         > vlTOPp->SoC__DOT__cpu_address));
    vlTOPp->SoC__DOT__i2c_select = ((0x50000030U <= vlTOPp->SoC__DOT__cpu_address) 
                                    & (0x50000040U 
                                       > vlTOPp->SoC__DOT__cpu_address));
    vlTOPp->SoC__DOT__sd_select = ((0x50000040U <= vlTOPp->SoC__DOT__cpu_address) 
                                   & (0x50000050U > vlTOPp->SoC__DOT__cpu_address));
    vlTOPp->SoC__DOT__uart_select = ((0x50000010U <= vlTOPp->SoC__DOT__cpu_address) 
                                     & (0x50000020U 
                                        > vlTOPp->SoC__DOT__cpu_address));
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_lt_result 
        = (VL_LTS_III(1,32,32, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)
            ? 1U : 0U);
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__unsigned_lt_result 
        = ((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
            < vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)
            ? 1U : 0U);
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__equal_result 
        = ((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
            == vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)
            ? 1U : 0U);
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__not_equal_result 
        = ((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
            != vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)
            ? 1U : 0U);
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_get_result 
        = (VL_GTES_III(1,32,32, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)
            ? 1U : 0U);
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__unsigned_get_result 
        = ((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
            >= vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)
            ? 1U : 0U);
    vlTOPp->SoC__DOT____Vcellinp__video_bus__i_cpu_request 
        = ((IData)(vlTOPp->SoC__DOT__vram_select) & (IData)(vlTOPp->SoC__DOT__cpu_request));
    vlTOPp->SoC__DOT____Vcellinp__led__i_request = 
        ((IData)(vlTOPp->SoC__DOT__led_select) & (IData)(vlTOPp->SoC__DOT__cpu_request));
    vlTOPp->SoC__DOT____Vcellinp__ram__i_request = 
        ((IData)(vlTOPp->SoC__DOT__ram_select) & (IData)(vlTOPp->SoC__DOT__cpu_request));
    vlTOPp->SoC__DOT____Vcellinp__sdram__i_request 
        = ((IData)(vlTOPp->SoC__DOT__sdram_select) 
           & (IData)(vlTOPp->SoC__DOT__cpu_request));
    vlTOPp->SoC__DOT____Vcellinp__i2c__i_request = 
        ((IData)(vlTOPp->SoC__DOT__i2c_select) & (IData)(vlTOPp->SoC__DOT__cpu_request));
    vlTOPp->SoC__DOT____Vcellinp__sd__i_request = ((IData)(vlTOPp->SoC__DOT__sd_select) 
                                                   & (IData)(vlTOPp->SoC__DOT__cpu_request));
    vlTOPp->SoC__DOT__cpu_rdata = ((0x10000U > vlTOPp->SoC__DOT__cpu_address)
                                    ? vlTOPp->SoC__DOT__rom_rdata
                                    : ((IData)(vlTOPp->SoC__DOT__ram_select)
                                        ? vlTOPp->SoC__DOT__ram_rdata
                                        : ((IData)(vlTOPp->SoC__DOT__sdram_select)
                                            ? vlTOPp->SoC__DOT__sdram_rdata
                                            : ((IData)(vlTOPp->SoC__DOT__uart_select)
                                                ? vlTOPp->SoC__DOT__uart_rdata
                                                : ((IData)(vlTOPp->SoC__DOT__sd_select)
                                                    ? vlTOPp->SoC__DOT__sd_rdata
                                                    : 
                                                   ((IData)(vlTOPp->SoC__DOT__i2c_select)
                                                     ? vlTOPp->SoC__DOT__i2c_rdata
                                                     : 
                                                    ((IData)(vlTOPp->SoC__DOT__timer_select)
                                                      ? vlTOPp->SoC__DOT__timer_rdata
                                                      : 0U)))))));
    vlTOPp->SoC__DOT____Vcellinp__uart__i_request = 
        ((IData)(vlTOPp->SoC__DOT__uart_select) & (IData)(vlTOPp->SoC__DOT__cpu_request));
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_result 
        = ((1U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
            ? (vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
               + vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)
            : ((2U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                ? (vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                   + vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)
                : ((3U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                    ? (vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                       - vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)
                    : ((4U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                        ? (vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                           & vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)
                        : ((5U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                            ? (vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                               | vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)
                            : ((6U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                ? (vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                   ^ vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)
                                : ((7U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                    ? (vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                       << (0x1fU & vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2))
                                    : ((8U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                        ? (vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                           >> (0x1fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2))
                                        : ((9U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                            ? VL_SHIFTRS_III(32,32,5, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1, 
                                                             (0x1fU 
                                                              & vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2))
                                            : ((0xaU 
                                                == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                                ? (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_lt_result)
                                                : (
                                                   (0xbU 
                                                    == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                                    ? (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__unsigned_lt_result)
                                                    : 
                                                   ((0xcU 
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
                                                        : 0U)))))))))))))));
    vlTOPp->SoC__DOT__uart__DOT__rx_request = ((IData)(vlTOPp->SoC__DOT____Vcellinp__uart__i_request) 
                                               & (~ (IData)(vlTOPp->SoC__DOT__cpu_rw)));
    vlTOPp->SoC__DOT__uart__DOT__tx_request = ((IData)(vlTOPp->SoC__DOT____Vcellinp__uart__i_request) 
                                               & (IData)(vlTOPp->SoC__DOT__cpu_rw));
    vlTOPp->SoC__DOT__cpu_ready = ((0x10000U > vlTOPp->SoC__DOT__cpu_address)
                                    ? (IData)(vlTOPp->SoC__DOT__rom_ready)
                                    : ((IData)(vlTOPp->SoC__DOT__ram_select)
                                        ? (IData)(vlTOPp->SoC__DOT__ram_ready)
                                        : ((IData)(vlTOPp->SoC__DOT__sdram_select)
                                            ? (IData)(vlTOPp->SoC__DOT__sdram_ready)
                                            : ((IData)(vlTOPp->SoC__DOT__vram_select)
                                                ? (IData)(vlTOPp->SoC__DOT__vram_ready)
                                                : ((IData)(vlTOPp->SoC__DOT__led_select)
                                                    ? (IData)(vlTOPp->SoC__DOT__led_ready)
                                                    : 
                                                   ((IData)(vlTOPp->SoC__DOT__uart_select)
                                                     ? 
                                                    ((IData)(vlTOPp->SoC__DOT__uart__DOT__rx_request)
                                                      ? 
                                                     ((5U 
                                                       == (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds)) 
                                                      & (IData)(vlTOPp->SoC__DOT__uart__DOT__rx_request))
                                                      : 
                                                     ((IData)(vlTOPp->SoC__DOT__uart__DOT__tx_request) 
                                                      & (IData)(vlTOPp->SoC__DOT__uart__DOT__tx_ready)))
                                                     : 
                                                    ((IData)(vlTOPp->SoC__DOT__sd_select)
                                                      ? (IData)(vlTOPp->SoC__DOT__sd_ready)
                                                      : 
                                                     ((IData)(vlTOPp->SoC__DOT__i2c_select)
                                                       ? (IData)(vlTOPp->SoC__DOT__i2c_ready)
                                                       : (IData)(vlTOPp->SoC__DOT__timer_select)))))))));
    vlTOPp->SoC__DOT__cpu__DOT__bus_pb_ready = ((~ (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pa_request)) 
                                                & (IData)(vlTOPp->SoC__DOT__cpu_ready));
}

void VSoC::_eval_initial(VSoC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoC::_eval_initial\n"); );
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_initial__TOP__1(vlSymsp);
    vlTOPp->__Vm_traceActivity[4U] = 1U;
    vlTOPp->__Vm_traceActivity[3U] = 1U;
    vlTOPp->__Vm_traceActivity[2U] = 1U;
    vlTOPp->__Vm_traceActivity[1U] = 1U;
    vlTOPp->__Vm_traceActivity[0U] = 1U;
    vlTOPp->__Vclklast__TOP__CLOCK_125_p = vlTOPp->CLOCK_125_p;
    vlTOPp->__Vclklast__TOP____VinpClk__TOP__SoC__DOT__reset 
        = vlTOPp->__VinpClk__TOP__SoC__DOT__reset;
}

void VSoC::final() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoC::final\n"); );
    // Variables
    VSoC__Syms* __restrict vlSymsp = this->__VlSymsp;
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void VSoC::_eval_settle(VSoC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoC::_eval_settle\n"); );
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__4(vlSymsp);
    vlTOPp->__Vm_traceActivity[4U] = 1U;
    vlTOPp->__Vm_traceActivity[3U] = 1U;
    vlTOPp->__Vm_traceActivity[2U] = 1U;
    vlTOPp->__Vm_traceActivity[1U] = 1U;
    vlTOPp->__Vm_traceActivity[0U] = 1U;
}

void VSoC::_ctor_var_reset() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoC::_ctor_var_reset\n"); );
    // Body
    ADC_CONVST = VL_RAND_RESET_I(1);
    ADC_SCK = VL_RAND_RESET_I(1);
    ADC_SDI = VL_RAND_RESET_I(1);
    ADC_SDO = VL_RAND_RESET_I(1);
    AUD_ADCDAT = VL_RAND_RESET_I(1);
    AUD_ADCLRCK = VL_RAND_RESET_I(1);
    AUD_BCLK = VL_RAND_RESET_I(1);
    AUD_DACDAT = VL_RAND_RESET_I(1);
    AUD_DACLRCK = VL_RAND_RESET_I(1);
    AUD_XCK = VL_RAND_RESET_I(1);
    CLOCK_125_p = VL_RAND_RESET_I(1);
    CLOCK_50_B5B = VL_RAND_RESET_I(1);
    CLOCK_50_B6A = VL_RAND_RESET_I(1);
    CLOCK_50_B7A = VL_RAND_RESET_I(1);
    CLOCK_50_B8A = VL_RAND_RESET_I(1);
    CPU_RESET_n = VL_RAND_RESET_I(1);
    DDR2LP_CA = VL_RAND_RESET_I(10);
    DDR2LP_CKE = VL_RAND_RESET_I(2);
    DDR2LP_CK_n = VL_RAND_RESET_I(1);
    DDR2LP_CK_p = VL_RAND_RESET_I(1);
    DDR2LP_CS_n = VL_RAND_RESET_I(2);
    DDR2LP_DM = VL_RAND_RESET_I(4);
    DDR2LP_DQ = VL_RAND_RESET_I(32);
    DDR2LP_DQS_n = VL_RAND_RESET_I(4);
    DDR2LP_DQS_p = VL_RAND_RESET_I(4);
    DDR2LP_OCT_RZQ = VL_RAND_RESET_I(1);
    GPIO = VL_RAND_RESET_Q(36);
    HDMI_TX_CLK = VL_RAND_RESET_I(1);
    HDMI_TX_D = VL_RAND_RESET_I(24);
    HDMI_TX_DE = VL_RAND_RESET_I(1);
    HDMI_TX_HS = VL_RAND_RESET_I(1);
    HDMI_TX_INT = VL_RAND_RESET_I(1);
    HDMI_TX_VS = VL_RAND_RESET_I(1);
    HEX0 = VL_RAND_RESET_I(7);
    HEX1 = VL_RAND_RESET_I(7);
    HSMC_CLKIN0 = VL_RAND_RESET_I(1);
    HSMC_CLKIN_n = VL_RAND_RESET_I(2);
    HSMC_CLKIN_p = VL_RAND_RESET_I(2);
    HSMC_CLKOUT0 = VL_RAND_RESET_I(1);
    HSMC_CLKOUT_n = VL_RAND_RESET_I(2);
    HSMC_CLKOUT_p = VL_RAND_RESET_I(2);
    HSMC_D = VL_RAND_RESET_I(4);
    HSMC_RX_n = VL_RAND_RESET_I(17);
    HSMC_RX_p = VL_RAND_RESET_I(17);
    HSMC_TX_n = VL_RAND_RESET_I(17);
    HSMC_TX_p = VL_RAND_RESET_I(17);
    I2C_SCL = VL_RAND_RESET_I(1);
    I2C_SDA = VL_RAND_RESET_I(1);
    KEY = VL_RAND_RESET_I(4);
    LEDG = VL_RAND_RESET_I(8);
    LEDR = VL_RAND_RESET_I(10);
    SD_CLK = VL_RAND_RESET_I(1);
    SD_CMD = VL_RAND_RESET_I(1);
    SD_DAT = VL_RAND_RESET_I(4);
    SRAM_A = VL_RAND_RESET_I(18);
    SRAM_CE_n = VL_RAND_RESET_I(1);
    SRAM_D = VL_RAND_RESET_I(16);
    SRAM_LB_n = VL_RAND_RESET_I(1);
    SRAM_OE_n = VL_RAND_RESET_I(1);
    SRAM_UB_n = VL_RAND_RESET_I(1);
    SRAM_WE_n = VL_RAND_RESET_I(1);
    SW = VL_RAND_RESET_I(10);
    UART_RX = VL_RAND_RESET_I(1);
    UART_TX = VL_RAND_RESET_I(1);
    SoC__DOT__reset = VL_RAND_RESET_I(1);
    SoC__DOT__video_sram_request = VL_RAND_RESET_I(1);
    SoC__DOT__video_sram_rw = VL_RAND_RESET_I(1);
    SoC__DOT__video_sram_address = VL_RAND_RESET_I(32);
    SoC__DOT__video_sram_wdata = VL_RAND_RESET_I(32);
    SoC__DOT__video_sram_rdata = VL_RAND_RESET_I(32);
    SoC__DOT__video_sram_ready = VL_RAND_RESET_I(1);
    SoC__DOT__vram_select = VL_RAND_RESET_I(1);
    SoC__DOT__vram_ready = VL_RAND_RESET_I(1);
    SoC__DOT____Vcellout__video_bus__o_video_rdata = VL_RAND_RESET_I(32);
    SoC__DOT____Vcellinp__video_bus__i_cpu_request = VL_RAND_RESET_I(1);
    SoC__DOT__vga_enable = VL_RAND_RESET_I(1);
    SoC__DOT__vga_address = VL_RAND_RESET_I(16);
    SoC__DOT__rom_rdata = VL_RAND_RESET_I(32);
    SoC__DOT__rom_ready = VL_RAND_RESET_I(1);
    SoC__DOT____Vcellinp__rom__i_request = VL_RAND_RESET_I(1);
    SoC__DOT__ram_select = VL_RAND_RESET_I(1);
    SoC__DOT__ram_rdata = VL_RAND_RESET_I(32);
    SoC__DOT__ram_ready = VL_RAND_RESET_I(1);
    SoC__DOT____Vcellinp__ram__i_request = VL_RAND_RESET_I(1);
    SoC__DOT__sdram_select = VL_RAND_RESET_I(1);
    SoC__DOT__sdram_rdata = VL_RAND_RESET_I(32);
    SoC__DOT__sdram_ready = VL_RAND_RESET_I(1);
    SoC__DOT____Vcellinp__sdram__i_request = VL_RAND_RESET_I(1);
    SoC__DOT__led_select = VL_RAND_RESET_I(1);
    SoC__DOT__led_ready = VL_RAND_RESET_I(1);
    SoC__DOT____Vcellinp__led__i_request = VL_RAND_RESET_I(1);
    SoC__DOT__uart_select = VL_RAND_RESET_I(1);
    SoC__DOT__uart_rdata = VL_RAND_RESET_I(32);
    SoC__DOT____Vcellinp__uart__i_request = VL_RAND_RESET_I(1);
    SoC__DOT__i2c_select = VL_RAND_RESET_I(1);
    SoC__DOT__i2c_rdata = VL_RAND_RESET_I(32);
    SoC__DOT__i2c_ready = VL_RAND_RESET_I(1);
    SoC__DOT____Vcellinp__i2c__i_request = VL_RAND_RESET_I(1);
    SoC__DOT__sd_select = VL_RAND_RESET_I(1);
    SoC__DOT__sd_rdata = VL_RAND_RESET_I(32);
    SoC__DOT__sd_ready = VL_RAND_RESET_I(1);
    SoC__DOT____Vcellinp__sd__i_request = VL_RAND_RESET_I(1);
    SoC__DOT__timer_select = VL_RAND_RESET_I(1);
    SoC__DOT__timer_rdata = VL_RAND_RESET_I(32);
    SoC__DOT__cpu_rw = VL_RAND_RESET_I(1);
    SoC__DOT__cpu_request = VL_RAND_RESET_I(1);
    SoC__DOT__cpu_ready = VL_RAND_RESET_I(1);
    SoC__DOT__cpu_address = VL_RAND_RESET_I(32);
    SoC__DOT__cpu_rdata = VL_RAND_RESET_I(32);
    SoC__DOT__cpu_wdata = VL_RAND_RESET_I(32);
    SoC__DOT__cpu_retire_count = VL_RAND_RESET_I(32);
    { int __Vi0=0; for (; __Vi0<64000; ++__Vi0) {
            SoC__DOT__video_sram__DOT__data[__Vi0] = VL_RAND_RESET_I(32);
    }}
    SoC__DOT__video_sram__DOT____Vlvbound1 = VL_RAND_RESET_I(32);
    SoC__DOT__video_bus__DOT__fifo_write = VL_RAND_RESET_I(1);
    SoC__DOT__video_bus__DOT__fifo_wdata = VL_RAND_RESET_Q(64);
    SoC__DOT__video_bus__DOT__fifo_read = VL_RAND_RESET_I(1);
    SoC__DOT__video_bus__DOT__state = VL_RAND_RESET_I(3);
    SoC__DOT__video_bus__DOT__last_video_address = VL_RAND_RESET_I(1);
    SoC__DOT__video_bus__DOT__fifo__DOT__rdata = VL_RAND_RESET_Q(64);
    { int __Vi0=0; for (; __Vi0<1024; ++__Vi0) {
            SoC__DOT__video_bus__DOT__fifo__DOT__data[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    SoC__DOT__video_bus__DOT__fifo__DOT__in = VL_RAND_RESET_I(10);
    SoC__DOT__video_bus__DOT__fifo__DOT__out = VL_RAND_RESET_I(10);
    SoC__DOT__video_bus__DOT__fifo__DOT__last_write = VL_RAND_RESET_I(1);
    SoC__DOT__video_bus__DOT__fifo__DOT__last_read = VL_RAND_RESET_I(1);
    SoC__DOT__vga__DOT__prescale = VL_RAND_RESET_I(4);
    SoC__DOT__vga__DOT__vga_h = VL_RAND_RESET_I(10);
    SoC__DOT__vga__DOT__vga_v = VL_RAND_RESET_I(10);
    { int __Vi0=0; for (; __Vi0<401; ++__Vi0) {
            SoC__DOT__rom__DOT__data[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<16384; ++__Vi0) {
            SoC__DOT__ram__DOT__data[__Vi0] = VL_RAND_RESET_I(32);
    }}
    { int __Vi0=0; for (; __Vi0<33554432; ++__Vi0) {
            SoC__DOT__sdram__DOT__data[__Vi0] = VL_RAND_RESET_I(32);
    }}
    SoC__DOT__led__DOT__leds = VL_RAND_RESET_I(10);
    SoC__DOT__uart__DOT__rx_request = VL_RAND_RESET_I(1);
    SoC__DOT__uart__DOT__tx_request = VL_RAND_RESET_I(1);
    SoC__DOT__uart__DOT__tx_ready = VL_RAND_RESET_I(1);
    SoC__DOT__uart__DOT__rx__DOT__frame_error = VL_RAND_RESET_I(1);
    SoC__DOT__uart__DOT__rx__DOT__prescale = VL_RAND_RESET_I(19);
    SoC__DOT__uart__DOT__rx__DOT__data = VL_RAND_RESET_I(8);
    SoC__DOT__uart__DOT__rx__DOT__bidx = VL_RAND_RESET_I(4);
    SoC__DOT__uart__DOT__rx__DOT__rds = VL_RAND_RESET_I(4);
    SoC__DOT__uart__DOT__rx__DOT__rx = VL_RAND_RESET_I(1);
    SoC__DOT__uart__DOT__rx__DOT__rx_fifo_empty = VL_RAND_RESET_I(1);
    SoC__DOT__uart__DOT__rx__DOT__rx_fifo_write = VL_RAND_RESET_I(1);
    SoC__DOT__uart__DOT__rx__DOT__rx_fifo_read = VL_RAND_RESET_I(1);
    SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__rdata = VL_RAND_RESET_I(8);
    { int __Vi0=0; for (; __Vi0<4; ++__Vi0) {
            SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data[__Vi0] = VL_RAND_RESET_I(8);
    }}
    SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__in = VL_RAND_RESET_I(2);
    SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__out = VL_RAND_RESET_I(2);
    SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__last_write = VL_RAND_RESET_I(1);
    SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__last_read = VL_RAND_RESET_I(1);
    SoC__DOT__uart__DOT__tx__DOT__prescale = VL_RAND_RESET_I(19);
    SoC__DOT__uart__DOT__tx__DOT__data = VL_RAND_RESET_I(9);
    SoC__DOT__uart__DOT__tx__DOT__bidx = VL_RAND_RESET_I(4);
    SoC__DOT__i2c__DOT__scl = VL_RAND_RESET_I(1);
    SoC__DOT__i2c__DOT__sda = VL_RAND_RESET_I(1);
    SoC__DOT__sd__DOT__clk = VL_RAND_RESET_I(1);
    SoC__DOT__sd__DOT__cdir = VL_RAND_RESET_I(1);
    SoC__DOT__sd__DOT__ddir = VL_RAND_RESET_I(1);
    SoC__DOT__sd__DOT__cmd = VL_RAND_RESET_I(1);
    SoC__DOT__sd__DOT__dat = VL_RAND_RESET_I(4);
    SoC__DOT__timer__DOT__prescale = VL_RAND_RESET_I(17);
    SoC__DOT__timer__DOT__cycles = VL_RAND_RESET_I(32);
    SoC__DOT__timer__DOT__ms = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__bus_busy = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__bus_pa_request = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__bus_pa_address = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__bus_pb_rw = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__bus_pb_request = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__bus_pb_ready = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__bus_pb_address = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__bus_pb_wdata = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__rs1 = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__rs2 = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__fetch_tag = VL_RAND_RESET_I(4);
    SoC__DOT__cpu__DOT__fetch_instruction = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__fetch_pc = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__decode_tag = VL_RAND_RESET_I(4);
    SoC__DOT__cpu__DOT__decode_instruction = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__decode_pc = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__decode_inst_rs1 = VL_RAND_RESET_I(5);
    SoC__DOT__cpu__DOT__decode_inst_rs2 = VL_RAND_RESET_I(5);
    SoC__DOT__cpu__DOT__decode_inst_rd = VL_RAND_RESET_I(5);
    SoC__DOT__cpu__DOT__decode_imm = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__decode_arithmetic = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__decode_jump = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__decode_jump_conditional = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__decode_alu_operation = VL_RAND_RESET_I(4);
    SoC__DOT__cpu__DOT__decode_alu_operand1 = VL_RAND_RESET_I(3);
    SoC__DOT__cpu__DOT__decode_alu_operand2 = VL_RAND_RESET_I(3);
    SoC__DOT__cpu__DOT__decode_memory_read = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__decode_memory_write = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__decode_memory_width = VL_RAND_RESET_I(3);
    SoC__DOT__cpu__DOT__decode_memory_signed = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__decode_op = VL_RAND_RESET_I(5);
    SoC__DOT__cpu__DOT__fwd_rs1 = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__fwd_rs2 = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__execute_tag = VL_RAND_RESET_I(4);
    SoC__DOT__cpu__DOT__execute_inst_rd = VL_RAND_RESET_I(5);
    SoC__DOT__cpu__DOT__execute_rd = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__execute_pc_next = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__execute_mem_read = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__execute_mem_write = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__execute_mem_width = VL_RAND_RESET_I(3);
    SoC__DOT__cpu__DOT__execute_mem_signed = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__execute_mem_address = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__execute_mem_wdata = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__execute_stall = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__memory_tag = VL_RAND_RESET_I(4);
    SoC__DOT__cpu__DOT__memory_inst_rd = VL_RAND_RESET_I(5);
    SoC__DOT__cpu__DOT__memory_rd = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__memory_pc_next = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__memory_stall = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__writeback_tag = VL_RAND_RESET_I(4);
    SoC__DOT__cpu__DOT__writeback_inst_rd = VL_RAND_RESET_I(5);
    SoC__DOT__cpu__DOT__writeback_rd = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__writeback_pc_next = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__retire_tag = VL_RAND_RESET_I(4);
    SoC__DOT__cpu__DOT__bus__DOT__i_pa_wdata = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__registers__DOT__read_tag = VL_RAND_RESET_I(4);
    SoC__DOT__cpu__DOT__registers__DOT__write_tag = VL_RAND_RESET_I(4);
    { int __Vi0=0; for (; __Vi0<32; ++__Vi0) {
            SoC__DOT__cpu__DOT__registers__DOT__r[__Vi0] = VL_RAND_RESET_I(32);
    }}
    SoC__DOT__cpu__DOT__fetch__DOT__state = VL_RAND_RESET_I(3);
    SoC__DOT__cpu__DOT__fetch__DOT__pc = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__fetch__DOT__icache_output_pc = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__fetch__DOT__is_R = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__fetch__DOT__is_ARITHMETIC = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state = VL_RAND_RESET_I(4);
    VL_RAND_RESET_W(1024, SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid);
    SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_wdata = VL_RAND_RESET_Q(64);
    SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rdata = VL_RAND_RESET_Q(64);
    SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_input_address = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__o_ready = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<1024; ++__Vi0) {
            SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state = VL_RAND_RESET_I(4);
    SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__address = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__prefetch = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__decode__DOT__is_B = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__decode__DOT__is_I = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__decode__DOT__is_R = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__decode__DOT__is_S = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__decode__DOT__is_U = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__decode__DOT__is_ARITHMETIC = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__decode__DOT__alu_operation = VL_RAND_RESET_I(4);
    SoC__DOT__cpu__DOT__decode__DOT__alu_operand1 = VL_RAND_RESET_I(3);
    SoC__DOT__cpu__DOT__decode__DOT__alu_operand2 = VL_RAND_RESET_I(3);
    SoC__DOT__cpu__DOT__execute__DOT__cycle = VL_RAND_RESET_I(5);
    SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__execute__DOT__alu_operand2 = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__execute__DOT__alu_result = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__execute__DOT__mul_signed = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__execute__DOT__mul_op1 = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__execute__DOT__mul_op2 = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__execute__DOT__div_signed = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__execute__DOT__div_numerator = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__execute__DOT__div_denominator = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_lt_result = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__unsigned_lt_result = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__equal_result = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__not_equal_result = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_get_result = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__unsigned_get_result = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p1 = VL_RAND_RESET_Q(64);
    SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p2 = VL_RAND_RESET_Q(64);
    SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p1_result = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p1_remainder = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p2_result = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p2_remainder = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__memory__DOT__state = VL_RAND_RESET_I(5);
    SoC__DOT__cpu__DOT__memory__DOT__dcache_rw = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__memory__DOT__dcache_request = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__memory__DOT__dcache_ready = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__memory__DOT__dcache_wdata = VL_RAND_RESET_I(32);
    SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_byte = VL_RAND_RESET_I(8);
    SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_half = VL_RAND_RESET_I(16);
    SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state = VL_RAND_RESET_I(8);
    VL_RAND_RESET_W(256, SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid);
    VL_RAND_RESET_W(256, SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty);
    SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw = VL_RAND_RESET_I(1);
    SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_wdata = VL_RAND_RESET_Q(64);
    SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rdata = VL_RAND_RESET_Q(64);
    SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__o_ready = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<256; ++__Vi0) {
            SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__data[__Vi0] = VL_RAND_RESET_Q(64);
    }}
    __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__state = VL_RAND_RESET_I(3);
    __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__pc = VL_RAND_RESET_I(32);
    __Vdly__SoC__DOT__cpu__DOT__fetch_tag = VL_RAND_RESET_I(4);
    __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache_output_pc = VL_RAND_RESET_I(32);
    __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_input_address = VL_RAND_RESET_I(32);
    __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state = VL_RAND_RESET_I(4);
    VL_RAND_RESET_W(1024, __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid);
    __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw = VL_RAND_RESET_I(1);
    __Vdlyvdim0__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data__v0 = 0;
    __Vdlyvval__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data__v0 = VL_RAND_RESET_Q(64);
    __Vdlyvset__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data__v0 = 0;
    __Vdly__SoC__DOT__cpu__DOT__bus_pa_address = VL_RAND_RESET_I(32);
    __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state = VL_RAND_RESET_I(4);
    __Vdly__SoC__DOT__cpu__DOT__decode_tag = VL_RAND_RESET_I(4);
    __Vdly__SoC__DOT__cpu__DOT__execute_tag = VL_RAND_RESET_I(4);
    __Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle = VL_RAND_RESET_I(5);
    __Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_signed = VL_RAND_RESET_I(1);
    __Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_numerator = VL_RAND_RESET_I(32);
    __Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_denominator = VL_RAND_RESET_I(32);
    __Vdly__SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p1 = VL_RAND_RESET_Q(64);
    __Vdly__SoC__DOT__cpu__DOT__memory__DOT__state = VL_RAND_RESET_I(5);
    __Vdly__SoC__DOT__cpu__DOT__memory_tag = VL_RAND_RESET_I(4);
    __Vdly__SoC__DOT__cpu__DOT__bus_pb_request = VL_RAND_RESET_I(1);
    __Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata = VL_RAND_RESET_I(32);
    __Vdly__SoC__DOT__cpu__DOT__writeback_tag = VL_RAND_RESET_I(4);
    __Vdly__SoC__DOT__cpu__DOT__writeback_pc_next = VL_RAND_RESET_I(32);
    __VinpClk__TOP__SoC__DOT__reset = VL_RAND_RESET_I(1);
    __Vchglast__TOP__SoC__DOT__reset = VL_RAND_RESET_I(1);
    { int __Vi0=0; for (; __Vi0<5; ++__Vi0) {
            __Vm_traceActivity[__Vi0] = VL_RAND_RESET_I(1);
    }}
}
