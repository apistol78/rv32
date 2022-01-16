// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VSoC.h for the primary calling header

#include "VSoC.h"
#include "VSoC__Syms.h"

//==========

void VSoC::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VSoC::eval\n"); );
    VSoC__Syms* __restrict vlSymsp = this->__VlSymsp;  // Setup global symbol table
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
#ifdef VL_DEBUG
    // Debug assertions
    _eval_debug_assertions();
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        vlSymsp->__Vm_activity = true;
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("code/Rv32H/SoC.v", 10, "",
                "Verilated model didn't converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

void VSoC::_eval_initial_loop(VSoC__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    vlSymsp->__Vm_activity = true;
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        _eval_settle(vlSymsp);
        _eval(vlSymsp);
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = _change_request(vlSymsp);
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("code/Rv32H/SoC.v", 10, "",
                "Verilated model didn't DC converge\n"
                "- See DIDNOTCONVERGE in the Verilator manual");
        } else {
            __Vchange = _change_request(vlSymsp);
        }
    } while (VL_UNLIKELY(__Vchange));
}

VL_INLINE_OPT void VSoC::_sequent__TOP__2(VSoC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoC::_sequent__TOP__2\n"); );
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*0:0*/ __Vdlyvset__SoC__DOT__video_sram__DOT__data__v0;
    CData/*0:0*/ __Vdly__SoC__DOT__video_sram_ready;
    CData/*2:0*/ __Vdly__SoC__DOT__video_bus__DOT__state;
    CData/*0:0*/ __Vdly__SoC__DOT__video_bus__DOT__fifo_read;
    CData/*0:0*/ __Vdlyvset__SoC__DOT__video_bus__DOT__fifo__DOT__data__v0;
    CData/*3:0*/ __Vdly__SoC__DOT__vga__DOT__prescale;
    CData/*0:0*/ __Vdlyvset__SoC__DOT__ram__DOT__data__v0;
    CData/*0:0*/ __Vdlyvset__SoC__DOT__sdram__DOT__data__v0;
    CData/*3:0*/ __Vdly__SoC__DOT__uart__DOT__rx__DOT__rds;
    CData/*0:0*/ __Vdly__SoC__DOT__uart__DOT__rx__DOT__rx_fifo_read;
    CData/*3:0*/ __Vdly__SoC__DOT__uart__DOT__rx__DOT__bidx;
    CData/*1:0*/ __Vdlyvdim0__SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data__v0;
    CData/*7:0*/ __Vdlyvval__SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data__v0;
    CData/*0:0*/ __Vdlyvset__SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data__v0;
    CData/*3:0*/ __Vdly__SoC__DOT__uart__DOT__tx__DOT__bidx;
    CData/*0:0*/ __Vdly__SoC__DOT__i2c__DOT__scl;
    CData/*0:0*/ __Vdly__SoC__DOT__sd__DOT__clk;
    CData/*0:0*/ __Vdly__SoC__DOT__sd__DOT__cdir;
    CData/*0:0*/ __Vdly__SoC__DOT__sd__DOT__ddir;
    CData/*0:0*/ __Vdly__SoC__DOT__sd__DOT__cmd;
    CData/*3:0*/ __Vdly__SoC__DOT__sd__DOT__dat;
    CData/*3:0*/ __Vdly__SoC__DOT__cpu__DOT__retire_tag;
    SData/*15:0*/ __Vdlyvdim0__SoC__DOT__video_sram__DOT__data__v0;
    SData/*9:0*/ __Vdlyvdim0__SoC__DOT__video_bus__DOT__fifo__DOT__data__v0;
    SData/*9:0*/ __Vdly__SoC__DOT__video_bus__DOT__fifo__DOT__in;
    SData/*9:0*/ __Vdly__SoC__DOT__vga__DOT__vga_h;
    SData/*9:0*/ __Vdly__SoC__DOT__vga__DOT__vga_v;
    SData/*13:0*/ __Vdlyvdim0__SoC__DOT__ram__DOT__data__v0;
    SData/*8:0*/ __Vdly__SoC__DOT__uart__DOT__tx__DOT__data;
    IData/*31:0*/ __Vdlyvval__SoC__DOT__video_sram__DOT__data__v0;
    IData/*31:0*/ __Vdly__SoC__DOT__video_sram_rdata;
    IData/*31:0*/ __Vdlyvval__SoC__DOT__ram__DOT__data__v0;
    IData/*24:0*/ __Vdlyvdim0__SoC__DOT__sdram__DOT__data__v0;
    IData/*31:0*/ __Vdlyvval__SoC__DOT__sdram__DOT__data__v0;
    IData/*31:0*/ __Vdly__SoC__DOT__timer__DOT__cycles;
    IData/*31:0*/ __Vdly__SoC__DOT__timer__DOT__ms;
    IData/*16:0*/ __Vdly__SoC__DOT__timer__DOT__prescale;
    IData/*31:0*/ __Vdly__SoC__DOT__cpu_retire_count;
    QData/*63:0*/ __Vdlyvval__SoC__DOT__video_bus__DOT__fifo__DOT__data__v0;
    // Body
    __Vdly__SoC__DOT__timer__DOT__cycles = vlTOPp->SoC__DOT__timer__DOT__cycles;
    __Vdly__SoC__DOT__timer__DOT__prescale = vlTOPp->SoC__DOT__timer__DOT__prescale;
    __Vdly__SoC__DOT__timer__DOT__ms = vlTOPp->SoC__DOT__timer__DOT__ms;
    __Vdly__SoC__DOT__vga__DOT__vga_v = vlTOPp->SoC__DOT__vga__DOT__vga_v;
    __Vdly__SoC__DOT__vga__DOT__vga_h = vlTOPp->SoC__DOT__vga__DOT__vga_h;
    __Vdly__SoC__DOT__vga__DOT__prescale = vlTOPp->SoC__DOT__vga__DOT__prescale;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__bus_pa_address 
        = vlTOPp->SoC__DOT__cpu__DOT__bus_pa_address;
    __Vdly__SoC__DOT__uart__DOT__rx__DOT__bidx = vlTOPp->SoC__DOT__uart__DOT__rx__DOT__bidx;
    __Vdly__SoC__DOT__video_bus__DOT__fifo_read = vlTOPp->SoC__DOT__video_bus__DOT__fifo_read;
    __Vdly__SoC__DOT__video_bus__DOT__state = vlTOPp->SoC__DOT__video_bus__DOT__state;
    __Vdly__SoC__DOT__video_sram_ready = vlTOPp->SoC__DOT__video_sram_ready;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[1U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[1U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[2U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[2U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[3U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[3U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[4U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[4U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[5U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[5U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[6U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[6U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[7U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[7U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[8U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[8U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[9U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[9U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xaU] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xaU];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xbU] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xbU];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xcU] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xcU];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xdU] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xdU];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xeU] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xeU];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xfU] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xfU];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x10U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x10U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x11U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x11U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x12U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x12U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x13U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x13U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x14U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x14U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x15U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x15U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x16U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x16U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x17U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x17U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x18U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x18U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x19U] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x19U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1aU] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1aU];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1bU] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1bU];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1cU] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1cU];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1dU] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1dU];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1eU] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1eU];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1fU] 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1fU];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_request 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_request;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache_output_tag 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_output_tag;
    vlTOPp->__Vdlyvset__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data__v0 = 0U;
    __Vdlyvset__SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data__v0 = 0U;
    __Vdly__SoC__DOT__video_sram_rdata = vlTOPp->SoC__DOT__video_sram_rdata;
    __Vdlyvset__SoC__DOT__video_sram__DOT__data__v0 = 0U;
    __Vdly__SoC__DOT__cpu_retire_count = vlTOPp->SoC__DOT__cpu_retire_count;
    __Vdly__SoC__DOT__cpu__DOT__retire_tag = vlTOPp->SoC__DOT__cpu__DOT__retire_tag;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__writeback_tag 
        = vlTOPp->SoC__DOT__cpu__DOT__writeback_tag;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch_tag = vlTOPp->SoC__DOT__cpu__DOT__fetch_tag;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__pc 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__state 
        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__state;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__decode_tag 
        = vlTOPp->SoC__DOT__cpu__DOT__decode_tag;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_denominator 
        = vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_denominator;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_numerator 
        = vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_numerator;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_signed 
        = vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_signed;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle 
        = vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute_tag 
        = vlTOPp->SoC__DOT__cpu__DOT__execute_tag;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p1 
        = vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p1;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_pc_next 
        = vlTOPp->SoC__DOT__cpu__DOT__memory_pc_next;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__state 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__state;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_tag 
        = vlTOPp->SoC__DOT__cpu__DOT__memory_tag;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_rd = vlTOPp->SoC__DOT__cpu__DOT__memory_rd;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_inst_rd 
        = vlTOPp->SoC__DOT__cpu__DOT__memory_inst_rd;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[0U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[0U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[1U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[1U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[2U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[2U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[3U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[3U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[4U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[4U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[5U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[5U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[6U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[6U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[7U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[7U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[0U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[0U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[1U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[1U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[2U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[2U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[3U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[3U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[4U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[4U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[5U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[5U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[6U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[6U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[7U] 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[7U];
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata 
        = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata;
    vlTOPp->__Vdlyvset__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__data__v0 = 0U;
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__bus__DOT__state 
        = vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state;
    __Vdly__SoC__DOT__uart__DOT__tx__DOT__data = vlTOPp->SoC__DOT__uart__DOT__tx__DOT__data;
    __Vdly__SoC__DOT__uart__DOT__tx__DOT__bidx = vlTOPp->SoC__DOT__uart__DOT__tx__DOT__bidx;
    __Vdly__SoC__DOT__uart__DOT__rx__DOT__rx_fifo_read 
        = vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_read;
    __Vdly__SoC__DOT__uart__DOT__rx__DOT__rds = vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds;
    __Vdly__SoC__DOT__i2c__DOT__scl = vlTOPp->SoC__DOT__i2c__DOT__scl;
    __Vdly__SoC__DOT__sd__DOT__clk = vlTOPp->SoC__DOT__sd__DOT__clk;
    __Vdly__SoC__DOT__sd__DOT__dat = vlTOPp->SoC__DOT__sd__DOT__dat;
    __Vdly__SoC__DOT__sd__DOT__cmd = vlTOPp->SoC__DOT__sd__DOT__cmd;
    __Vdly__SoC__DOT__sd__DOT__ddir = vlTOPp->SoC__DOT__sd__DOT__ddir;
    __Vdly__SoC__DOT__sd__DOT__cdir = vlTOPp->SoC__DOT__sd__DOT__cdir;
    __Vdly__SoC__DOT__video_bus__DOT__fifo__DOT__in 
        = vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__in;
    __Vdlyvset__SoC__DOT__video_bus__DOT__fifo__DOT__data__v0 = 0U;
    __Vdlyvset__SoC__DOT__sdram__DOT__data__v0 = 0U;
    __Vdlyvset__SoC__DOT__ram__DOT__data__v0 = 0U;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__o_ready = 1U;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__o_ready = 1U;
    __Vdly__SoC__DOT__timer__DOT__cycles = ((IData)(vlTOPp->SoC__DOT__reset)
                                             ? 0U : 
                                            ((IData)(1U) 
                                             + vlTOPp->SoC__DOT__timer__DOT__cycles));
    if (vlTOPp->SoC__DOT__reset) {
        __Vdly__SoC__DOT__timer__DOT__ms = 0U;
        __Vdly__SoC__DOT__timer__DOT__prescale = 0U;
    } else {
        __Vdly__SoC__DOT__timer__DOT__prescale = (0x1ffffU 
                                                  & ((IData)(1U) 
                                                     + vlTOPp->SoC__DOT__timer__DOT__prescale));
        if ((0x186a0U <= vlTOPp->SoC__DOT__timer__DOT__prescale)) {
            __Vdly__SoC__DOT__timer__DOT__ms = ((IData)(1U) 
                                                + vlTOPp->SoC__DOT__timer__DOT__ms);
            __Vdly__SoC__DOT__timer__DOT__prescale = 0U;
        }
    }
    __Vdly__SoC__DOT__vga__DOT__prescale = (0xfU & 
                                            ((IData)(1U) 
                                             + (IData)(vlTOPp->SoC__DOT__vga__DOT__prescale)));
    if ((3U <= (IData)(vlTOPp->SoC__DOT__vga__DOT__prescale))) {
        if (((0U == (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_h)) 
             & (0x20cU != (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_v)))) {
            __Vdly__SoC__DOT__vga__DOT__vga_h = (0x3ffU 
                                                 & ((IData)(1U) 
                                                    + (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_h)));
            __Vdly__SoC__DOT__vga__DOT__vga_v = (0x3ffU 
                                                 & ((IData)(1U) 
                                                    + (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_v)));
        } else {
            if (((0U == (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_h)) 
                 & (0x20cU == (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_v)))) {
                __Vdly__SoC__DOT__vga__DOT__vga_h = 
                    (0x3ffU & ((IData)(1U) + (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_h)));
                __Vdly__SoC__DOT__vga__DOT__vga_v = 0U;
            } else {
                if ((0x280U >= (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_h))) {
                    __Vdly__SoC__DOT__vga__DOT__vga_h 
                        = (0x3ffU & ((IData)(1U) + (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_h)));
                } else {
                    if ((0x290U >= (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_h))) {
                        __Vdly__SoC__DOT__vga__DOT__vga_h 
                            = (0x3ffU & ((IData)(1U) 
                                         + (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_h)));
                    } else {
                        if ((0x2f0U >= (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_h))) {
                            __Vdly__SoC__DOT__vga__DOT__vga_h 
                                = (0x3ffU & ((IData)(1U) 
                                             + (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_h)));
                            vlTOPp->HDMI_TX_HS = 0U;
                        } else {
                            if ((0x31fU > (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_h))) {
                                __Vdly__SoC__DOT__vga__DOT__vga_h 
                                    = (0x3ffU & ((IData)(1U) 
                                                 + (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_h)));
                                vlTOPp->HDMI_TX_HS = 1U;
                            } else {
                                __Vdly__SoC__DOT__vga__DOT__vga_h = 0U;
                            }
                        }
                    }
                }
            }
        }
        __Vdly__SoC__DOT__vga__DOT__prescale = 0U;
        vlTOPp->HDMI_TX_VS = (1U & (~ ((0x1ebU == (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_v)) 
                                       | (0x1ecU == (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_v)))));
    }
    vlTOPp->HDMI_TX_CLK = (1U & (~ ((IData)(vlTOPp->SoC__DOT__vga__DOT__prescale) 
                                    >> 1U)));
    vlTOPp->SoC__DOT__sd_ready = vlTOPp->SoC__DOT____Vcellinp__sd__i_request;
    vlTOPp->SoC__DOT__i2c_ready = vlTOPp->SoC__DOT____Vcellinp__i2c__i_request;
    vlTOPp->SoC__DOT__led_ready = vlTOPp->SoC__DOT____Vcellinp__led__i_request;
    vlTOPp->SoC__DOT__rom_ready = vlTOPp->SoC__DOT____Vcellinp__rom__i_request;
    vlTOPp->SoC__DOT__sdram_ready = vlTOPp->SoC__DOT____Vcellinp__sdram__i_request;
    vlTOPp->SoC__DOT__ram_ready = vlTOPp->SoC__DOT____Vcellinp__ram__i_request;
    __Vdly__SoC__DOT__video_sram_ready = vlTOPp->SoC__DOT__video_sram_request;
    if (vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw) {
        vlTOPp->__Vdlyvval__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data__v0 
            = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_wdata;
        vlTOPp->__Vdlyvset__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data__v0 = 1U;
        vlTOPp->__Vdlyvdim0__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data__v0 
            = (0xffU & (vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc 
                        >> 2U));
    }
    if (((IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_write) 
         & (~ (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__last_write)))) {
        __Vdlyvval__SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data__v0 
            = vlTOPp->SoC__DOT__uart__DOT__rx__DOT__data;
        __Vdlyvset__SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data__v0 = 1U;
        __Vdlyvdim0__SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data__v0 
            = vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__in;
        vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__in 
            = (3U & ((IData)(1U) + (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__in)));
    }
    if (vlTOPp->SoC__DOT__video_sram_request) {
        if ((1U & (~ (IData)(vlTOPp->SoC__DOT__video_sram_rw)))) {
            __Vdly__SoC__DOT__video_sram_rdata = ((0xf9ffU 
                                                   >= 
                                                   (0xffffU 
                                                    & (vlTOPp->SoC__DOT__video_sram_address 
                                                       >> 2U)))
                                                   ? 
                                                  vlTOPp->SoC__DOT__video_sram__DOT__data
                                                  [
                                                  (0xffffU 
                                                   & (vlTOPp->SoC__DOT__video_sram_address 
                                                      >> 2U))]
                                                   : 0U);
        }
    }
    if (vlTOPp->SoC__DOT__video_sram_request) {
        if (vlTOPp->SoC__DOT__video_sram_rw) {
            vlTOPp->SoC__DOT__video_sram__DOT____Vlvbound1 
                = vlTOPp->SoC__DOT__video_sram_wdata;
            if ((0xf9ffU >= (0xffffU & (vlTOPp->SoC__DOT__video_sram_address 
                                        >> 2U)))) {
                __Vdlyvval__SoC__DOT__video_sram__DOT__data__v0 
                    = vlTOPp->SoC__DOT__video_sram__DOT____Vlvbound1;
                __Vdlyvset__SoC__DOT__video_sram__DOT__data__v0 = 1U;
                __Vdlyvdim0__SoC__DOT__video_sram__DOT__data__v0 
                    = (0xffffU & (vlTOPp->SoC__DOT__video_sram_address 
                                  >> 2U));
            }
        }
    }
    if (vlTOPp->SoC__DOT__reset) {
        __Vdly__SoC__DOT__cpu__DOT__retire_tag = 0U;
        __Vdly__SoC__DOT__cpu_retire_count = 0U;
    } else {
        if (((IData)(vlTOPp->SoC__DOT__cpu__DOT__memory_tag) 
             != (IData)(vlTOPp->SoC__DOT__cpu__DOT__retire_tag))) {
            __Vdly__SoC__DOT__cpu_retire_count = ((IData)(1U) 
                                                  + vlTOPp->SoC__DOT__cpu_retire_count);
            __Vdly__SoC__DOT__cpu__DOT__retire_tag 
                = vlTOPp->SoC__DOT__cpu__DOT__memory_tag;
        }
    }
    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p1 
        = ((IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_signed)
            ? VL_MULS_QQQ(64,64,64, VL_EXTENDS_QI(64,32, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_op1), 
                          VL_EXTENDS_QI(64,32, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_op2))
            : ((QData)((IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_op1)) 
               * (QData)((IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_op2))));
    if (vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw) {
        vlTOPp->__Vdlyvval__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__data__v0 
            = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_wdata;
        vlTOPp->__Vdlyvset__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__data__v0 = 1U;
        vlTOPp->__Vdlyvdim0__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__data__v0 
            = (0xffU & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                        >> 2U));
    }
    if ((0U < vlTOPp->SoC__DOT__uart__DOT__tx__DOT__prescale)) {
        vlTOPp->SoC__DOT__uart__DOT__tx__DOT__prescale 
            = (0x7ffffU & (vlTOPp->SoC__DOT__uart__DOT__tx__DOT__prescale 
                           - (IData)(1U)));
    } else {
        if (vlTOPp->SoC__DOT__uart__DOT__tx_request) {
            if ((0U == (IData)(vlTOPp->SoC__DOT__uart__DOT__tx__DOT__bidx))) {
                vlTOPp->SoC__DOT__uart__DOT__tx__DOT__prescale = 0x35fU;
                __Vdly__SoC__DOT__uart__DOT__tx__DOT__bidx = 9U;
                __Vdly__SoC__DOT__uart__DOT__tx__DOT__data 
                    = (0x1ffU & vlTOPp->SoC__DOT__cpu_wdata);
                vlTOPp->UART_TX = 0U;
            } else {
                if ((1U < (IData)(vlTOPp->SoC__DOT__uart__DOT__tx__DOT__bidx))) {
                    __Vdly__SoC__DOT__uart__DOT__tx__DOT__bidx 
                        = (0xfU & ((IData)(vlTOPp->SoC__DOT__uart__DOT__tx__DOT__bidx) 
                                   - (IData)(1U)));
                    __Vdly__SoC__DOT__uart__DOT__tx__DOT__data 
                        = (0x1ffU & ((IData)(vlTOPp->SoC__DOT__uart__DOT__tx__DOT__data) 
                                     >> 1U));
                    vlTOPp->SoC__DOT__uart__DOT__tx__DOT__prescale = 0x35fU;
                    vlTOPp->UART_TX = (1U & (IData)(vlTOPp->SoC__DOT__uart__DOT__tx__DOT__data));
                } else {
                    if ((1U == (IData)(vlTOPp->SoC__DOT__uart__DOT__tx__DOT__bidx))) {
                        __Vdly__SoC__DOT__uart__DOT__tx__DOT__bidx 
                            = (0xfU & ((IData)(vlTOPp->SoC__DOT__uart__DOT__tx__DOT__bidx) 
                                       - (IData)(1U)));
                        vlTOPp->SoC__DOT__uart__DOT__tx__DOT__prescale = 0x360U;
                        vlTOPp->UART_TX = 1U;
                        vlTOPp->SoC__DOT__uart__DOT__tx_ready = 1U;
                    }
                }
            }
        }
    }
    if ((1U & (~ (IData)(vlTOPp->SoC__DOT__uart__DOT__tx_request)))) {
        __Vdly__SoC__DOT__uart__DOT__tx__DOT__bidx = 0U;
        vlTOPp->SoC__DOT__uart__DOT__tx_ready = 0U;
    }
    vlTOPp->SoC__DOT__rom_rdata = ((IData)(vlTOPp->SoC__DOT____Vcellinp__rom__i_request)
                                    ? ((0x190U >= (0x1ffU 
                                                   & (vlTOPp->SoC__DOT__cpu_address 
                                                      >> 2U)))
                                        ? vlTOPp->SoC__DOT__rom__DOT__data
                                       [(0x1ffU & (vlTOPp->SoC__DOT__cpu_address 
                                                   >> 2U))]
                                        : 0U) : 0U);
    if (vlTOPp->SoC__DOT__reset) {
        __Vdly__SoC__DOT__uart__DOT__rx__DOT__rds = 0U;
        vlTOPp->SoC__DOT__uart_rdata = 0U;
    } else {
        if (vlTOPp->SoC__DOT__uart__DOT__rx_request) {
            if ((0U == (3U & (vlTOPp->SoC__DOT__cpu_address 
                              >> 2U)))) {
                if ((0U == (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds))) {
                    if ((1U & (~ (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_empty)))) {
                        __Vdly__SoC__DOT__uart__DOT__rx__DOT__rx_fifo_read = 1U;
                        __Vdly__SoC__DOT__uart__DOT__rx__DOT__rds = 1U;
                    }
                } else {
                    if ((((1U == (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds)) 
                          | (2U == (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds))) 
                         | (3U == (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds)))) {
                        __Vdly__SoC__DOT__uart__DOT__rx__DOT__rds 
                            = (0xfU & ((IData)(1U) 
                                       + (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds)));
                        vlTOPp->SoC__DOT__uart_rdata 
                            = vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__rdata;
                    } else {
                        if ((4U == (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds))) {
                            __Vdly__SoC__DOT__uart__DOT__rx__DOT__rds = 5U;
                        }
                    }
                }
            } else {
                if ((1U == (3U & (vlTOPp->SoC__DOT__cpu_address 
                                  >> 2U)))) {
                    vlTOPp->SoC__DOT__uart_rdata = 
                        ((IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_empty) 
                         << 1U);
                    __Vdly__SoC__DOT__uart__DOT__rx__DOT__rds = 5U;
                }
            }
        } else {
            __Vdly__SoC__DOT__uart__DOT__rx__DOT__rx_fifo_read = 0U;
            __Vdly__SoC__DOT__uart__DOT__rx__DOT__rds = 0U;
        }
    }
    if (((IData)(vlTOPp->SoC__DOT__timer_select) & 
         (0U != (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)))) {
        vlTOPp->SoC__DOT__timer_rdata = ((0U == (3U 
                                                 & (vlTOPp->SoC__DOT__cpu_address 
                                                    >> 2U)))
                                          ? vlTOPp->SoC__DOT__timer__DOT__ms
                                          : ((1U == 
                                              (3U & 
                                               (vlTOPp->SoC__DOT__cpu_address 
                                                >> 2U)))
                                              ? vlTOPp->SoC__DOT__timer__DOT__cycles
                                              : ((2U 
                                                  == 
                                                  (3U 
                                                   & (vlTOPp->SoC__DOT__cpu_address 
                                                      >> 2U)))
                                                  ? vlTOPp->SoC__DOT__cpu_retire_count
                                                  : 0U)));
    }
    if (vlTOPp->SoC__DOT____Vcellinp__i2c__i_request) {
        if (vlTOPp->SoC__DOT__cpu_rw) {
            vlTOPp->SoC__DOT__i2c__DOT__sda = (1U & 
                                               (((IData)(vlTOPp->SoC__DOT__i2c__DOT__sda) 
                                                 & (~ 
                                                    (vlTOPp->SoC__DOT__cpu_wdata 
                                                     >> 4U))) 
                                                | vlTOPp->SoC__DOT__cpu_wdata));
        }
    }
    if (vlTOPp->SoC__DOT____Vcellinp__i2c__i_request) {
        if (vlTOPp->SoC__DOT__cpu_rw) {
            __Vdly__SoC__DOT__i2c__DOT__scl = (1U & 
                                               (((IData)(vlTOPp->SoC__DOT__i2c__DOT__scl) 
                                                 & (~ 
                                                    (vlTOPp->SoC__DOT__cpu_wdata 
                                                     >> 5U))) 
                                                | (vlTOPp->SoC__DOT__cpu_wdata 
                                                   >> 1U)));
        } else {
            vlTOPp->SoC__DOT__i2c_rdata = (((IData)(vlTOPp->SoC__DOT__i2c__DOT__scl) 
                                            << 1U) 
                                           | (IData)(vlTOPp->I2C_SDA));
        }
    }
    if (vlTOPp->SoC__DOT__reset) {
        __Vdly__SoC__DOT__sd__DOT__clk = 0U;
        __Vdly__SoC__DOT__sd__DOT__cdir = 0U;
        __Vdly__SoC__DOT__sd__DOT__ddir = 0U;
        vlTOPp->SoC__DOT__sd_rdata = 0U;
    } else {
        if (vlTOPp->SoC__DOT____Vcellinp__sd__i_request) {
            if (vlTOPp->SoC__DOT__cpu_rw) {
                __Vdly__SoC__DOT__sd__DOT__clk = (1U 
                                                  & (((IData)(vlTOPp->SoC__DOT__sd__DOT__clk) 
                                                      & (~ 
                                                         (vlTOPp->SoC__DOT__cpu_wdata 
                                                          >> 8U))) 
                                                     | vlTOPp->SoC__DOT__cpu_wdata));
                __Vdly__SoC__DOT__sd__DOT__cdir = (1U 
                                                   & (((IData)(vlTOPp->SoC__DOT__sd__DOT__cdir) 
                                                       & (~ 
                                                          (vlTOPp->SoC__DOT__cpu_wdata 
                                                           >> 9U))) 
                                                      | (vlTOPp->SoC__DOT__cpu_wdata 
                                                         >> 1U)));
                __Vdly__SoC__DOT__sd__DOT__ddir = (1U 
                                                   & (((IData)(vlTOPp->SoC__DOT__sd__DOT__ddir) 
                                                       & (~ 
                                                          (vlTOPp->SoC__DOT__cpu_wdata 
                                                           >> 0xaU))) 
                                                      | (vlTOPp->SoC__DOT__cpu_wdata 
                                                         >> 2U)));
                __Vdly__SoC__DOT__sd__DOT__cmd = (1U 
                                                  & (((IData)(vlTOPp->SoC__DOT__sd__DOT__cmd) 
                                                      & (~ 
                                                         (vlTOPp->SoC__DOT__cpu_wdata 
                                                          >> 0xbU))) 
                                                     | (vlTOPp->SoC__DOT__cpu_wdata 
                                                        >> 3U)));
                __Vdly__SoC__DOT__sd__DOT__dat = (0xfU 
                                                  & (((IData)(vlTOPp->SoC__DOT__sd__DOT__dat) 
                                                      & (~ 
                                                         (vlTOPp->SoC__DOT__cpu_wdata 
                                                          >> 0xcU))) 
                                                     | (vlTOPp->SoC__DOT__cpu_wdata 
                                                        >> 4U)));
            } else {
                vlTOPp->SoC__DOT__sd_rdata = ((((IData)(vlTOPp->SoC__DOT__sd__DOT__ddir)
                                                 ? (IData)(vlTOPp->SoC__DOT__sd__DOT__dat)
                                                 : (IData)(vlTOPp->SD_DAT)) 
                                               << 4U) 
                                              | ((((IData)(vlTOPp->SoC__DOT__sd__DOT__cdir)
                                                    ? (IData)(vlTOPp->SoC__DOT__sd__DOT__cmd)
                                                    : (IData)(vlTOPp->SD_CMD)) 
                                                  << 3U) 
                                                 | (((IData)(vlTOPp->SoC__DOT__sd__DOT__ddir) 
                                                     << 2U) 
                                                    | (((IData)(vlTOPp->SoC__DOT__sd__DOT__cdir) 
                                                        << 1U) 
                                                       | (IData)(vlTOPp->SoC__DOT__sd__DOT__clk)))));
            }
        }
    }
    if (((IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo_write) 
         & (~ (IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__last_write)))) {
        __Vdlyvval__SoC__DOT__video_bus__DOT__fifo__DOT__data__v0 
            = vlTOPp->SoC__DOT__video_bus__DOT__fifo_wdata;
        __Vdlyvset__SoC__DOT__video_bus__DOT__fifo__DOT__data__v0 = 1U;
        __Vdlyvdim0__SoC__DOT__video_bus__DOT__fifo__DOT__data__v0 
            = vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__in;
        __Vdly__SoC__DOT__video_bus__DOT__fifo__DOT__in 
            = (0x3ffU & ((IData)(1U) + (IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__in)));
    }
    if (vlTOPp->SoC__DOT____Vcellinp__sdram__i_request) {
        if ((1U & (~ (IData)(vlTOPp->SoC__DOT__cpu_rw)))) {
            vlTOPp->SoC__DOT__sdram_rdata = vlTOPp->SoC__DOT__sdram__DOT__data
                [(0x1ffffffU & ((vlTOPp->SoC__DOT__cpu_address 
                                 - (IData)(0x20000000U)) 
                                >> 2U))];
        }
    }
    if (vlTOPp->SoC__DOT____Vcellinp__ram__i_request) {
        if ((1U & (~ (IData)(vlTOPp->SoC__DOT__cpu_rw)))) {
            vlTOPp->SoC__DOT__ram_rdata = vlTOPp->SoC__DOT__ram__DOT__data
                [(0x3fffU & ((vlTOPp->SoC__DOT__cpu_address 
                              - (IData)(0x10000U)) 
                             >> 2U))];
        }
    }
    if (vlTOPp->SoC__DOT____Vcellinp__sdram__i_request) {
        if (vlTOPp->SoC__DOT__cpu_rw) {
            __Vdlyvval__SoC__DOT__sdram__DOT__data__v0 
                = vlTOPp->SoC__DOT__cpu_wdata;
            __Vdlyvset__SoC__DOT__sdram__DOT__data__v0 = 1U;
            __Vdlyvdim0__SoC__DOT__sdram__DOT__data__v0 
                = (0x1ffffffU & ((vlTOPp->SoC__DOT__cpu_address 
                                  - (IData)(0x20000000U)) 
                                 >> 2U));
        }
    }
    if (vlTOPp->SoC__DOT____Vcellinp__ram__i_request) {
        if (vlTOPp->SoC__DOT__cpu_rw) {
            __Vdlyvval__SoC__DOT__ram__DOT__data__v0 
                = vlTOPp->SoC__DOT__cpu_wdata;
            __Vdlyvset__SoC__DOT__ram__DOT__data__v0 = 1U;
            __Vdlyvdim0__SoC__DOT__ram__DOT__data__v0 
                = (0x3fffU & ((vlTOPp->SoC__DOT__cpu_address 
                               - (IData)(0x10000U)) 
                              >> 2U));
        }
    }
    vlTOPp->SoC__DOT__timer__DOT__prescale = __Vdly__SoC__DOT__timer__DOT__prescale;
    vlTOPp->SoC__DOT__vga__DOT__prescale = __Vdly__SoC__DOT__vga__DOT__prescale;
    if (__Vdlyvset__SoC__DOT__video_sram__DOT__data__v0) {
        vlTOPp->SoC__DOT__video_sram__DOT__data[__Vdlyvdim0__SoC__DOT__video_sram__DOT__data__v0] 
            = __Vdlyvval__SoC__DOT__video_sram__DOT__data__v0;
    }
    vlTOPp->SoC__DOT__cpu__DOT__retire_tag = __Vdly__SoC__DOT__cpu__DOT__retire_tag;
    vlTOPp->SoC__DOT__uart__DOT__tx__DOT__bidx = __Vdly__SoC__DOT__uart__DOT__tx__DOT__bidx;
    vlTOPp->SoC__DOT__uart__DOT__tx__DOT__data = __Vdly__SoC__DOT__uart__DOT__tx__DOT__data;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds = __Vdly__SoC__DOT__uart__DOT__rx__DOT__rds;
    vlTOPp->SoC__DOT__timer__DOT__ms = __Vdly__SoC__DOT__timer__DOT__ms;
    vlTOPp->SoC__DOT__timer__DOT__cycles = __Vdly__SoC__DOT__timer__DOT__cycles;
    vlTOPp->SoC__DOT__cpu_retire_count = __Vdly__SoC__DOT__cpu_retire_count;
    vlTOPp->SoC__DOT__i2c__DOT__scl = __Vdly__SoC__DOT__i2c__DOT__scl;
    vlTOPp->SoC__DOT__sd__DOT__cmd = __Vdly__SoC__DOT__sd__DOT__cmd;
    vlTOPp->SoC__DOT__sd__DOT__dat = __Vdly__SoC__DOT__sd__DOT__dat;
    vlTOPp->SoC__DOT__sd__DOT__clk = __Vdly__SoC__DOT__sd__DOT__clk;
    vlTOPp->SoC__DOT__sd__DOT__cdir = __Vdly__SoC__DOT__sd__DOT__cdir;
    vlTOPp->SoC__DOT__sd__DOT__ddir = __Vdly__SoC__DOT__sd__DOT__ddir;
    if (__Vdlyvset__SoC__DOT__sdram__DOT__data__v0) {
        vlTOPp->SoC__DOT__sdram__DOT__data[__Vdlyvdim0__SoC__DOT__sdram__DOT__data__v0] 
            = __Vdlyvval__SoC__DOT__sdram__DOT__data__v0;
    }
    if (__Vdlyvset__SoC__DOT__ram__DOT__data__v0) {
        vlTOPp->SoC__DOT__ram__DOT__data[__Vdlyvdim0__SoC__DOT__ram__DOT__data__v0] 
            = __Vdlyvval__SoC__DOT__ram__DOT__data__v0;
    }
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__last_write 
        = vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_write;
    if ((0U == (IData)(vlTOPp->SoC__DOT__video_bus__DOT__state))) {
        if (vlTOPp->SoC__DOT__vga_enable) {
            if (((1U & (IData)(vlTOPp->SoC__DOT__vga_address)) 
                 != (IData)(vlTOPp->SoC__DOT__video_bus__DOT__last_video_address))) {
                vlTOPp->SoC__DOT__video_sram_request = 1U;
                vlTOPp->SoC__DOT__video_sram_rw = 0U;
                vlTOPp->SoC__DOT__video_sram_address 
                    = ((IData)(vlTOPp->SoC__DOT__vga_address) 
                       << 2U);
                vlTOPp->SoC__DOT__video_bus__DOT__last_video_address 
                    = (1U & (IData)(vlTOPp->SoC__DOT__vga_address));
                __Vdly__SoC__DOT__video_bus__DOT__state = 3U;
            }
        } else {
            vlTOPp->SoC__DOT__video_sram_request = 0U;
            if (((IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__in) 
                 != (IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__out))) {
                __Vdly__SoC__DOT__video_bus__DOT__fifo_read = 1U;
                __Vdly__SoC__DOT__video_bus__DOT__state = 1U;
            }
        }
    } else {
        if ((1U == (IData)(vlTOPp->SoC__DOT__video_bus__DOT__state))) {
            __Vdly__SoC__DOT__video_bus__DOT__fifo_read = 0U;
            vlTOPp->SoC__DOT__video_sram_request = 1U;
            vlTOPp->SoC__DOT__video_sram_rw = 1U;
            vlTOPp->SoC__DOT__video_sram_address = (IData)(
                                                           (vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__rdata 
                                                            >> 0x20U));
            vlTOPp->SoC__DOT__video_sram_wdata = (IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__rdata);
            __Vdly__SoC__DOT__video_bus__DOT__state = 2U;
        } else {
            if ((2U == (IData)(vlTOPp->SoC__DOT__video_bus__DOT__state))) {
                if (vlTOPp->SoC__DOT__video_sram_ready) {
                    vlTOPp->SoC__DOT__video_sram_request = 0U;
                    __Vdly__SoC__DOT__video_bus__DOT__state = 0U;
                }
            } else {
                if ((3U == (IData)(vlTOPp->SoC__DOT__video_bus__DOT__state))) {
                    if (vlTOPp->SoC__DOT__video_sram_ready) {
                        vlTOPp->SoC__DOT____Vcellout__video_bus__o_video_rdata 
                            = vlTOPp->SoC__DOT__video_sram_rdata;
                        vlTOPp->SoC__DOT__video_sram_request = 0U;
                        __Vdly__SoC__DOT__video_bus__DOT__state = 0U;
                    }
                }
            }
        }
    }
    if (((IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_read) 
         & (~ (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__last_read)))) {
        vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__rdata 
            = vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data
            [vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__out];
        vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__out 
            = (3U & ((IData)(1U) + (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__out)));
    }
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
    vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__last_write 
        = vlTOPp->SoC__DOT__video_bus__DOT__fifo_write;
    if ((((IData)(vlTOPp->SoC__DOT____Vcellinp__video_bus__i_cpu_request) 
          & ((0x3ffU & ((IData)(1U) + (IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__in))) 
             != (IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__out))) 
         & (~ (IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo_write)))) {
        vlTOPp->SoC__DOT__video_bus__DOT__fifo_write = 1U;
        vlTOPp->SoC__DOT__video_bus__DOT__fifo_wdata 
            = (((QData)((IData)((vlTOPp->SoC__DOT__cpu_address 
                                 - (IData)(0x40000000U)))) 
                << 0x20U) | (QData)((IData)(vlTOPp->SoC__DOT__cpu_wdata)));
        vlTOPp->SoC__DOT__vram_ready = 1U;
    } else {
        vlTOPp->SoC__DOT__video_bus__DOT__fifo_write = 0U;
        if ((1U & (~ (IData)(vlTOPp->SoC__DOT____Vcellinp__video_bus__i_cpu_request)))) {
            vlTOPp->SoC__DOT__vram_ready = 0U;
        }
    }
    vlTOPp->SoC__DOT__video_sram_rdata = __Vdly__SoC__DOT__video_sram_rdata;
    vlTOPp->SoC__DOT__video_sram_ready = __Vdly__SoC__DOT__video_sram_ready;
    vlTOPp->SoC__DOT__video_bus__DOT__state = __Vdly__SoC__DOT__video_bus__DOT__state;
    if (__Vdlyvset__SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data__v0) {
        vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data[__Vdlyvdim0__SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data__v0] 
            = __Vdlyvval__SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data__v0;
    }
    vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__in 
        = __Vdly__SoC__DOT__video_bus__DOT__fifo__DOT__in;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__frame_error = 0U;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_write = 0U;
    if ((0U < vlTOPp->SoC__DOT__uart__DOT__rx__DOT__prescale)) {
        vlTOPp->SoC__DOT__uart__DOT__rx__DOT__prescale 
            = (0x7ffffU & (vlTOPp->SoC__DOT__uart__DOT__rx__DOT__prescale 
                           - (IData)(1U)));
    } else {
        if ((0U < (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__bidx))) {
            if ((9U < (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__bidx))) {
                if (vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx) {
                    __Vdly__SoC__DOT__uart__DOT__rx__DOT__bidx = 0U;
                    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__prescale = 0U;
                } else {
                    __Vdly__SoC__DOT__uart__DOT__rx__DOT__bidx 
                        = (0xfU & ((IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__bidx) 
                                   - (IData)(1U)));
                    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__prescale = 0x35fU;
                }
            } else {
                if ((1U < (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__bidx))) {
                    __Vdly__SoC__DOT__uart__DOT__rx__DOT__bidx 
                        = (0xfU & ((IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__bidx) 
                                   - (IData)(1U)));
                    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__data 
                        = (((IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx) 
                            << 7U) | (0x7fU & ((IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__data) 
                                               >> 1U)));
                    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__prescale = 0x35fU;
                } else {
                    if ((1U == (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__bidx))) {
                        __Vdly__SoC__DOT__uart__DOT__rx__DOT__bidx 
                            = (0xfU & ((IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__bidx) 
                                       - (IData)(1U)));
                        if (vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx) {
                            vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_write = 1U;
                        } else {
                            vlTOPp->SoC__DOT__uart__DOT__rx__DOT__frame_error = 1U;
                        }
                    }
                }
            }
        } else {
            if ((1U & (~ (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx)))) {
                vlTOPp->SoC__DOT__uart__DOT__rx__DOT__data = 0U;
                vlTOPp->SoC__DOT__uart__DOT__rx__DOT__prescale = 0x1aeU;
                __Vdly__SoC__DOT__uart__DOT__rx__DOT__bidx = 0xaU;
            }
        }
    }
    vlTOPp->HDMI_TX_D = (0xffffffU & vlTOPp->SoC__DOT____Vcellout__video_bus__o_video_rdata);
    if (((0x280U > (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_h)) 
         & (0x190U > (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_v)))) {
        vlTOPp->SoC__DOT__vga_address = (0xffffU & 
                                         ((0x1ffU & 
                                           ((IData)(vlTOPp->SoC__DOT__vga__DOT__vga_h) 
                                            >> 1U)) 
                                          + ((IData)(0x140U) 
                                             * (0x1ffU 
                                                & ((IData)(vlTOPp->SoC__DOT__vga__DOT__vga_v) 
                                                   >> 1U)))));
    }
    vlTOPp->SoC__DOT__vga_enable = ((0x280U > (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_h)) 
                                    & (0x190U > (IData)(vlTOPp->SoC__DOT__vga__DOT__vga_v)));
    if (((IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo_read) 
         & (~ (IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__last_read)))) {
        vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__rdata 
            = vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__data
            [vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__out];
        vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__out 
            = (0x3ffU & ((IData)(1U) + (IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__out)));
    }
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__last_read 
        = vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_read;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_empty 
        = ((IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__in) 
           == (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__out));
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__bidx = __Vdly__SoC__DOT__uart__DOT__rx__DOT__bidx;
    vlTOPp->SoC__DOT__vga__DOT__vga_h = __Vdly__SoC__DOT__vga__DOT__vga_h;
    vlTOPp->SoC__DOT__vga__DOT__vga_v = __Vdly__SoC__DOT__vga__DOT__vga_v;
    if (__Vdlyvset__SoC__DOT__video_bus__DOT__fifo__DOT__data__v0) {
        vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__data[__Vdlyvdim0__SoC__DOT__video_bus__DOT__fifo__DOT__data__v0] 
            = __Vdlyvval__SoC__DOT__video_bus__DOT__fifo__DOT__data__v0;
    }
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_read 
        = __Vdly__SoC__DOT__uart__DOT__rx__DOT__rx_fifo_read;
    vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx = vlTOPp->UART_RX;
    vlTOPp->HDMI_TX_DE = vlTOPp->SoC__DOT__vga_enable;
    vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__last_read 
        = vlTOPp->SoC__DOT__video_bus__DOT__fifo_read;
    vlTOPp->SoC__DOT__video_bus__DOT__fifo_read = __Vdly__SoC__DOT__video_bus__DOT__fifo_read;
}

VL_INLINE_OPT void VSoC::_sequent__TOP__3(VSoC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoC::_sequent__TOP__3\n"); );
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    CData/*3:0*/ __Vdly__SoC__DOT__cpu__DOT__registers__DOT__read_tag;
    CData/*3:0*/ __Vdly__SoC__DOT__cpu__DOT__registers__DOT__write_tag;
    CData/*0:0*/ __Vdlyvset__SoC__DOT__cpu__DOT__registers__DOT__r__v0;
    CData/*4:0*/ __Vdlyvdim0__SoC__DOT__cpu__DOT__registers__DOT__r__v32;
    CData/*0:0*/ __Vdlyvset__SoC__DOT__cpu__DOT__registers__DOT__r__v32;
    IData/*31:0*/ __Vdlyvval__SoC__DOT__cpu__DOT__registers__DOT__r__v32;
    // Body
    __Vdly__SoC__DOT__cpu__DOT__registers__DOT__write_tag 
        = vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__write_tag;
    __Vdly__SoC__DOT__cpu__DOT__registers__DOT__read_tag 
        = vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__read_tag;
    __Vdlyvset__SoC__DOT__cpu__DOT__registers__DOT__r__v0 = 0U;
    __Vdlyvset__SoC__DOT__cpu__DOT__registers__DOT__r__v32 = 0U;
    if (vlTOPp->SoC__DOT__reset) {
        vlTOPp->SoC__DOT__cpu__DOT__rs1 = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__rs2 = 0U;
        __Vdly__SoC__DOT__cpu__DOT__registers__DOT__read_tag = 0U;
    } else {
        if (((IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch_tag) 
             != (IData)(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__read_tag))) {
            vlTOPp->SoC__DOT__cpu__DOT__rs1 = ((0U 
                                                != 
                                                (0x1fU 
                                                 & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                    >> 0xfU)))
                                                ? vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r
                                               [(0x1fU 
                                                 & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                    >> 0xfU))]
                                                : 0U);
            vlTOPp->SoC__DOT__cpu__DOT__rs2 = ((0U 
                                                != 
                                                (0x1fU 
                                                 & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                    >> 0x14U)))
                                                ? vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r
                                               [(0x1fU 
                                                 & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                    >> 0x14U))]
                                                : 0U);
            __Vdly__SoC__DOT__cpu__DOT__registers__DOT__read_tag 
                = vlTOPp->SoC__DOT__cpu__DOT__fetch_tag;
        }
    }
    if (vlTOPp->SoC__DOT__reset) {
        __Vdly__SoC__DOT__cpu__DOT__registers__DOT__write_tag = 0U;
        __Vdlyvset__SoC__DOT__cpu__DOT__registers__DOT__r__v0 = 1U;
    } else {
        if (((IData)(vlTOPp->SoC__DOT__cpu__DOT__memory_tag) 
             != (IData)(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__write_tag))) {
            __Vdlyvval__SoC__DOT__cpu__DOT__registers__DOT__r__v32 
                = vlTOPp->SoC__DOT__cpu__DOT__memory_rd;
            __Vdlyvset__SoC__DOT__cpu__DOT__registers__DOT__r__v32 = 1U;
            __Vdlyvdim0__SoC__DOT__cpu__DOT__registers__DOT__r__v32 
                = vlTOPp->SoC__DOT__cpu__DOT__memory_inst_rd;
            __Vdly__SoC__DOT__cpu__DOT__registers__DOT__write_tag 
                = vlTOPp->SoC__DOT__cpu__DOT__memory_tag;
        }
    }
    if (vlTOPp->SoC__DOT__reset) {
        vlTOPp->SoC__DOT__led__DOT__leds = 0x155U;
    } else {
        if (vlTOPp->SoC__DOT____Vcellinp__led__i_request) {
            vlTOPp->SoC__DOT__led__DOT__leds = (0x3ffU 
                                                & vlTOPp->SoC__DOT__cpu_wdata);
        }
    }
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__read_tag 
        = __Vdly__SoC__DOT__cpu__DOT__registers__DOT__read_tag;
    if (__Vdlyvset__SoC__DOT__cpu__DOT__registers__DOT__r__v0) {
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
    }
    if (__Vdlyvset__SoC__DOT__cpu__DOT__registers__DOT__r__v32) {
        vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[__Vdlyvdim0__SoC__DOT__cpu__DOT__registers__DOT__r__v32] 
            = __Vdlyvval__SoC__DOT__cpu__DOT__registers__DOT__r__v32;
    }
    vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__write_tag 
        = __Vdly__SoC__DOT__cpu__DOT__registers__DOT__write_tag;
    vlTOPp->LEDR = vlTOPp->SoC__DOT__led__DOT__leds;
}

VL_INLINE_OPT void VSoC::_sequent__TOP__5(VSoC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoC::_sequent__TOP__5\n"); );
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (vlTOPp->SoC__DOT__reset) {
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__bus__DOT__state = 0U;
        vlTOPp->SoC__DOT__cpu_rw = 0U;
        vlTOPp->SoC__DOT__cpu_address = 0U;
        vlTOPp->SoC__DOT__cpu_wdata = 0U;
    } else {
        if ((0U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state))) {
            if (vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request) {
                vlTOPp->SoC__DOT__cpu_rw = vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rw;
                vlTOPp->SoC__DOT__cpu_address = vlTOPp->SoC__DOT__cpu__DOT__bus_pb_address;
                vlTOPp->SoC__DOT__cpu_wdata = vlTOPp->SoC__DOT__cpu__DOT__bus_pb_wdata;
                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__bus__DOT__state = 2U;
            } else {
                if (vlTOPp->SoC__DOT__cpu__DOT__bus_pa_request) {
                    vlTOPp->SoC__DOT__cpu_rw = 0U;
                    vlTOPp->SoC__DOT__cpu_address = vlTOPp->SoC__DOT__cpu__DOT__bus_pa_address;
                    vlTOPp->SoC__DOT__cpu_wdata = vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__i_pa_wdata;
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__bus__DOT__state = 1U;
                }
            }
        } else {
            if (((1U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)) 
                 | (2U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)))) {
                if (vlTOPp->SoC__DOT__cpu_ready) {
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__bus__DOT__state = 0U;
                }
            } else {
                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__bus__DOT__state = 0U;
            }
        }
    }
    vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__bus__DOT__state;
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
    vlTOPp->SoC__DOT____Vcellinp__rom__i_request = 
        ((0x10000U > vlTOPp->SoC__DOT__cpu_address) 
         & (0U != (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)));
    if (vlTOPp->SoC__DOT__reset) {
        vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rw = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__bus_pb_address = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__bus_pb_wdata = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[0U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[1U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[2U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[3U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[4U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[5U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[6U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[7U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[0U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[1U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[2U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[3U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[4U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[5U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[6U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[7U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_wdata = 0ULL;
    } else {
        if ((0U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state))) {
            if (vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_request) {
                if (vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rw) {
                    if ((0x40000000U > (0xfffffffcU 
                                        & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address))) {
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw = 1U;
                        vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_wdata 
                            = (((QData)((IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_wdata)) 
                                << 0x20U) | (QData)((IData)(
                                                            (0xfffffffcU 
                                                             & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address))));
                        if ((1U & ((~ (vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[
                                       (7U & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                              >> 7U))] 
                                       >> (0x1fU & 
                                           (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                            >> 2U)))) 
                                   | ((IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rdata) 
                                      == (0xfffffffcU 
                                          & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address))))) {
                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state = 5U;
                        } else {
                            if ((1U & ((vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[
                                        (7U & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                               >> 7U))] 
                                        >> (0x1fU & 
                                            (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                             >> 2U))) 
                                       & (vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[
                                          (7U & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                                 >> 7U))] 
                                          >> (0x1fU 
                                              & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                                 >> 2U)))))) {
                                vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rw = 1U;
                                vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request = 1U;
                                vlTOPp->SoC__DOT__cpu__DOT__bus_pb_address 
                                    = (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rdata);
                                vlTOPp->SoC__DOT__cpu__DOT__bus_pb_wdata 
                                    = (IData)((vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rdata 
                                               >> 0x20U));
                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state = 4U;
                            }
                        }
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[(7U 
                                                                                & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                                                                >> 7U))] 
                            = (vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[
                               (7U & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                      >> 7U))] | ((IData)(1U) 
                                                  << 
                                                  (0x1fU 
                                                   & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                                      >> 2U))));
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[(7U 
                                                                                & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                                                                >> 7U))] 
                            = (vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[
                               (7U & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                      >> 7U))] | ((IData)(1U) 
                                                  << 
                                                  (0x1fU 
                                                   & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                                      >> 2U))));
                    } else {
                        vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rw = 1U;
                        vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request = 1U;
                        vlTOPp->SoC__DOT__cpu__DOT__bus_pb_address 
                            = (0xfffffffcU & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address);
                        vlTOPp->SoC__DOT__cpu__DOT__bus_pb_wdata 
                            = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_wdata;
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state = 4U;
                    }
                } else {
                    if ((0x40000000U > (0xfffffffcU 
                                        & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address))) {
                        if (((vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[
                              (7U & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                     >> 7U))] >> (0x1fU 
                                                  & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                                     >> 2U))) 
                             & ((IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rdata) 
                                == (0xfffffffcU & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address)))) {
                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata 
                                = (IData)((vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rdata 
                                           >> 0x20U));
                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state = 5U;
                        } else {
                            if ((1U & ((vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[
                                        (7U & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                               >> 7U))] 
                                        >> (0x1fU & 
                                            (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                             >> 2U))) 
                                       & (vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[
                                          (7U & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                                 >> 7U))] 
                                          >> (0x1fU 
                                              & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                                 >> 2U)))))) {
                                vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rw = 1U;
                                vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request = 1U;
                                vlTOPp->SoC__DOT__cpu__DOT__bus_pb_address 
                                    = (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rdata);
                                vlTOPp->SoC__DOT__cpu__DOT__bus_pb_wdata 
                                    = (IData)((vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rdata 
                                               >> 0x20U));
                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state = 2U;
                            } else {
                                vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rw = 0U;
                                vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request = 1U;
                                vlTOPp->SoC__DOT__cpu__DOT__bus_pb_address 
                                    = (0xfffffffcU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address);
                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state = 1U;
                            }
                        }
                    } else {
                        vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rw = 0U;
                        vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request = 1U;
                        vlTOPp->SoC__DOT__cpu__DOT__bus_pb_address 
                            = (0xfffffffcU & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address);
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state = 3U;
                    }
                }
            }
        } else {
            if ((1U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state))) {
                if (vlTOPp->SoC__DOT__cpu__DOT__bus_pb_ready) {
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[(7U 
                                                                                & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                                                                >> 7U))] 
                        = (vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[
                           (7U & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                  >> 7U))] | ((IData)(1U) 
                                              << (0x1fU 
                                                  & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                                     >> 2U))));
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[(7U 
                                                                                & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                                                                >> 7U))] 
                        = ((~ ((IData)(1U) << (0x1fU 
                                               & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                                  >> 2U)))) 
                           & vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[
                           (7U & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                  >> 7U))]);
                    vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request = 0U;
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata 
                        = vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rdata;
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw = 1U;
                    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_wdata 
                        = (((QData)((IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rdata)) 
                            << 0x20U) | (QData)((IData)(
                                                        (0xfffffffcU 
                                                         & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address))));
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state = 5U;
                }
            } else {
                if ((2U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state))) {
                    if (vlTOPp->SoC__DOT__cpu__DOT__bus_pb_ready) {
                        vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rw = 0U;
                        vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request = 1U;
                        vlTOPp->SoC__DOT__cpu__DOT__bus_pb_address 
                            = (0xfffffffcU & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address);
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state = 1U;
                    }
                } else {
                    if ((3U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state))) {
                        if (vlTOPp->SoC__DOT__cpu__DOT__bus_pb_ready) {
                            vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request = 0U;
                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata 
                                = vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rdata;
                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state = 5U;
                        }
                    } else {
                        if ((4U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state))) {
                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw = 0U;
                            if (vlTOPp->SoC__DOT__cpu__DOT__bus_pb_ready) {
                                vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rw = 0U;
                                vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request = 0U;
                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state = 5U;
                            }
                        } else {
                            if ((5U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state))) {
                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw = 0U;
                                if ((1U & (~ (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_request)))) {
                                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state = 0U;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->SoC__DOT____Vcellinp__video_bus__i_cpu_request 
        = ((IData)(vlTOPp->SoC__DOT__vram_select) & 
           (0U != (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)));
    vlTOPp->SoC__DOT____Vcellinp__led__i_request = 
        ((IData)(vlTOPp->SoC__DOT__led_select) & (0U 
                                                  != (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)));
    vlTOPp->SoC__DOT____Vcellinp__ram__i_request = 
        ((IData)(vlTOPp->SoC__DOT__ram_select) & (0U 
                                                  != (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)));
    vlTOPp->SoC__DOT____Vcellinp__sdram__i_request 
        = ((IData)(vlTOPp->SoC__DOT__sdram_select) 
           & (0U != (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)));
    vlTOPp->SoC__DOT____Vcellinp__i2c__i_request = 
        ((IData)(vlTOPp->SoC__DOT__i2c_select) & (0U 
                                                  != (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)));
    vlTOPp->SoC__DOT____Vcellinp__sd__i_request = ((IData)(vlTOPp->SoC__DOT__sd_select) 
                                                   & (0U 
                                                      != (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)));
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
        ((IData)(vlTOPp->SoC__DOT__uart_select) & (0U 
                                                   != (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)));
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[0U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[0U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[1U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[1U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[2U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[2U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[3U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[3U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[4U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[4U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[5U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[5U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[6U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[6U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[7U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[7U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[0U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[0U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[1U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[1U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[2U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[2U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[3U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[3U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[4U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[4U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[5U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[5U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[6U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[6U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[7U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[7U];
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state;
    vlTOPp->SoC__DOT__uart__DOT__rx_request = ((IData)(vlTOPp->SoC__DOT____Vcellinp__uart__i_request) 
                                               & (~ (IData)(vlTOPp->SoC__DOT__cpu_rw)));
    vlTOPp->SoC__DOT__uart__DOT__tx_request = ((IData)(vlTOPp->SoC__DOT____Vcellinp__uart__i_request) 
                                               & (IData)(vlTOPp->SoC__DOT__cpu_rw));
    vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__o_pb_rdata__out__en1 
        = ((((IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request) 
             & (~ (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rw))) 
            & (2U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)))
            ? 0xffffffffU : 0U);
    if ((1U & (~ (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw)))) {
        vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rdata 
            = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__data
            [(0xffU & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                       >> 2U))];
    }
    if (vlTOPp->SoC__DOT__reset) {
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__state = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rw = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_request = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_wdata = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_tag = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_inst_rd = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_rd = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_pc_next = 0U;
    } else {
        if ((0U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__state))) {
            if (((IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_tag) 
                 != (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory_tag))) {
                if (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_read) {
                    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_request = 1U;
                    if (vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_ready) {
                        if ((4U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_width))) {
                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_rd 
                                = vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata;
                        } else {
                            if ((2U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_width))) {
                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_rd 
                                    = ((0xffff0000U 
                                        & ((- (IData)(
                                                      ((IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_signed) 
                                                       & ((IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_half) 
                                                          >> 0xfU)))) 
                                           << 0x10U)) 
                                       | (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_half));
                            } else {
                                if ((1U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_width))) {
                                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_rd 
                                        = ((0xffffff00U 
                                            & ((- (IData)(
                                                          ((IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_signed) 
                                                           & ((IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_byte) 
                                                              >> 7U)))) 
                                               << 8U)) 
                                           | (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_byte));
                                }
                            }
                        }
                        vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_request = 0U;
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_pc_next 
                            = vlTOPp->SoC__DOT__cpu__DOT__execute_pc_next;
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_inst_rd 
                            = vlTOPp->SoC__DOT__cpu__DOT__execute_inst_rd;
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_tag 
                            = vlTOPp->SoC__DOT__cpu__DOT__execute_tag;
                    }
                } else {
                    if (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_write) {
                        vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_request = 1U;
                        if ((4U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_width))) {
                            vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rw = 1U;
                            vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_wdata 
                                = vlTOPp->SoC__DOT__cpu__DOT__execute_rd;
                            if (vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_ready) {
                                vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rw = 0U;
                                vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_request = 0U;
                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_pc_next 
                                    = vlTOPp->SoC__DOT__cpu__DOT__execute_pc_next;
                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_inst_rd 
                                    = vlTOPp->SoC__DOT__cpu__DOT__execute_inst_rd;
                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_tag 
                                    = vlTOPp->SoC__DOT__cpu__DOT__execute_tag;
                            }
                        } else {
                            vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rw = 0U;
                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__state = 1U;
                        }
                    } else {
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_pc_next 
                            = vlTOPp->SoC__DOT__cpu__DOT__execute_pc_next;
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_inst_rd 
                            = vlTOPp->SoC__DOT__cpu__DOT__execute_inst_rd;
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_rd 
                            = vlTOPp->SoC__DOT__cpu__DOT__execute_rd;
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_tag 
                            = vlTOPp->SoC__DOT__cpu__DOT__execute_tag;
                    }
                }
            }
        } else {
            if ((1U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__state))) {
                if (vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_ready) {
                    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_request = 0U;
                    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rw = 1U;
                    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_wdata 
                        = ((1U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_width))
                            ? ((2U & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address)
                                ? ((1U & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address)
                                    ? ((0xff000000U 
                                        & (vlTOPp->SoC__DOT__cpu__DOT__execute_rd 
                                           << 0x18U)) 
                                       | (0xffffffU 
                                          & vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata))
                                    : ((0xff000000U 
                                        & vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata) 
                                       | ((0xff0000U 
                                           & (vlTOPp->SoC__DOT__cpu__DOT__execute_rd 
                                              << 0x10U)) 
                                          | (0xffffU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata))))
                                : ((1U & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address)
                                    ? ((0xffff0000U 
                                        & vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata) 
                                       | ((0xff00U 
                                           & (vlTOPp->SoC__DOT__cpu__DOT__execute_rd 
                                              << 8U)) 
                                          | (0xffU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata)))
                                    : ((0xffffff00U 
                                        & vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata) 
                                       | (0xffU & vlTOPp->SoC__DOT__cpu__DOT__execute_rd))))
                            : ((0U == (3U & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address))
                                ? ((0xffff0000U & vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata) 
                                   | (0xffffU & vlTOPp->SoC__DOT__cpu__DOT__execute_rd))
                                : ((2U == (3U & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address))
                                    ? ((0xffff0000U 
                                        & (vlTOPp->SoC__DOT__cpu__DOT__execute_rd 
                                           << 0x10U)) 
                                       | (0xffffU & vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata))
                                    : 0U)));
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__state = 2U;
                }
            } else {
                if ((2U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__state))) {
                    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_request = 1U;
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__state = 3U;
                } else {
                    if ((3U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__state))) {
                        if (vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_ready) {
                            vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_request = 0U;
                            vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rw = 0U;
                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_pc_next 
                                = vlTOPp->SoC__DOT__cpu__DOT__execute_pc_next;
                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_inst_rd 
                                = vlTOPp->SoC__DOT__cpu__DOT__execute_inst_rd;
                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_tag 
                                = vlTOPp->SoC__DOT__cpu__DOT__execute_tag;
                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__state = 0U;
                        }
                    }
                }
            }
        }
    }
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
    vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rdata = ((((
                                                   ((IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request) 
                                                    & (~ (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rw))) 
                                                   & (2U 
                                                      == (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)))
                                                   ? vlTOPp->SoC__DOT__cpu_rdata
                                                   : 0U) 
                                                 & vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__o_pb_rdata__out__en1) 
                                                & vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__o_pb_rdata__out__en1);
    if (vlTOPp->__Vdlyvset__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__data__v0) {
        vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__data[vlTOPp->__Vdlyvdim0__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__data__v0] 
            = vlTOPp->__Vdlyvval__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__data__v0;
    }
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata;
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__state 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory__DOT__state;
    vlTOPp->SoC__DOT__cpu__DOT__bus_pb_ready = (((IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request) 
                                                 & (2U 
                                                    == (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state))) 
                                                & (IData)(vlTOPp->SoC__DOT__cpu_ready));
    vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_ready 
        = ((IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_request) 
           & (5U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state)));
    if (vlTOPp->SoC__DOT__reset) {
        vlTOPp->SoC__DOT__cpu__DOT__execute_inst_rd = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__execute_pc_next = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__execute_mem_read = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__execute_mem_write = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__execute_mem_width = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__execute_mem_signed = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute_tag = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle = 0U;
    } else {
        if (((~ (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory_stall)) 
             & ((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_tag) 
                != (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_tag)))) {
            vlTOPp->SoC__DOT__cpu__DOT__execute_pc_next 
                = ((IData)(4U) + vlTOPp->SoC__DOT__cpu__DOT__decode_pc);
            vlTOPp->SoC__DOT__cpu__DOT__execute_inst_rd 
                = vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rd;
            vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                = vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_result;
            vlTOPp->SoC__DOT__cpu__DOT__execute_mem_read 
                = vlTOPp->SoC__DOT__cpu__DOT__decode_memory_read;
            vlTOPp->SoC__DOT__cpu__DOT__execute_mem_write 
                = vlTOPp->SoC__DOT__cpu__DOT__decode_memory_write;
            vlTOPp->SoC__DOT__cpu__DOT__execute_mem_width 
                = vlTOPp->SoC__DOT__cpu__DOT__decode_memory_width;
            vlTOPp->SoC__DOT__cpu__DOT__execute_mem_signed 
                = vlTOPp->SoC__DOT__cpu__DOT__decode_memory_signed;
            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle 
                = (0xffU & ((IData)(1U) + (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle)));
            if (vlTOPp->SoC__DOT__cpu__DOT__decode_arithmetic) {
                vlTOPp->SoC__DOT__cpu__DOT__execute_rd 
                    = vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_result;
                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute_tag 
                    = vlTOPp->SoC__DOT__cpu__DOT__decode_tag;
                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle = 0U;
            } else {
                if (vlTOPp->SoC__DOT__cpu__DOT__decode_jump) {
                    vlTOPp->SoC__DOT__cpu__DOT__execute_rd 
                        = ((IData)(4U) + vlTOPp->SoC__DOT__cpu__DOT__decode_pc);
                    vlTOPp->SoC__DOT__cpu__DOT__execute_pc_next 
                        = vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_result;
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute_tag 
                        = vlTOPp->SoC__DOT__cpu__DOT__decode_tag;
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle = 0U;
                } else {
                    if (vlTOPp->SoC__DOT__cpu__DOT__decode_jump_conditional) {
                        if ((1U & ((0xaU == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                    ? (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_lt_result)
                                    : ((0xbU == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                        ? (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__unsigned_lt_result)
                                        : ((0xcU == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                            ? (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__equal_result)
                                            : ((0xdU 
                                                == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                                ? (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__not_equal_result)
                                                : (
                                                   (0xeU 
                                                    == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                                    ? (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_get_result)
                                                    : 
                                                   ((0xfU 
                                                     == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                                     ? (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__unsigned_get_result)
                                                     : 0U)))))))) {
                            vlTOPp->SoC__DOT__cpu__DOT__execute_pc_next 
                                = (vlTOPp->SoC__DOT__cpu__DOT__decode_pc 
                                   + vlTOPp->SoC__DOT__cpu__DOT__decode_imm);
                        }
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute_tag 
                            = vlTOPp->SoC__DOT__cpu__DOT__decode_tag;
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle = 0U;
                    } else {
                        if (vlTOPp->SoC__DOT__cpu__DOT__decode_memory_read) {
                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute_tag 
                                = vlTOPp->SoC__DOT__cpu__DOT__decode_tag;
                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle = 0U;
                        } else {
                            if (vlTOPp->SoC__DOT__cpu__DOT__decode_memory_write) {
                                vlTOPp->SoC__DOT__cpu__DOT__execute_rd 
                                    = vlTOPp->SoC__DOT__cpu__DOT__fwd_rs2;
                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute_tag 
                                    = vlTOPp->SoC__DOT__cpu__DOT__decode_tag;
                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle = 0U;
                            } else {
                                if ((1U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_op))) {
                                    if ((0U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle))) {
                                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_signed = 1U;
                                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_numerator 
                                            = vlTOPp->SoC__DOT__cpu__DOT__fwd_rs1;
                                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_denominator 
                                            = vlTOPp->SoC__DOT__cpu__DOT__fwd_rs2;
                                    } else {
                                        if ((0x10U 
                                             == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle))) {
                                            vlTOPp->SoC__DOT__cpu__DOT__execute_rd 
                                                = vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p2_result;
                                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute_tag 
                                                = vlTOPp->SoC__DOT__cpu__DOT__decode_tag;
                                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle = 0U;
                                        }
                                    }
                                } else {
                                    if ((2U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_op))) {
                                        if ((0U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle))) {
                                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_signed = 0U;
                                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_numerator 
                                                = vlTOPp->SoC__DOT__cpu__DOT__fwd_rs1;
                                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_denominator 
                                                = vlTOPp->SoC__DOT__cpu__DOT__fwd_rs2;
                                        } else {
                                            if ((0x10U 
                                                 == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle))) {
                                                vlTOPp->SoC__DOT__cpu__DOT__execute_rd 
                                                    = vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p2_result;
                                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute_tag 
                                                    = vlTOPp->SoC__DOT__cpu__DOT__decode_tag;
                                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle = 0U;
                                            }
                                        }
                                    } else {
                                        if ((3U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_op))) {
                                            if ((0U 
                                                 == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle))) {
                                                vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_signed = 1U;
                                                vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_op1 
                                                    = vlTOPp->SoC__DOT__cpu__DOT__fwd_rs1;
                                                vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_op2 
                                                    = vlTOPp->SoC__DOT__cpu__DOT__fwd_rs2;
                                            } else {
                                                if (
                                                    (6U 
                                                     == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle))) {
                                                    vlTOPp->SoC__DOT__cpu__DOT__execute_rd 
                                                        = (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p2);
                                                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute_tag 
                                                        = vlTOPp->SoC__DOT__cpu__DOT__decode_tag;
                                                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle = 0U;
                                                }
                                            }
                                        } else {
                                            if ((4U 
                                                 == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_op))) {
                                                if (
                                                    (0U 
                                                     == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle))) {
                                                    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_signed = 1U;
                                                    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_op1 
                                                        = vlTOPp->SoC__DOT__cpu__DOT__fwd_rs1;
                                                    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_op2 
                                                        = vlTOPp->SoC__DOT__cpu__DOT__fwd_rs2;
                                                } else {
                                                    if (
                                                        (6U 
                                                         == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle))) {
                                                        vlTOPp->SoC__DOT__cpu__DOT__execute_rd 
                                                            = (IData)(
                                                                      (vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p2 
                                                                       >> 0x20U));
                                                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute_tag 
                                                            = vlTOPp->SoC__DOT__cpu__DOT__decode_tag;
                                                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle = 0U;
                                                    }
                                                }
                                            } else {
                                                if (
                                                    (5U 
                                                     == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_op))) {
                                                    if (
                                                        (0U 
                                                         == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle))) {
                                                        vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_signed = 0U;
                                                        vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_op1 
                                                            = vlTOPp->SoC__DOT__cpu__DOT__fwd_rs1;
                                                        vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_op2 
                                                            = vlTOPp->SoC__DOT__cpu__DOT__fwd_rs2;
                                                    } else {
                                                        if (
                                                            (6U 
                                                             == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle))) {
                                                            vlTOPp->SoC__DOT__cpu__DOT__execute_rd 
                                                                = (IData)(
                                                                          (vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p2 
                                                                           >> 0x20U));
                                                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute_tag 
                                                                = vlTOPp->SoC__DOT__cpu__DOT__decode_tag;
                                                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle = 0U;
                                                        }
                                                    }
                                                } else {
                                                    if (
                                                        (6U 
                                                         == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_op))) {
                                                        if (
                                                            (0U 
                                                             == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle))) {
                                                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_signed = 1U;
                                                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_numerator 
                                                                = vlTOPp->SoC__DOT__cpu__DOT__fwd_rs1;
                                                            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_denominator 
                                                                = vlTOPp->SoC__DOT__cpu__DOT__fwd_rs2;
                                                        } else {
                                                            if (
                                                                (0x10U 
                                                                 == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle))) {
                                                                vlTOPp->SoC__DOT__cpu__DOT__execute_rd 
                                                                    = vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p2_remainder;
                                                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute_tag 
                                                                    = vlTOPp->SoC__DOT__cpu__DOT__decode_tag;
                                                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle = 0U;
                                                            }
                                                        }
                                                    } else {
                                                        if (
                                                            (7U 
                                                             == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_op))) {
                                                            if (
                                                                (0U 
                                                                 == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle))) {
                                                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_signed = 0U;
                                                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_numerator 
                                                                    = vlTOPp->SoC__DOT__cpu__DOT__fwd_rs1;
                                                                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_denominator 
                                                                    = vlTOPp->SoC__DOT__cpu__DOT__fwd_rs2;
                                                            } else {
                                                                if (
                                                                    (0x10U 
                                                                     == (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle))) {
                                                                    vlTOPp->SoC__DOT__cpu__DOT__execute_rd 
                                                                        = vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p2_remainder;
                                                                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute_tag 
                                                                        = vlTOPp->SoC__DOT__cpu__DOT__decode_tag;
                                                                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle = 0U;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle;
    vlTOPp->SoC__DOT__cpu__DOT__execute_tag = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute_tag;
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p2 
        = vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p1;
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
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p2_result 
        = vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p1_result;
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p2_remainder 
        = vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p1_remainder;
    if (vlTOPp->SoC__DOT__reset) {
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__decode_tag = 0U;
    } else {
        if (((~ ((IData)(vlTOPp->SoC__DOT__cpu__DOT__memory_stall) 
                 | (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_stall))) 
             & ((IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch_tag) 
                != (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_tag)))) {
            vlTOPp->SoC__DOT__cpu__DOT__decode_instruction 
                = vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction;
            vlTOPp->SoC__DOT__cpu__DOT__decode_pc = vlTOPp->SoC__DOT__cpu__DOT__fetch_pc;
            vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rs1 
                = (((((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_B) 
                      | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_I)) 
                     | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_R)) 
                    | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_S))
                    ? (0x1fU & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                >> 0xfU)) : 0U);
            vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rs2 
                = ((((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_B) 
                     | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_R)) 
                    | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_S))
                    ? (0x1fU & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                >> 0x14U)) : 0U);
            vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rd 
                = (((((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_I) 
                      | (0x6fU == (0x7fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                     | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_R)) 
                    | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_U))
                    ? (0x1fU & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                >> 7U)) : 0U);
            vlTOPp->SoC__DOT__cpu__DOT__decode_imm 
                = ((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_B)
                    ? ((0xfffff000U & ((- (IData)((1U 
                                                   & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                      >> 0x1fU)))) 
                                       << 0xcU)) | 
                       ((0x800U & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                   << 4U)) | ((0x7e0U 
                                               & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                  >> 0x14U)) 
                                              | (0x1eU 
                                                 & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                    >> 7U)))))
                    : ((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_I)
                        ? ((0xfffff800U & ((- (IData)(
                                                      (1U 
                                                       & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                          >> 0x1fU)))) 
                                           << 0xbU)) 
                           | (0x7ffU & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                        >> 0x14U)))
                        : ((0x6fU == (0x7fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                            ? ((0xfff00000U & ((- (IData)(
                                                          (1U 
                                                           & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                              >> 0x1fU)))) 
                                               << 0x14U)) 
                               | ((0xff000U & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction) 
                                  | ((0x800U & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                >> 9U)) 
                                     | (0x7feU & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                  >> 0x14U)))))
                            : ((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_S)
                                ? ((0xfffff800U & (
                                                   (- (IData)(
                                                              (1U 
                                                               & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                                  >> 0x1fU)))) 
                                                   << 0xbU)) 
                                   | ((0x7e0U & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                 >> 0x14U)) 
                                      | (0x1fU & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                  >> 7U))))
                                : ((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_U)
                                    ? (0xfffff000U 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)
                                    : ((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_R)
                                        ? (0x3fU & 
                                           (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                            >> 0x14U))
                                        : 0U))))));
            vlTOPp->SoC__DOT__cpu__DOT__decode_arithmetic 
                = vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_ARITHMETIC;
            vlTOPp->SoC__DOT__cpu__DOT__decode_jump 
                = ((0x6fU == (0x7fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                   | (0x67U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)));
            vlTOPp->SoC__DOT__cpu__DOT__decode_jump_conditional 
                = ((((((0x63U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                       | (0x5063U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                      | (0x7063U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                     | (0x4063U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                    | (0x6063U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                   | (0x1063U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)));
            vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation 
                = vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__alu_operation;
            vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand1 
                = vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__alu_operand1;
            vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand2 
                = vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__alu_operand2;
            vlTOPp->SoC__DOT__cpu__DOT__decode_memory_read 
                = (((((3U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                      | (0x4003U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                     | (0x1003U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                    | (0x5003U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                   | (0x2003U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)));
            vlTOPp->SoC__DOT__cpu__DOT__decode_memory_write 
                = (((0x23U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                    | (0x1023U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                   | (0x2023U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)));
            vlTOPp->SoC__DOT__cpu__DOT__decode_memory_width 
                = ((3U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                    ? 1U : ((0x4003U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                             ? 1U : ((0x1003U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                      ? 2U : ((0x5003U 
                                               == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                               ? 2U
                                               : ((0x2003U 
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
                                                      : 0U))))))));
            vlTOPp->SoC__DOT__cpu__DOT__decode_memory_signed 
                = ((3U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                   | (0x1003U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)));
            if ((0x2004033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) {
                vlTOPp->SoC__DOT__cpu__DOT__decode_op = 1U;
            } else {
                if ((0x2005033U == (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) {
                    vlTOPp->SoC__DOT__cpu__DOT__decode_op = 2U;
                } else {
                    if ((0x2000033U == (0xfe00707fU 
                                        & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) {
                        vlTOPp->SoC__DOT__cpu__DOT__decode_op = 3U;
                    } else {
                        if ((0x2001033U == (0xfe00707fU 
                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) {
                            vlTOPp->SoC__DOT__cpu__DOT__decode_op = 4U;
                        } else {
                            if ((0x2003033U == (0xfe00707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) {
                                vlTOPp->SoC__DOT__cpu__DOT__decode_op = 5U;
                            } else {
                                if ((0x2006033U == 
                                     (0xfe00707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) {
                                    vlTOPp->SoC__DOT__cpu__DOT__decode_op = 6U;
                                } else {
                                    if ((0x2007033U 
                                         == (0xfe00707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) {
                                        vlTOPp->SoC__DOT__cpu__DOT__decode_op = 7U;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__decode_tag 
                = vlTOPp->SoC__DOT__cpu__DOT__fetch_tag;
        }
    }
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p1 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p1;
    vlTOPp->SoC__DOT__cpu__DOT__decode_tag = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__decode_tag;
    if (vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_signed) {
        vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p1_result 
            = VL_DIVS_III(32, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_numerator, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_denominator);
        vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p1_remainder 
            = VL_MODDIVS_III(32, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_numerator, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_denominator);
    } else {
        vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p1_result 
            = VL_DIV_III(32, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_numerator, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_denominator);
        vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p1_remainder 
            = VL_MODDIV_III(32, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_numerator, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_denominator);
    }
    vlTOPp->SoC__DOT__cpu__DOT__execute_stall = (((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_tag) 
                                                  != (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_tag)) 
                                                 & (0U 
                                                    != (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle)));
    if (vlTOPp->SoC__DOT__reset) {
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__state = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__pc = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch_tag = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__fetch_pc = 0U;
    } else {
        if ((0U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__state))) {
            if ((1U & (~ (IData)(vlTOPp->SoC__DOT__cpu__DOT____Vcellinp__fetch__i_stall)))) {
                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag 
                    = (0xfU & ((IData)(1U) + (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag)));
                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__state = 1U;
            }
        } else {
            if ((1U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__state))) {
                if (((~ (IData)(vlTOPp->SoC__DOT__cpu__DOT____Vcellinp__fetch__i_stall)) 
                     & ((IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_output_tag) 
                        == (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag)))) {
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch_tag 
                        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_output_tag;
                    vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata;
                    vlTOPp->SoC__DOT__cpu__DOT__fetch_pc 
                        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc;
                    if ((((0x6fU == (0x7fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                          | (0x67U == (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                         | ((((((0x63U == (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                                | (0x5063U == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                               | (0x7063U == (0x707fU 
                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                              | (0x4063U == (0x707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                             | (0x6063U == (0x707fU 
                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                            | (0x1063U == (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))))) {
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__state = 2U;
                    } else {
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag 
                            = (0xfU & ((IData)(1U) 
                                       + (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag)));
                    }
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__pc 
                        = ((IData)(4U) + vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc);
                }
            } else {
                if ((2U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__state))) {
                    if (((IData)(vlTOPp->SoC__DOT__cpu__DOT__writeback_tag) 
                         == (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch_tag))) {
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__pc 
                            = vlTOPp->SoC__DOT__cpu__DOT__writeback_pc_next;
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__state = 0U;
                    }
                }
            }
        }
    }
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_signed 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_signed;
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_numerator 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_numerator;
    vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_denominator 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_denominator;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__state = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__state;
    vlTOPp->SoC__DOT__cpu__DOT__fetch_tag = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch_tag;
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
    if (vlTOPp->SoC__DOT__reset) {
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache_output_tag = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_request = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[1U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[2U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[3U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[4U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[5U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[6U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[7U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[8U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[9U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xaU] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xbU] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xcU] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xdU] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xeU] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xfU] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x10U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x11U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x12U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x13U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x14U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x15U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x16U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x17U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x18U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x19U] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1aU] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1bU] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1cU] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1dU] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1eU] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1fU] = 0U;
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_wdata = 0ULL;
    } else {
        if ((0U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state))) {
            if (((IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag) 
                 != (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_output_tag))) {
                if ((1U & (vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[
                           (7U & (vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc 
                                  >> 7U))] >> (0x1fU 
                                               & (vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc 
                                                  >> 2U))))) {
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state = 1U;
                } else {
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_request = 1U;
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state = 2U;
                }
            }
        } else {
            if ((1U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state))) {
                if (((IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rdata) 
                     == vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc)) {
                    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata 
                        = (IData)((vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rdata 
                                   >> 0x20U));
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache_output_tag 
                        = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag;
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state = 0U;
                } else {
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_request = 1U;
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state = 2U;
                }
            } else {
                if ((2U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state))) {
                    if (vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_ready) {
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_request = 0U;
                        vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata 
                            = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_rdata;
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache_output_tag 
                            = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag;
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw = 1U;
                        vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_wdata 
                            = (((QData)((IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_rdata)) 
                                << 0x20U) | (QData)((IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc)));
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state = 3U;
                    }
                } else {
                    if ((3U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state))) {
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[(7U 
                                                                                & (vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc 
                                                                                >> 7U))] 
                            = (vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[
                               (7U & (vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc 
                                      >> 7U))] | ((IData)(1U) 
                                                  << 
                                                  (0x1fU 
                                                   & (vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc 
                                                      >> 2U))));
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw = 0U;
                        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state = 0U;
                    }
                }
            }
        }
    }
    if (vlTOPp->SoC__DOT__reset) {
        vlTOPp->__Vdly__SoC__DOT__cpu__DOT__writeback_tag = 0U;
        vlTOPp->SoC__DOT__cpu__DOT__writeback_pc_next = 0U;
    } else {
        if (((IData)(vlTOPp->SoC__DOT__cpu__DOT__memory_tag) 
             != (IData)(vlTOPp->SoC__DOT__cpu__DOT__writeback_tag))) {
            vlTOPp->__Vdly__SoC__DOT__cpu__DOT__writeback_tag 
                = vlTOPp->SoC__DOT__cpu__DOT__memory_tag;
            vlTOPp->SoC__DOT__cpu__DOT__writeback_inst_rd 
                = vlTOPp->SoC__DOT__cpu__DOT__memory_inst_rd;
            vlTOPp->SoC__DOT__cpu__DOT__writeback_rd 
                = vlTOPp->SoC__DOT__cpu__DOT__memory_rd;
            vlTOPp->SoC__DOT__cpu__DOT__writeback_pc_next 
                = vlTOPp->SoC__DOT__cpu__DOT__memory_pc_next;
        }
    }
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[1U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[1U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[2U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[2U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[3U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[3U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[4U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[4U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[5U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[5U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[6U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[6U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[7U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[7U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[8U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[8U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[9U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[9U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xaU] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xaU];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xbU] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xbU];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xcU] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xcU];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xdU] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xdU];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xeU] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xeU];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xfU] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0xfU];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x10U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x10U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x11U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x11U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x12U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x12U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x13U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x13U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x14U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x14U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x15U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x15U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x16U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x16U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x17U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x17U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x18U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x18U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x19U] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x19U];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1aU] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1aU];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1bU] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1bU];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1cU] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1cU];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1dU] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1dU];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1eU] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1eU];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1fU] 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[0x1fU];
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_output_tag 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache_output_tag;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag;
    vlTOPp->SoC__DOT__cpu__DOT__writeback_tag = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__writeback_tag;
    vlTOPp->SoC__DOT__cpu__DOT__memory_pc_next = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_pc_next;
    vlTOPp->SoC__DOT__cpu__DOT__memory_tag = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_tag;
    vlTOPp->SoC__DOT__cpu__DOT__memory_inst_rd = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_inst_rd;
    vlTOPp->SoC__DOT__cpu__DOT__memory_rd = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__memory_rd;
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
    if ((1U & (~ (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw)))) {
        vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rdata 
            = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data
            [(0xffU & (vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc 
                       >> 2U))];
    }
    if ((0U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state))) {
        if (vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_request) {
            if ((vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc 
                 == vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__address)) {
                vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_rdata 
                    = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__prefetch;
                vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_ready = 1U;
                vlTOPp->SoC__DOT__cpu__DOT__bus_pa_request = 1U;
                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__bus_pa_address 
                    = ((IData)(4U) + vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc);
                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state = 1U;
            } else {
                vlTOPp->SoC__DOT__cpu__DOT__bus_pa_request = 1U;
                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__bus_pa_address 
                    = vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc;
                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state = 2U;
            }
        } else {
            vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_ready = 0U;
        }
    } else {
        if ((1U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state))) {
            vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_ready = 0U;
            if (vlTOPp->SoC__DOT__cpu__DOT__bus_pa_ready) {
                vlTOPp->SoC__DOT__cpu__DOT__bus_pa_request = 0U;
                vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__prefetch 
                    = vlTOPp->SoC__DOT__cpu__DOT__bus_pa_rdata;
                vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__address 
                    = vlTOPp->SoC__DOT__cpu__DOT__bus_pa_address;
                vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state = 0U;
            }
        } else {
            if ((2U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state))) {
                if (vlTOPp->SoC__DOT__cpu__DOT__bus_pa_ready) {
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__bus_pa_address 
                        = ((IData)(4U) + vlTOPp->SoC__DOT__cpu__DOT__bus_pa_address);
                    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_rdata 
                        = vlTOPp->SoC__DOT__cpu__DOT__bus_pa_rdata;
                    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_ready = 1U;
                    vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state = 1U;
                }
            }
        }
    }
    vlTOPp->SoC__DOT__cpu__DOT__memory_stall = (((IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_tag) 
                                                 != (IData)(vlTOPp->SoC__DOT__cpu__DOT__memory_tag)) 
                                                & ((IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_read) 
                                                   | (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_write)));
    if (vlTOPp->__Vdlyvset__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data__v0) {
        vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data[vlTOPp->__Vdlyvdim0__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data__v0] 
            = vlTOPp->__Vdlyvval__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data__v0;
    }
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_request 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_request;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state 
        = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state;
    vlTOPp->SoC__DOT__cpu__DOT__bus_pa_address = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__bus_pa_address;
    vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc = vlTOPp->__Vdly__SoC__DOT__cpu__DOT__fetch__DOT__pc;
    vlTOPp->SoC__DOT__cpu__DOT____Vcellinp__fetch__i_stall 
        = ((IData)(vlTOPp->SoC__DOT__cpu__DOT__memory_stall) 
           | (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_stall));
    vlTOPp->SoC__DOT__cpu__DOT__bus_pa_ready = (((IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pa_request) 
                                                 & (1U 
                                                    == (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state))) 
                                                & (IData)(vlTOPp->SoC__DOT__cpu_ready));
    vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__o_pa_rdata__out__en0 
        = (((IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pa_request) 
            & (1U == (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)))
            ? 0xffffffffU : 0U);
    vlTOPp->SoC__DOT__cpu__DOT__bus_pa_rdata = (((((IData)(vlTOPp->SoC__DOT__cpu__DOT__bus_pa_request) 
                                                   & (1U 
                                                      == (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)))
                                                   ? vlTOPp->SoC__DOT__cpu_rdata
                                                   : 0U) 
                                                 & vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__o_pa_rdata__out__en0) 
                                                & vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__o_pa_rdata__out__en0);
}

VL_INLINE_OPT void VSoC::_combo__TOP__6(VSoC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoC::_combo__TOP__6\n"); );
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->SoC__DOT__reset = (1U & (~ (IData)(vlTOPp->CPU_RESET_n)));
}

VL_INLINE_OPT void VSoC::_sequent__TOP__7(VSoC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoC::_sequent__TOP__7\n"); );
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
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
}

void VSoC::_eval(VSoC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoC::_eval\n"); );
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if (((IData)(vlTOPp->CLOCK_125_p) & (~ (IData)(vlTOPp->__Vclklast__TOP__CLOCK_125_p)))) {
        vlTOPp->_sequent__TOP__2(vlSymsp);
        vlTOPp->__Vm_traceActivity[1U] = 1U;
    }
    if ((((IData)(vlTOPp->CLOCK_125_p) & (~ (IData)(vlTOPp->__Vclklast__TOP__CLOCK_125_p))) 
         | ((IData)(vlTOPp->__VinpClk__TOP__SoC__DOT__reset) 
            & (~ (IData)(vlTOPp->__Vclklast__TOP____VinpClk__TOP__SoC__DOT__reset))))) {
        vlTOPp->_sequent__TOP__3(vlSymsp);
        vlTOPp->__Vm_traceActivity[2U] = 1U;
    }
    if (((IData)(vlTOPp->CLOCK_125_p) & (~ (IData)(vlTOPp->__Vclklast__TOP__CLOCK_125_p)))) {
        vlTOPp->_sequent__TOP__5(vlSymsp);
        vlTOPp->__Vm_traceActivity[3U] = 1U;
    }
    vlTOPp->_combo__TOP__6(vlSymsp);
    if ((((IData)(vlTOPp->CLOCK_125_p) & (~ (IData)(vlTOPp->__Vclklast__TOP__CLOCK_125_p))) 
         | ((IData)(vlTOPp->__VinpClk__TOP__SoC__DOT__reset) 
            & (~ (IData)(vlTOPp->__Vclklast__TOP____VinpClk__TOP__SoC__DOT__reset))))) {
        vlTOPp->_sequent__TOP__7(vlSymsp);
        vlTOPp->__Vm_traceActivity[4U] = 1U;
    }
    // Final
    vlTOPp->__Vclklast__TOP__CLOCK_125_p = vlTOPp->CLOCK_125_p;
    vlTOPp->__Vclklast__TOP____VinpClk__TOP__SoC__DOT__reset 
        = vlTOPp->__VinpClk__TOP__SoC__DOT__reset;
    vlTOPp->__VinpClk__TOP__SoC__DOT__reset = vlTOPp->SoC__DOT__reset;
}

VL_INLINE_OPT QData VSoC::_change_request(VSoC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoC::_change_request\n"); );
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    return (vlTOPp->_change_request_1(vlSymsp));
}

VL_INLINE_OPT QData VSoC::_change_request_1(VSoC__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoC::_change_request_1\n"); );
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    __req |= ((vlTOPp->SoC__DOT__reset ^ vlTOPp->__Vchglast__TOP__SoC__DOT__reset));
    VL_DEBUG_IF( if(__req && ((vlTOPp->SoC__DOT__reset ^ vlTOPp->__Vchglast__TOP__SoC__DOT__reset))) VL_DBG_MSGF("        CHANGE: code/Rv32H/SoC.v:182: SoC.reset\n"); );
    // Final
    vlTOPp->__Vchglast__TOP__SoC__DOT__reset = vlTOPp->SoC__DOT__reset;
    return __req;
}

#ifdef VL_DEBUG
void VSoC::_eval_debug_assertions() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VSoC::_eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((ADC_SDO & 0xfeU))) {
        Verilated::overWidthError("ADC_SDO");}
    if (VL_UNLIKELY((AUD_ADCDAT & 0xfeU))) {
        Verilated::overWidthError("AUD_ADCDAT");}
    if (VL_UNLIKELY((AUD_ADCLRCK & 0xfeU))) {
        Verilated::overWidthError("AUD_ADCLRCK");}
    if (VL_UNLIKELY((AUD_BCLK & 0xfeU))) {
        Verilated::overWidthError("AUD_BCLK");}
    if (VL_UNLIKELY((AUD_DACLRCK & 0xfeU))) {
        Verilated::overWidthError("AUD_DACLRCK");}
    if (VL_UNLIKELY((CLOCK_125_p & 0xfeU))) {
        Verilated::overWidthError("CLOCK_125_p");}
    if (VL_UNLIKELY((CLOCK_50_B5B & 0xfeU))) {
        Verilated::overWidthError("CLOCK_50_B5B");}
    if (VL_UNLIKELY((CLOCK_50_B6A & 0xfeU))) {
        Verilated::overWidthError("CLOCK_50_B6A");}
    if (VL_UNLIKELY((CLOCK_50_B7A & 0xfeU))) {
        Verilated::overWidthError("CLOCK_50_B7A");}
    if (VL_UNLIKELY((CLOCK_50_B8A & 0xfeU))) {
        Verilated::overWidthError("CLOCK_50_B8A");}
    if (VL_UNLIKELY((CPU_RESET_n & 0xfeU))) {
        Verilated::overWidthError("CPU_RESET_n");}
    if (VL_UNLIKELY((DDR2LP_DQS_n & 0xf0U))) {
        Verilated::overWidthError("DDR2LP_DQS_n");}
    if (VL_UNLIKELY((DDR2LP_DQS_p & 0xf0U))) {
        Verilated::overWidthError("DDR2LP_DQS_p");}
    if (VL_UNLIKELY((DDR2LP_OCT_RZQ & 0xfeU))) {
        Verilated::overWidthError("DDR2LP_OCT_RZQ");}
    if (VL_UNLIKELY((GPIO & 0ULL))) {
        Verilated::overWidthError("GPIO");}
    if (VL_UNLIKELY((HDMI_TX_INT & 0xfeU))) {
        Verilated::overWidthError("HDMI_TX_INT");}
    if (VL_UNLIKELY((HSMC_CLKIN0 & 0xfeU))) {
        Verilated::overWidthError("HSMC_CLKIN0");}
    if (VL_UNLIKELY((HSMC_CLKIN_n & 0xfcU))) {
        Verilated::overWidthError("HSMC_CLKIN_n");}
    if (VL_UNLIKELY((HSMC_CLKIN_p & 0xfcU))) {
        Verilated::overWidthError("HSMC_CLKIN_p");}
    if (VL_UNLIKELY((HSMC_D & 0xf0U))) {
        Verilated::overWidthError("HSMC_D");}
    if (VL_UNLIKELY((HSMC_RX_n & 0xfffe0000U))) {
        Verilated::overWidthError("HSMC_RX_n");}
    if (VL_UNLIKELY((HSMC_RX_p & 0xfffe0000U))) {
        Verilated::overWidthError("HSMC_RX_p");}
    if (VL_UNLIKELY((HSMC_TX_n & 0xfffe0000U))) {
        Verilated::overWidthError("HSMC_TX_n");}
    if (VL_UNLIKELY((HSMC_TX_p & 0xfffe0000U))) {
        Verilated::overWidthError("HSMC_TX_p");}
    if (VL_UNLIKELY((I2C_SDA & 0xfeU))) {
        Verilated::overWidthError("I2C_SDA");}
    if (VL_UNLIKELY((KEY & 0xf0U))) {
        Verilated::overWidthError("KEY");}
    if (VL_UNLIKELY((SD_CMD & 0xfeU))) {
        Verilated::overWidthError("SD_CMD");}
    if (VL_UNLIKELY((SD_DAT & 0xf0U))) {
        Verilated::overWidthError("SD_DAT");}
    if (VL_UNLIKELY((SW & 0xfc00U))) {
        Verilated::overWidthError("SW");}
    if (VL_UNLIKELY((UART_RX & 0xfeU))) {
        Verilated::overWidthError("UART_RX");}
}
#endif  // VL_DEBUG
