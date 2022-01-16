// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_vcd_c.h"
#include "VSoC__Syms.h"


void VSoC::traceChgTop0(void* userp, VerilatedVcd* tracep) {
    VSoC__Syms* __restrict vlSymsp = static_cast<VSoC__Syms*>(userp);
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Variables
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    {
        vlTOPp->traceChgSub0(userp, tracep);
    }
}

void VSoC::traceChgSub0(void* userp, VerilatedVcd* tracep) {
    VSoC__Syms* __restrict vlSymsp = static_cast<VSoC__Syms*>(userp);
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    vluint32_t* const oldp = tracep->oldp(vlSymsp->__Vm_baseCode + 1);
    if (false && oldp) {}  // Prevent unused
    // Body
    {
        if (VL_UNLIKELY(vlTOPp->__Vm_traceActivity[1U])) {
            tracep->chgBit(oldp+0,(vlTOPp->SoC__DOT__video_sram_request));
            tracep->chgBit(oldp+1,(vlTOPp->SoC__DOT__video_sram_rw));
            tracep->chgIData(oldp+2,(vlTOPp->SoC__DOT__video_sram_address),32);
            tracep->chgIData(oldp+3,(vlTOPp->SoC__DOT__video_sram_wdata),32);
            tracep->chgIData(oldp+4,(vlTOPp->SoC__DOT__video_sram_rdata),32);
            tracep->chgBit(oldp+5,(vlTOPp->SoC__DOT__video_sram_ready));
            tracep->chgBit(oldp+6,(vlTOPp->SoC__DOT__vram_ready));
            tracep->chgBit(oldp+7,(vlTOPp->SoC__DOT__vga_enable));
            tracep->chgSData(oldp+8,(vlTOPp->SoC__DOT__vga_address),16);
            tracep->chgIData(oldp+9,(vlTOPp->SoC__DOT__rom_rdata),32);
            tracep->chgBit(oldp+10,(vlTOPp->SoC__DOT__rom_ready));
            tracep->chgIData(oldp+11,(vlTOPp->SoC__DOT__ram_rdata),32);
            tracep->chgBit(oldp+12,(vlTOPp->SoC__DOT__ram_ready));
            tracep->chgIData(oldp+13,(vlTOPp->SoC__DOT__sdram_rdata),32);
            tracep->chgBit(oldp+14,(vlTOPp->SoC__DOT__sdram_ready));
            tracep->chgBit(oldp+15,(vlTOPp->SoC__DOT__led_ready));
            tracep->chgIData(oldp+16,(vlTOPp->SoC__DOT__uart_rdata),32);
            tracep->chgIData(oldp+17,(vlTOPp->SoC__DOT__i2c_rdata),32);
            tracep->chgBit(oldp+18,(vlTOPp->SoC__DOT__i2c_ready));
            tracep->chgIData(oldp+19,(vlTOPp->SoC__DOT__sd_rdata),32);
            tracep->chgBit(oldp+20,(vlTOPp->SoC__DOT__sd_ready));
            tracep->chgIData(oldp+21,(vlTOPp->SoC__DOT__timer_rdata),32);
            tracep->chgIData(oldp+22,(vlTOPp->SoC__DOT__cpu_retire_count),32);
            tracep->chgIData(oldp+23,(vlTOPp->SoC__DOT____Vcellout__video_bus__o_video_rdata),32);
            tracep->chgBit(oldp+24,(((IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__in) 
                                     == (IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__out))));
            tracep->chgBit(oldp+25,(((0x3ffU & ((IData)(1U) 
                                                + (IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__in))) 
                                     == (IData)(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__out))));
            tracep->chgBit(oldp+26,(vlTOPp->SoC__DOT__video_bus__DOT__fifo_write));
            tracep->chgQData(oldp+27,(vlTOPp->SoC__DOT__video_bus__DOT__fifo_wdata),64);
            tracep->chgBit(oldp+29,(vlTOPp->SoC__DOT__video_bus__DOT__fifo_read));
            tracep->chgQData(oldp+30,(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__rdata),64);
            tracep->chgCData(oldp+32,(vlTOPp->SoC__DOT__video_bus__DOT__state),3);
            tracep->chgBit(oldp+33,(vlTOPp->SoC__DOT__video_bus__DOT__last_video_address));
            tracep->chgSData(oldp+34,(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__in),10);
            tracep->chgSData(oldp+35,(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__out),10);
            tracep->chgBit(oldp+36,(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__last_write));
            tracep->chgBit(oldp+37,(vlTOPp->SoC__DOT__video_bus__DOT__fifo__DOT__last_read));
            tracep->chgCData(oldp+38,(vlTOPp->SoC__DOT__vga__DOT__prescale),4);
            tracep->chgSData(oldp+39,(vlTOPp->SoC__DOT__vga__DOT__vga_h),10);
            tracep->chgSData(oldp+40,(vlTOPp->SoC__DOT__vga__DOT__vga_v),10);
            tracep->chgBit(oldp+41,(vlTOPp->SoC__DOT__uart__DOT__tx_ready));
            tracep->chgBit(oldp+42,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__frame_error));
            tracep->chgIData(oldp+43,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__prescale),19);
            tracep->chgCData(oldp+44,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__data),8);
            tracep->chgCData(oldp+45,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__bidx),4);
            tracep->chgCData(oldp+46,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds),4);
            tracep->chgBit(oldp+47,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx));
            tracep->chgBit(oldp+48,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_empty));
            tracep->chgBit(oldp+49,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_write));
            tracep->chgBit(oldp+50,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo_read));
            tracep->chgCData(oldp+51,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__rdata),8);
            tracep->chgBit(oldp+52,(((3U & ((IData)(1U) 
                                            + (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__in))) 
                                     == (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__out))));
            tracep->chgCData(oldp+53,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data[0]),8);
            tracep->chgCData(oldp+54,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data[1]),8);
            tracep->chgCData(oldp+55,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data[2]),8);
            tracep->chgCData(oldp+56,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data[3]),8);
            tracep->chgCData(oldp+57,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__in),2);
            tracep->chgCData(oldp+58,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__out),2);
            tracep->chgBit(oldp+59,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__last_write));
            tracep->chgBit(oldp+60,(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__last_read));
            tracep->chgIData(oldp+61,(vlTOPp->SoC__DOT__uart__DOT__tx__DOT__prescale),19);
            tracep->chgSData(oldp+62,(vlTOPp->SoC__DOT__uart__DOT__tx__DOT__data),9);
            tracep->chgCData(oldp+63,(vlTOPp->SoC__DOT__uart__DOT__tx__DOT__bidx),4);
            tracep->chgBit(oldp+64,(vlTOPp->SoC__DOT__i2c__DOT__scl));
            tracep->chgBit(oldp+65,(vlTOPp->SoC__DOT__i2c__DOT__sda));
            tracep->chgBit(oldp+66,(vlTOPp->SoC__DOT__sd__DOT__clk));
            tracep->chgBit(oldp+67,(vlTOPp->SoC__DOT__sd__DOT__cdir));
            tracep->chgBit(oldp+68,(vlTOPp->SoC__DOT__sd__DOT__ddir));
            tracep->chgBit(oldp+69,(vlTOPp->SoC__DOT__sd__DOT__cmd));
            tracep->chgCData(oldp+70,(vlTOPp->SoC__DOT__sd__DOT__dat),4);
            tracep->chgIData(oldp+71,(vlTOPp->SoC__DOT__timer__DOT__prescale),17);
            tracep->chgIData(oldp+72,(vlTOPp->SoC__DOT__timer__DOT__cycles),32);
            tracep->chgIData(oldp+73,(vlTOPp->SoC__DOT__timer__DOT__ms),32);
            tracep->chgCData(oldp+74,(vlTOPp->SoC__DOT__cpu__DOT__retire_tag),4);
            tracep->chgBit(oldp+75,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__o_ready));
            tracep->chgBit(oldp+76,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__o_ready));
        }
        if (VL_UNLIKELY(vlTOPp->__Vm_traceActivity[2U])) {
            tracep->chgSData(oldp+77,(vlTOPp->SoC__DOT__led__DOT__leds),10);
            tracep->chgIData(oldp+78,(vlTOPp->SoC__DOT__cpu__DOT__rs1),32);
            tracep->chgIData(oldp+79,(vlTOPp->SoC__DOT__cpu__DOT__rs2),32);
            tracep->chgCData(oldp+80,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__read_tag),4);
            tracep->chgCData(oldp+81,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__write_tag),4);
            tracep->chgIData(oldp+82,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[0]),32);
            tracep->chgIData(oldp+83,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[1]),32);
            tracep->chgIData(oldp+84,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[2]),32);
            tracep->chgIData(oldp+85,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[3]),32);
            tracep->chgIData(oldp+86,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[4]),32);
            tracep->chgIData(oldp+87,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[5]),32);
            tracep->chgIData(oldp+88,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[6]),32);
            tracep->chgIData(oldp+89,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[7]),32);
            tracep->chgIData(oldp+90,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[8]),32);
            tracep->chgIData(oldp+91,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[9]),32);
            tracep->chgIData(oldp+92,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[10]),32);
            tracep->chgIData(oldp+93,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[11]),32);
            tracep->chgIData(oldp+94,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[12]),32);
            tracep->chgIData(oldp+95,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[13]),32);
            tracep->chgIData(oldp+96,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[14]),32);
            tracep->chgIData(oldp+97,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[15]),32);
            tracep->chgIData(oldp+98,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[16]),32);
            tracep->chgIData(oldp+99,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[17]),32);
            tracep->chgIData(oldp+100,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[18]),32);
            tracep->chgIData(oldp+101,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[19]),32);
            tracep->chgIData(oldp+102,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[20]),32);
            tracep->chgIData(oldp+103,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[21]),32);
            tracep->chgIData(oldp+104,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[22]),32);
            tracep->chgIData(oldp+105,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[23]),32);
            tracep->chgIData(oldp+106,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[24]),32);
            tracep->chgIData(oldp+107,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[25]),32);
            tracep->chgIData(oldp+108,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[26]),32);
            tracep->chgIData(oldp+109,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[27]),32);
            tracep->chgIData(oldp+110,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[28]),32);
            tracep->chgIData(oldp+111,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[29]),32);
            tracep->chgIData(oldp+112,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[30]),32);
            tracep->chgIData(oldp+113,(vlTOPp->SoC__DOT__cpu__DOT__registers__DOT__r[31]),32);
        }
        if (VL_UNLIKELY(vlTOPp->__Vm_traceActivity[3U])) {
            tracep->chgBit(oldp+114,(vlTOPp->SoC__DOT__vram_select));
            tracep->chgIData(oldp+115,((vlTOPp->SoC__DOT__cpu_address 
                                        - (IData)(0x40000000U))),32);
            tracep->chgBit(oldp+116,((0x10000U > vlTOPp->SoC__DOT__cpu_address)));
            tracep->chgIData(oldp+117,(vlTOPp->SoC__DOT__cpu_address),32);
            tracep->chgBit(oldp+118,(vlTOPp->SoC__DOT__ram_select));
            tracep->chgIData(oldp+119,((vlTOPp->SoC__DOT__cpu_address 
                                        - (IData)(0x10000U))),32);
            tracep->chgBit(oldp+120,(vlTOPp->SoC__DOT__sdram_select));
            tracep->chgIData(oldp+121,((vlTOPp->SoC__DOT__cpu_address 
                                        - (IData)(0x20000000U))),32);
            tracep->chgBit(oldp+122,(vlTOPp->SoC__DOT__led_select));
            tracep->chgBit(oldp+123,(vlTOPp->SoC__DOT__uart_select));
            tracep->chgCData(oldp+124,((3U & (vlTOPp->SoC__DOT__cpu_address 
                                              >> 2U))),2);
            tracep->chgBit(oldp+125,(vlTOPp->SoC__DOT__i2c_select));
            tracep->chgBit(oldp+126,(vlTOPp->SoC__DOT__sd_select));
            tracep->chgBit(oldp+127,(vlTOPp->SoC__DOT__timer_select));
            tracep->chgBit(oldp+128,(vlTOPp->SoC__DOT__cpu_rw));
            tracep->chgBit(oldp+129,((0U != (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state))));
            tracep->chgBit(oldp+130,(vlTOPp->SoC__DOT__cpu_ready));
            tracep->chgIData(oldp+131,(vlTOPp->SoC__DOT__cpu_rdata),32);
            tracep->chgIData(oldp+132,(vlTOPp->SoC__DOT__cpu_wdata),32);
            tracep->chgBit(oldp+133,(vlTOPp->SoC__DOT____Vcellinp__video_bus__i_cpu_request));
            tracep->chgBit(oldp+134,(vlTOPp->SoC__DOT____Vcellinp__rom__i_request));
            tracep->chgBit(oldp+135,(vlTOPp->SoC__DOT____Vcellinp__ram__i_request));
            tracep->chgBit(oldp+136,(vlTOPp->SoC__DOT____Vcellinp__sdram__i_request));
            tracep->chgBit(oldp+137,(vlTOPp->SoC__DOT____Vcellinp__led__i_request));
            tracep->chgBit(oldp+138,(vlTOPp->SoC__DOT____Vcellinp__uart__i_request));
            tracep->chgBit(oldp+139,(vlTOPp->SoC__DOT__uart__DOT__rx_request));
            tracep->chgBit(oldp+140,(vlTOPp->SoC__DOT__uart__DOT__tx_request));
            tracep->chgBit(oldp+141,(vlTOPp->SoC__DOT____Vcellinp__i2c__i_request));
            tracep->chgCData(oldp+142,((0xfU & (vlTOPp->SoC__DOT__cpu_wdata 
                                                >> 4U))),4);
            tracep->chgCData(oldp+143,((0xfU & vlTOPp->SoC__DOT__cpu_wdata)),4);
            tracep->chgBit(oldp+144,(vlTOPp->SoC__DOT____Vcellinp__sd__i_request));
            tracep->chgCData(oldp+145,((0xffU & (vlTOPp->SoC__DOT__cpu_wdata 
                                                 >> 8U))),8);
            tracep->chgCData(oldp+146,((0xffU & vlTOPp->SoC__DOT__cpu_wdata)),8);
            tracep->chgBit(oldp+147,(((IData)(vlTOPp->SoC__DOT__timer_select) 
                                      & (0U != (IData)(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state)))));
            tracep->chgBit(oldp+148,(vlTOPp->SoC__DOT__cpu__DOT__bus_pa_request));
            tracep->chgBit(oldp+149,(vlTOPp->SoC__DOT__cpu__DOT__bus_pa_ready));
            tracep->chgIData(oldp+150,(vlTOPp->SoC__DOT__cpu__DOT__bus_pa_address),32);
            tracep->chgIData(oldp+151,(vlTOPp->SoC__DOT__cpu__DOT__bus_pa_rdata),32);
            tracep->chgBit(oldp+152,(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rw));
            tracep->chgBit(oldp+153,(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_request));
            tracep->chgBit(oldp+154,(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_ready));
            tracep->chgIData(oldp+155,(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_address),32);
            tracep->chgIData(oldp+156,(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_rdata),32);
            tracep->chgIData(oldp+157,(vlTOPp->SoC__DOT__cpu__DOT__bus_pb_wdata),32);
            tracep->chgCData(oldp+158,((0x1fU & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                 >> 0xfU))),5);
            tracep->chgCData(oldp+159,((0x1fU & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                 >> 0x14U))),5);
            tracep->chgCData(oldp+160,(vlTOPp->SoC__DOT__cpu__DOT__fetch_tag),4);
            tracep->chgIData(oldp+161,(vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction),32);
            tracep->chgIData(oldp+162,(vlTOPp->SoC__DOT__cpu__DOT__fetch_pc),32);
            tracep->chgCData(oldp+163,(vlTOPp->SoC__DOT__cpu__DOT__decode_tag),4);
            tracep->chgIData(oldp+164,(vlTOPp->SoC__DOT__cpu__DOT__decode_instruction),32);
            tracep->chgIData(oldp+165,(vlTOPp->SoC__DOT__cpu__DOT__decode_pc),32);
            tracep->chgCData(oldp+166,(vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rs1),5);
            tracep->chgCData(oldp+167,(vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rs2),5);
            tracep->chgCData(oldp+168,(vlTOPp->SoC__DOT__cpu__DOT__decode_inst_rd),5);
            tracep->chgIData(oldp+169,(vlTOPp->SoC__DOT__cpu__DOT__decode_imm),32);
            tracep->chgBit(oldp+170,(vlTOPp->SoC__DOT__cpu__DOT__decode_arithmetic));
            tracep->chgBit(oldp+171,(vlTOPp->SoC__DOT__cpu__DOT__decode_jump));
            tracep->chgBit(oldp+172,(vlTOPp->SoC__DOT__cpu__DOT__decode_jump_conditional));
            tracep->chgCData(oldp+173,(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation),4);
            tracep->chgCData(oldp+174,(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand1),3);
            tracep->chgCData(oldp+175,(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operand2),3);
            tracep->chgBit(oldp+176,(vlTOPp->SoC__DOT__cpu__DOT__decode_memory_read));
            tracep->chgBit(oldp+177,(vlTOPp->SoC__DOT__cpu__DOT__decode_memory_write));
            tracep->chgCData(oldp+178,(vlTOPp->SoC__DOT__cpu__DOT__decode_memory_width),3);
            tracep->chgBit(oldp+179,(vlTOPp->SoC__DOT__cpu__DOT__decode_memory_signed));
            tracep->chgCData(oldp+180,(vlTOPp->SoC__DOT__cpu__DOT__decode_op),5);
            tracep->chgCData(oldp+181,(vlTOPp->SoC__DOT__cpu__DOT__execute_tag),4);
            tracep->chgCData(oldp+182,(vlTOPp->SoC__DOT__cpu__DOT__execute_inst_rd),5);
            tracep->chgIData(oldp+183,(vlTOPp->SoC__DOT__cpu__DOT__execute_rd),32);
            tracep->chgIData(oldp+184,(vlTOPp->SoC__DOT__cpu__DOT__execute_pc_next),32);
            tracep->chgBit(oldp+185,(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_read));
            tracep->chgBit(oldp+186,(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_write));
            tracep->chgCData(oldp+187,(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_width),3);
            tracep->chgBit(oldp+188,(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_signed));
            tracep->chgIData(oldp+189,(vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address),32);
            tracep->chgBit(oldp+190,(vlTOPp->SoC__DOT__cpu__DOT__execute_stall));
            tracep->chgCData(oldp+191,(vlTOPp->SoC__DOT__cpu__DOT__memory_tag),4);
            tracep->chgCData(oldp+192,(vlTOPp->SoC__DOT__cpu__DOT__memory_inst_rd),5);
            tracep->chgIData(oldp+193,(vlTOPp->SoC__DOT__cpu__DOT__memory_rd),32);
            tracep->chgIData(oldp+194,(vlTOPp->SoC__DOT__cpu__DOT__memory_pc_next),32);
            tracep->chgBit(oldp+195,(vlTOPp->SoC__DOT__cpu__DOT__memory_stall));
            tracep->chgCData(oldp+196,(vlTOPp->SoC__DOT__cpu__DOT__writeback_tag),4);
            tracep->chgCData(oldp+197,(vlTOPp->SoC__DOT__cpu__DOT__writeback_inst_rd),5);
            tracep->chgIData(oldp+198,(vlTOPp->SoC__DOT__cpu__DOT__writeback_rd),32);
            tracep->chgIData(oldp+199,(vlTOPp->SoC__DOT__cpu__DOT__writeback_pc_next),32);
            tracep->chgCData(oldp+200,(vlTOPp->SoC__DOT__cpu__DOT__bus__DOT__state),2);
            tracep->chgBit(oldp+201,(vlTOPp->SoC__DOT__cpu__DOT____Vcellinp__fetch__i_stall));
            tracep->chgCData(oldp+202,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__state),3);
            tracep->chgIData(oldp+203,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc),32);
            tracep->chgCData(oldp+204,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag),4);
            tracep->chgCData(oldp+205,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_output_tag),4);
            tracep->chgIData(oldp+206,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata),32);
            tracep->chgBit(oldp+207,((0x33U == (0xfe00707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+208,((0x13U == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+209,((0x7033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+210,((0x7013U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+211,((0x17U == (0x7fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+212,((0x63U == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+213,((0x5063U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+214,((0x7063U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+215,((0x4063U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+216,((0x6063U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+217,((0x1063U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+218,((0x2004033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+219,((0x2005033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+220,((0x6fU == (0x7fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+221,((0x67U == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+222,((3U == (0x707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+223,((0x4003U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+224,((0x1003U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+225,((0x5003U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+226,((0x37U == (0x7fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+227,((0x2003U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+228,((0x2000033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+229,((0x2001033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+230,((0x2003033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+231,((0x6033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+232,((0x6013U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+233,((0x2006033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+234,((0x2007033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+235,((0x23U == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+236,((0x1023U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+237,((0x1033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+238,((0x1013U == (0xfc00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+239,((0x2033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+240,((0x2013U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+241,((0x3013U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+242,((0x3033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+243,((0x40005033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+244,((0x40005013U == 
                                      (0xfc00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+245,((0x5033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+246,((0x5013U == (0xfc00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+247,((0x40000033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+248,((0x2023U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+249,((0x4033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+250,((0x4013U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))));
            tracep->chgBit(oldp+251,(((((((0x63U == 
                                           (0x707fU 
                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                                          | (0x5063U 
                                             == (0x707fU 
                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                         | (0x7063U 
                                            == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                        | (0x4063U 
                                           == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                       | (0x6063U == 
                                          (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                      | (0x1063U == 
                                         (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)))));
            tracep->chgBit(oldp+252,(((((((((((((0x13U 
                                                 == 
                                                 (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                                                | (0x7013U 
                                                   == 
                                                   (0x707fU 
                                                    & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                               | (0x67U 
                                                  == 
                                                  (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                              | (3U 
                                                 == 
                                                 (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                             | (0x4003U 
                                                == 
                                                (0x707fU 
                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                            | (0x1003U 
                                               == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                           | (0x5003U 
                                              == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                          | (0x2003U 
                                             == (0x707fU 
                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                         | (0x6013U 
                                            == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                        | (0x2013U 
                                           == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                       | (0x3013U == 
                                          (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                      | (0x4013U == 
                                         (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)))));
            tracep->chgBit(oldp+253,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__is_R));
            tracep->chgBit(oldp+254,((((0x23U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                                       | (0x1023U == 
                                          (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                      | (0x2023U == 
                                         (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)))));
            tracep->chgBit(oldp+255,(((0x17U == (0x7fU 
                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                                      | (0x37U == (0x7fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)))));
            tracep->chgBit(oldp+256,(((0x6fU == (0x7fU 
                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                                      | (0x67U == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)))));
            tracep->chgBit(oldp+257,(((((((((3U == 
                                             (0x707fU 
                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)) 
                                            | (0x4003U 
                                               == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                           | (0x1003U 
                                              == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                          | (0x5003U 
                                             == (0x707fU 
                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                         | (0x2003U 
                                            == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                        | (0x23U == 
                                           (0x707fU 
                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                       | (0x1023U == 
                                          (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                      | (0x2023U == 
                                         (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)))));
            tracep->chgBit(oldp+258,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__is_ARITHMETIC));
            tracep->chgBit(oldp+259,((((((((0x2004033U 
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
                                       | (0x2006033U 
                                          == (0xfe00707fU 
                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata))) 
                                      | (0x2007033U 
                                         == (0xfe00707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata)))));
            tracep->chgCData(oldp+260,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state),2);
            tracep->chgWData(oldp+261,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid),1024);
            tracep->chgBit(oldp+293,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw));
            tracep->chgSData(oldp+294,((0xffU & (vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc 
                                                 >> 2U))),10);
            tracep->chgQData(oldp+295,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_wdata),64);
            tracep->chgQData(oldp+297,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rdata),64);
            tracep->chgBit(oldp+299,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_request));
            tracep->chgBit(oldp+300,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_ready));
            tracep->chgIData(oldp+301,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_rdata),32);
            tracep->chgIData(oldp+302,((0xffU & (vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__pc 
                                                 >> 2U))),32);
            tracep->chgCData(oldp+303,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state),4);
            tracep->chgIData(oldp+304,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__address),32);
            tracep->chgIData(oldp+305,(vlTOPp->SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__prefetch),32);
            tracep->chgBit(oldp+306,(((IData)(vlTOPp->SoC__DOT__cpu__DOT__memory_stall) 
                                      | (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute_stall))));
            tracep->chgBit(oldp+307,((0x33U == (0xfe00707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+308,((0x13U == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+309,((0x7033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+310,((0x7013U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+311,((0x17U == (0x7fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+312,((0x63U == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+313,((0x5063U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+314,((0x7063U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+315,((0x4063U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+316,((0x6063U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+317,((0x1063U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+318,((0x2004033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+319,((0x2005033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+320,((0x6fU == (0x7fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+321,((0x67U == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+322,((3U == (0x707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+323,((0x4003U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+324,((0x1003U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+325,((0x5003U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+326,((0x37U == (0x7fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+327,((0x2003U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+328,((0x2000033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+329,((0x2001033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+330,((0x2003033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+331,((0x6033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+332,((0x6013U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+333,((0x2006033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+334,((0x2007033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+335,((0x23U == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+336,((0x1023U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+337,((0x1033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+338,((0x1013U == (0xfc00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+339,((0x2033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+340,((0x2013U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+341,((0x3013U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+342,((0x3033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+343,((0x40005033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+344,((0x40005013U == 
                                      (0xfc00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+345,((0x5033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+346,((0x5013U == (0xfc00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+347,((0x40000033U == 
                                      (0xfe00707fU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+348,((0x2023U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+349,((0x4033U == (0xfe00707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+350,((0x4013U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))));
            tracep->chgBit(oldp+351,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_B));
            tracep->chgBit(oldp+352,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_I));
            tracep->chgBit(oldp+353,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_R));
            tracep->chgBit(oldp+354,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_S));
            tracep->chgBit(oldp+355,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_U));
            tracep->chgBit(oldp+356,(((0x6fU == (0x7fU 
                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                                      | (0x67U == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)))));
            tracep->chgBit(oldp+357,(((((((((3U == 
                                             (0x707fU 
                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                                            | (0x4003U 
                                               == (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                           | (0x1003U 
                                              == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                          | (0x5003U 
                                             == (0x707fU 
                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                         | (0x2003U 
                                            == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                        | (0x23U == 
                                           (0x707fU 
                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                       | (0x1023U == 
                                          (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                      | (0x2023U == 
                                         (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)))));
            tracep->chgBit(oldp+358,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_ARITHMETIC));
            tracep->chgBit(oldp+359,((((((((0x2004033U 
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
                                       | (0x2006033U 
                                          == (0xfe00707fU 
                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                      | (0x2007033U 
                                         == (0xfe00707fU 
                                             & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)))));
            tracep->chgBit(oldp+360,(((((((0x63U == 
                                           (0x707fU 
                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                                          | (0x5063U 
                                             == (0x707fU 
                                                 & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                         | (0x7063U 
                                            == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                        | (0x4063U 
                                           == (0x707fU 
                                               & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                       | (0x6063U == 
                                          (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                      | (0x1063U == 
                                         (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)))));
            tracep->chgCData(oldp+361,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__alu_operation),4);
            tracep->chgCData(oldp+362,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__alu_operand1),3);
            tracep->chgCData(oldp+363,(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__alu_operand2),3);
            tracep->chgBit(oldp+364,((((((3U == (0x707fU 
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
                                         (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)))));
            tracep->chgBit(oldp+365,((((0x23U == (0x707fU 
                                                  & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                                       | (0x1023U == 
                                          (0x707fU 
                                           & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                      | (0x2023U == 
                                         (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)))));
            tracep->chgCData(oldp+366,(((3U == (0x707fU 
                                                & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                         ? 1U : ((0x4003U 
                                                  == 
                                                  (0x707fU 
                                                   & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))
                                                  ? 1U
                                                  : 
                                                 ((0x1003U 
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
            tracep->chgBit(oldp+367,(((3U == (0x707fU 
                                              & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)) 
                                      | (0x1003U == 
                                         (0x707fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)))));
            tracep->chgIData(oldp+368,(((0xfffff000U 
                                         & ((- (IData)(
                                                       (1U 
                                                        & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                           >> 0x1fU)))) 
                                            << 0xcU)) 
                                        | ((0x800U 
                                            & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                               << 4U)) 
                                           | ((0x7e0U 
                                               & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                  >> 0x14U)) 
                                              | (0x1eU 
                                                 & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                    >> 7U)))))),32);
            tracep->chgIData(oldp+369,(((0xfffff800U 
                                         & ((- (IData)(
                                                       (1U 
                                                        & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                           >> 0x1fU)))) 
                                            << 0xbU)) 
                                        | (0x7ffU & 
                                           (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                            >> 0x14U)))),32);
            tracep->chgIData(oldp+370,(((0xfff00000U 
                                         & ((- (IData)(
                                                       (1U 
                                                        & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                           >> 0x1fU)))) 
                                            << 0x14U)) 
                                        | ((0xff000U 
                                            & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction) 
                                           | ((0x800U 
                                               & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                  >> 9U)) 
                                              | (0x7feU 
                                                 & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                    >> 0x14U)))))),32);
            tracep->chgIData(oldp+371,(((0xfffff800U 
                                         & ((- (IData)(
                                                       (1U 
                                                        & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                           >> 0x1fU)))) 
                                            << 0xbU)) 
                                        | ((0x7e0U 
                                            & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                               >> 0x14U)) 
                                           | (0x1fU 
                                              & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                 >> 7U))))),32);
            tracep->chgIData(oldp+372,((0xfffff000U 
                                        & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction)),32);
            tracep->chgIData(oldp+373,((0x3fU & (vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction 
                                                 >> 0x14U))),32);
            tracep->chgBit(oldp+374,(((((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_B) 
                                        | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_I)) 
                                       | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_R)) 
                                      | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_S))));
            tracep->chgBit(oldp+375,((((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_B) 
                                       | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_R)) 
                                      | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_S))));
            tracep->chgBit(oldp+376,(((((IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_I) 
                                        | (0x6fU == 
                                           (0x7fU & vlTOPp->SoC__DOT__cpu__DOT__fetch_instruction))) 
                                       | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_R)) 
                                      | (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode__DOT__is_U))));
            tracep->chgCData(oldp+377,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__cycle),8);
            tracep->chgBit(oldp+378,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_signed));
            tracep->chgIData(oldp+379,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_op1),32);
            tracep->chgIData(oldp+380,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__mul_op2),32);
            tracep->chgQData(oldp+381,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p2),64);
            tracep->chgBit(oldp+383,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_signed));
            tracep->chgIData(oldp+384,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_numerator),32);
            tracep->chgIData(oldp+385,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__div_denominator),32);
            tracep->chgIData(oldp+386,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p2_result),32);
            tracep->chgIData(oldp+387,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p2_remainder),32);
            tracep->chgQData(oldp+388,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p1),64);
            tracep->chgIData(oldp+390,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p1_result),32);
            tracep->chgIData(oldp+391,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p1_remainder),32);
            tracep->chgCData(oldp+392,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__state),5);
            tracep->chgBit(oldp+393,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rw));
            tracep->chgBit(oldp+394,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_request));
            tracep->chgBit(oldp+395,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_ready));
            tracep->chgIData(oldp+396,((0xfffffffcU 
                                        & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address)),32);
            tracep->chgIData(oldp+397,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata),32);
            tracep->chgIData(oldp+398,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache_wdata),32);
            tracep->chgCData(oldp+399,((3U & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address)),2);
            tracep->chgCData(oldp+400,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_byte),8);
            tracep->chgSData(oldp+401,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_half),16);
            tracep->chgCData(oldp+402,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state),8);
            tracep->chgWData(oldp+403,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid),256);
            tracep->chgWData(oldp+411,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty),256);
            tracep->chgBit(oldp+419,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw));
            tracep->chgCData(oldp+420,((0xffU & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                                 >> 2U))),8);
            tracep->chgQData(oldp+421,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_wdata),64);
            tracep->chgQData(oldp+423,(vlTOPp->SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rdata),64);
            tracep->chgBit(oldp+425,((0x40000000U > 
                                      (0xfffffffcU 
                                       & vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address))));
            tracep->chgIData(oldp+426,((0xffU & (vlTOPp->SoC__DOT__cpu__DOT__execute_mem_address 
                                                 >> 2U))),32);
        }
        if (VL_UNLIKELY(vlTOPp->__Vm_traceActivity[4U])) {
            tracep->chgIData(oldp+427,(vlTOPp->SoC__DOT__cpu__DOT__fwd_rs1),32);
            tracep->chgIData(oldp+428,(vlTOPp->SoC__DOT__cpu__DOT__fwd_rs2),32);
            tracep->chgIData(oldp+429,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1),32);
            tracep->chgIData(oldp+430,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2),32);
            tracep->chgIData(oldp+431,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_result),32);
            tracep->chgIData(oldp+432,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                        + vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)),32);
            tracep->chgIData(oldp+433,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                        + vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)),32);
            tracep->chgIData(oldp+434,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                        - vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)),32);
            tracep->chgIData(oldp+435,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                        & vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)),32);
            tracep->chgIData(oldp+436,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                        | vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)),32);
            tracep->chgIData(oldp+437,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                        ^ vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2)),32);
            tracep->chgIData(oldp+438,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                        << (0x1fU & vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2))),32);
            tracep->chgIData(oldp+439,((vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1 
                                        >> (0x1fU & vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2))),32);
            tracep->chgIData(oldp+440,(VL_SHIFTRS_III(32,32,5, vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand1, 
                                                      (0x1fU 
                                                       & vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu_operand2))),32);
            tracep->chgBit(oldp+441,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_lt_result));
            tracep->chgBit(oldp+442,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__unsigned_lt_result));
            tracep->chgBit(oldp+443,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__equal_result));
            tracep->chgBit(oldp+444,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__not_equal_result));
            tracep->chgBit(oldp+445,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_get_result));
            tracep->chgBit(oldp+446,(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__unsigned_get_result));
        }
        tracep->chgBit(oldp+447,(vlTOPp->ADC_CONVST));
        tracep->chgBit(oldp+448,(vlTOPp->ADC_SCK));
        tracep->chgBit(oldp+449,(vlTOPp->ADC_SDI));
        tracep->chgBit(oldp+450,(vlTOPp->ADC_SDO));
        tracep->chgBit(oldp+451,(vlTOPp->AUD_ADCDAT));
        tracep->chgBit(oldp+452,(vlTOPp->AUD_ADCLRCK));
        tracep->chgBit(oldp+453,(vlTOPp->AUD_BCLK));
        tracep->chgBit(oldp+454,(vlTOPp->AUD_DACDAT));
        tracep->chgBit(oldp+455,(vlTOPp->AUD_DACLRCK));
        tracep->chgBit(oldp+456,(vlTOPp->AUD_XCK));
        tracep->chgBit(oldp+457,(vlTOPp->CLOCK_125_p));
        tracep->chgBit(oldp+458,(vlTOPp->CLOCK_50_B5B));
        tracep->chgBit(oldp+459,(vlTOPp->CLOCK_50_B6A));
        tracep->chgBit(oldp+460,(vlTOPp->CLOCK_50_B7A));
        tracep->chgBit(oldp+461,(vlTOPp->CLOCK_50_B8A));
        tracep->chgBit(oldp+462,(vlTOPp->CPU_RESET_n));
        tracep->chgSData(oldp+463,(vlTOPp->DDR2LP_CA),10);
        tracep->chgCData(oldp+464,(vlTOPp->DDR2LP_CKE),2);
        tracep->chgBit(oldp+465,(vlTOPp->DDR2LP_CK_n));
        tracep->chgBit(oldp+466,(vlTOPp->DDR2LP_CK_p));
        tracep->chgCData(oldp+467,(vlTOPp->DDR2LP_CS_n),2);
        tracep->chgCData(oldp+468,(vlTOPp->DDR2LP_DM),4);
        tracep->chgIData(oldp+469,(vlTOPp->DDR2LP_DQ),32);
        tracep->chgCData(oldp+470,(vlTOPp->DDR2LP_DQS_n),4);
        tracep->chgCData(oldp+471,(vlTOPp->DDR2LP_DQS_p),4);
        tracep->chgBit(oldp+472,(vlTOPp->DDR2LP_OCT_RZQ));
        tracep->chgQData(oldp+473,(vlTOPp->GPIO),36);
        tracep->chgBit(oldp+475,(vlTOPp->HDMI_TX_CLK));
        tracep->chgIData(oldp+476,(vlTOPp->HDMI_TX_D),24);
        tracep->chgBit(oldp+477,(vlTOPp->HDMI_TX_DE));
        tracep->chgBit(oldp+478,(vlTOPp->HDMI_TX_HS));
        tracep->chgBit(oldp+479,(vlTOPp->HDMI_TX_INT));
        tracep->chgBit(oldp+480,(vlTOPp->HDMI_TX_VS));
        tracep->chgCData(oldp+481,(vlTOPp->HEX0),7);
        tracep->chgCData(oldp+482,(vlTOPp->HEX1),7);
        tracep->chgBit(oldp+483,(vlTOPp->HSMC_CLKIN0));
        tracep->chgCData(oldp+484,(vlTOPp->HSMC_CLKIN_n),2);
        tracep->chgCData(oldp+485,(vlTOPp->HSMC_CLKIN_p),2);
        tracep->chgBit(oldp+486,(vlTOPp->HSMC_CLKOUT0));
        tracep->chgCData(oldp+487,(vlTOPp->HSMC_CLKOUT_n),2);
        tracep->chgCData(oldp+488,(vlTOPp->HSMC_CLKOUT_p),2);
        tracep->chgCData(oldp+489,(vlTOPp->HSMC_D),4);
        tracep->chgIData(oldp+490,(vlTOPp->HSMC_RX_n),17);
        tracep->chgIData(oldp+491,(vlTOPp->HSMC_RX_p),17);
        tracep->chgIData(oldp+492,(vlTOPp->HSMC_TX_n),17);
        tracep->chgIData(oldp+493,(vlTOPp->HSMC_TX_p),17);
        tracep->chgBit(oldp+494,(vlTOPp->I2C_SCL));
        tracep->chgBit(oldp+495,(vlTOPp->I2C_SDA));
        tracep->chgCData(oldp+496,(vlTOPp->KEY),4);
        tracep->chgCData(oldp+497,(vlTOPp->LEDG),8);
        tracep->chgSData(oldp+498,(vlTOPp->LEDR),10);
        tracep->chgBit(oldp+499,(vlTOPp->SD_CLK));
        tracep->chgBit(oldp+500,(vlTOPp->SD_CMD));
        tracep->chgCData(oldp+501,(vlTOPp->SD_DAT),4);
        tracep->chgIData(oldp+502,(vlTOPp->SRAM_A),18);
        tracep->chgBit(oldp+503,(vlTOPp->SRAM_CE_n));
        tracep->chgSData(oldp+504,(vlTOPp->SRAM_D),16);
        tracep->chgBit(oldp+505,(vlTOPp->SRAM_LB_n));
        tracep->chgBit(oldp+506,(vlTOPp->SRAM_OE_n));
        tracep->chgBit(oldp+507,(vlTOPp->SRAM_UB_n));
        tracep->chgBit(oldp+508,(vlTOPp->SRAM_WE_n));
        tracep->chgSData(oldp+509,(vlTOPp->SW),10);
        tracep->chgBit(oldp+510,(vlTOPp->UART_RX));
        tracep->chgBit(oldp+511,(vlTOPp->UART_TX));
        tracep->chgBit(oldp+512,(vlTOPp->SoC__DOT__reset));
        tracep->chgBit(oldp+513,(((IData)(vlTOPp->SoC__DOT__uart__DOT__rx_request)
                                   ? ((5U == (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds)) 
                                      & (IData)(vlTOPp->SoC__DOT__uart__DOT__rx_request))
                                   : ((IData)(vlTOPp->SoC__DOT__uart__DOT__tx_request) 
                                      & (IData)(vlTOPp->SoC__DOT__uart__DOT__tx_ready)))));
        tracep->chgBit(oldp+514,(((5U == (IData)(vlTOPp->SoC__DOT__uart__DOT__rx__DOT__rds)) 
                                  & (IData)(vlTOPp->SoC__DOT__uart__DOT__rx_request))));
        tracep->chgBit(oldp+515,((1U & ((0xaU == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
                                         ? (IData)(vlTOPp->SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_lt_result)
                                         : ((0xbU == (IData)(vlTOPp->SoC__DOT__cpu__DOT__decode_alu_operation))
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
    }
}

void VSoC::traceCleanup(void* userp, VerilatedVcd* /*unused*/) {
    VSoC__Syms* __restrict vlSymsp = static_cast<VSoC__Syms*>(userp);
    VSoC* const __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    {
        vlSymsp->__Vm_activity = false;
        vlTOPp->__Vm_traceActivity[0U] = 0U;
        vlTOPp->__Vm_traceActivity[1U] = 0U;
        vlTOPp->__Vm_traceActivity[2U] = 0U;
        vlTOPp->__Vm_traceActivity[3U] = 0U;
        vlTOPp->__Vm_traceActivity[4U] = 0U;
    }
}
