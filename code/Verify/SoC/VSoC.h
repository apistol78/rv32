// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _VSOC_H_
#define _VSOC_H_  // guard

#include "verilated_heavy.h"

//==========

class VSoC__Syms;
class VSoC_VerilatedVcd;


//----------

VL_MODULE(VSoC) {
  public:
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(CLOCK_125_p,0,0);
    VL_OUT8(ADC_CONVST,0,0);
    VL_OUT8(ADC_SCK,0,0);
    VL_OUT8(ADC_SDI,0,0);
    VL_IN8(ADC_SDO,0,0);
    VL_IN8(AUD_ADCDAT,0,0);
    VL_INOUT8(AUD_ADCLRCK,0,0);
    VL_INOUT8(AUD_BCLK,0,0);
    VL_OUT8(AUD_DACDAT,0,0);
    VL_INOUT8(AUD_DACLRCK,0,0);
    VL_OUT8(AUD_XCK,0,0);
    VL_IN8(CLOCK_50_B5B,0,0);
    VL_IN8(CLOCK_50_B6A,0,0);
    VL_IN8(CLOCK_50_B7A,0,0);
    VL_IN8(CLOCK_50_B8A,0,0);
    VL_IN8(CPU_RESET_n,0,0);
    VL_OUT8(DDR2LP_CKE,1,0);
    VL_OUT8(DDR2LP_CK_n,0,0);
    VL_OUT8(DDR2LP_CK_p,0,0);
    VL_OUT8(DDR2LP_CS_n,1,0);
    VL_OUT8(DDR2LP_DM,3,0);
    VL_INOUT8(DDR2LP_DQS_n,3,0);
    VL_INOUT8(DDR2LP_DQS_p,3,0);
    VL_IN8(DDR2LP_OCT_RZQ,0,0);
    VL_OUT8(HDMI_TX_CLK,0,0);
    VL_OUT8(HDMI_TX_DE,0,0);
    VL_OUT8(HDMI_TX_HS,0,0);
    VL_IN8(HDMI_TX_INT,0,0);
    VL_OUT8(HDMI_TX_VS,0,0);
    VL_OUT8(HEX0,6,0);
    VL_OUT8(HEX1,6,0);
    VL_IN8(HSMC_CLKIN0,0,0);
    VL_IN8(HSMC_CLKIN_n,2,1);
    VL_IN8(HSMC_CLKIN_p,2,1);
    VL_OUT8(HSMC_CLKOUT0,0,0);
    VL_OUT8(HSMC_CLKOUT_n,2,1);
    VL_OUT8(HSMC_CLKOUT_p,2,1);
    VL_INOUT8(HSMC_D,3,0);
    VL_OUT8(I2C_SCL,0,0);
    VL_INOUT8(I2C_SDA,0,0);
    VL_IN8(KEY,3,0);
    VL_OUT8(LEDG,7,0);
    VL_OUT8(SD_CLK,0,0);
    VL_INOUT8(SD_CMD,0,0);
    VL_INOUT8(SD_DAT,3,0);
    VL_OUT8(SRAM_CE_n,0,0);
    VL_OUT8(SRAM_LB_n,0,0);
    VL_OUT8(SRAM_OE_n,0,0);
    VL_OUT8(SRAM_UB_n,0,0);
    VL_OUT8(SRAM_WE_n,0,0);
    VL_IN8(UART_RX,0,0);
    VL_OUT8(UART_TX,0,0);
    VL_OUT16(DDR2LP_CA,9,0);
    VL_OUT16(LEDR,9,0);
    VL_INOUT16(SRAM_D,15,0);
    VL_IN16(SW,9,0);
    VL_INOUT(DDR2LP_DQ,31,0);
    VL_OUT(HDMI_TX_D,23,0);
    VL_INOUT(HSMC_RX_n,16,0);
    VL_INOUT(HSMC_RX_p,16,0);
    VL_INOUT(HSMC_TX_n,16,0);
    VL_INOUT(HSMC_TX_p,16,0);
    VL_OUT(SRAM_A,17,0);
    VL_INOUT64(GPIO,35,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ SoC__DOT__reset;
        CData/*0:0*/ SoC__DOT__video_sram_request;
        CData/*0:0*/ SoC__DOT__video_sram_rw;
        CData/*0:0*/ SoC__DOT__video_sram_ready;
        CData/*0:0*/ SoC__DOT__vram_select;
        CData/*0:0*/ SoC__DOT__vram_ready;
        CData/*0:0*/ SoC__DOT__vga_enable;
        CData/*0:0*/ SoC__DOT__rom_ready;
        CData/*0:0*/ SoC__DOT__ram_select;
        CData/*0:0*/ SoC__DOT__ram_ready;
        CData/*0:0*/ SoC__DOT__sdram_select;
        CData/*0:0*/ SoC__DOT__sdram_ready;
        CData/*0:0*/ SoC__DOT__led_select;
        CData/*0:0*/ SoC__DOT__led_ready;
        CData/*0:0*/ SoC__DOT__uart_select;
        CData/*0:0*/ SoC__DOT__i2c_select;
        CData/*0:0*/ SoC__DOT__i2c_ready;
        CData/*0:0*/ SoC__DOT__sd_select;
        CData/*0:0*/ SoC__DOT__sd_ready;
        CData/*0:0*/ SoC__DOT__timer_select;
        CData/*0:0*/ SoC__DOT__cpu_rw;
        CData/*0:0*/ SoC__DOT__cpu_ready;
        CData/*0:0*/ SoC__DOT__video_bus__DOT__fifo_write;
        CData/*0:0*/ SoC__DOT__video_bus__DOT__fifo_read;
        CData/*2:0*/ SoC__DOT__video_bus__DOT__state;
        CData/*0:0*/ SoC__DOT__video_bus__DOT__last_video_address;
        CData/*0:0*/ SoC__DOT__video_bus__DOT__fifo__DOT__last_write;
        CData/*0:0*/ SoC__DOT__video_bus__DOT__fifo__DOT__last_read;
        CData/*3:0*/ SoC__DOT__vga__DOT__prescale;
        CData/*0:0*/ SoC__DOT__uart__DOT__rx_request;
        CData/*0:0*/ SoC__DOT__uart__DOT__tx_request;
        CData/*0:0*/ SoC__DOT__uart__DOT__tx_ready;
        CData/*0:0*/ SoC__DOT__uart__DOT__rx__DOT__frame_error;
        CData/*7:0*/ SoC__DOT__uart__DOT__rx__DOT__data;
        CData/*3:0*/ SoC__DOT__uart__DOT__rx__DOT__bidx;
        CData/*3:0*/ SoC__DOT__uart__DOT__rx__DOT__rds;
        CData/*0:0*/ SoC__DOT__uart__DOT__rx__DOT__rx;
        CData/*0:0*/ SoC__DOT__uart__DOT__rx__DOT__rx_fifo_empty;
        CData/*0:0*/ SoC__DOT__uart__DOT__rx__DOT__rx_fifo_write;
        CData/*0:0*/ SoC__DOT__uart__DOT__rx__DOT__rx_fifo_read;
        CData/*7:0*/ SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__rdata;
        CData/*1:0*/ SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__in;
        CData/*1:0*/ SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__out;
        CData/*0:0*/ SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__last_write;
        CData/*0:0*/ SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__last_read;
        CData/*3:0*/ SoC__DOT__uart__DOT__tx__DOT__bidx;
        CData/*0:0*/ SoC__DOT__i2c__DOT__scl;
        CData/*0:0*/ SoC__DOT__i2c__DOT__sda;
        CData/*0:0*/ SoC__DOT__sd__DOT__clk;
        CData/*0:0*/ SoC__DOT__sd__DOT__cdir;
        CData/*0:0*/ SoC__DOT__sd__DOT__ddir;
        CData/*0:0*/ SoC__DOT__sd__DOT__cmd;
        CData/*3:0*/ SoC__DOT__sd__DOT__dat;
        CData/*0:0*/ SoC__DOT__cpu__DOT__bus_pa_request;
        CData/*0:0*/ SoC__DOT__cpu__DOT__bus_pa_ready;
        CData/*0:0*/ SoC__DOT__cpu__DOT__bus_pb_rw;
        CData/*0:0*/ SoC__DOT__cpu__DOT__bus_pb_request;
        CData/*0:0*/ SoC__DOT__cpu__DOT__bus_pb_ready;
        CData/*3:0*/ SoC__DOT__cpu__DOT__fetch_tag;
        CData/*3:0*/ SoC__DOT__cpu__DOT__decode_tag;
        CData/*4:0*/ SoC__DOT__cpu__DOT__decode_inst_rs1;
        CData/*4:0*/ SoC__DOT__cpu__DOT__decode_inst_rs2;
        CData/*4:0*/ SoC__DOT__cpu__DOT__decode_inst_rd;
        CData/*0:0*/ SoC__DOT__cpu__DOT__decode_arithmetic;
    };
    struct {
        CData/*0:0*/ SoC__DOT__cpu__DOT__decode_jump;
        CData/*0:0*/ SoC__DOT__cpu__DOT__decode_jump_conditional;
        CData/*3:0*/ SoC__DOT__cpu__DOT__decode_alu_operation;
        CData/*2:0*/ SoC__DOT__cpu__DOT__decode_alu_operand1;
        CData/*2:0*/ SoC__DOT__cpu__DOT__decode_alu_operand2;
        CData/*0:0*/ SoC__DOT__cpu__DOT__decode_memory_read;
        CData/*0:0*/ SoC__DOT__cpu__DOT__decode_memory_write;
        CData/*2:0*/ SoC__DOT__cpu__DOT__decode_memory_width;
        CData/*0:0*/ SoC__DOT__cpu__DOT__decode_memory_signed;
        CData/*4:0*/ SoC__DOT__cpu__DOT__decode_op;
        CData/*3:0*/ SoC__DOT__cpu__DOT__execute_tag;
        CData/*4:0*/ SoC__DOT__cpu__DOT__execute_inst_rd;
        CData/*0:0*/ SoC__DOT__cpu__DOT__execute_mem_read;
        CData/*0:0*/ SoC__DOT__cpu__DOT__execute_mem_write;
        CData/*2:0*/ SoC__DOT__cpu__DOT__execute_mem_width;
        CData/*0:0*/ SoC__DOT__cpu__DOT__execute_mem_signed;
        CData/*0:0*/ SoC__DOT__cpu__DOT__execute_stall;
        CData/*3:0*/ SoC__DOT__cpu__DOT__memory_tag;
        CData/*4:0*/ SoC__DOT__cpu__DOT__memory_inst_rd;
        CData/*0:0*/ SoC__DOT__cpu__DOT__memory_stall;
        CData/*3:0*/ SoC__DOT__cpu__DOT__writeback_tag;
        CData/*4:0*/ SoC__DOT__cpu__DOT__writeback_inst_rd;
        CData/*3:0*/ SoC__DOT__cpu__DOT__retire_tag;
        CData/*1:0*/ SoC__DOT__cpu__DOT__bus__DOT__state;
        CData/*3:0*/ SoC__DOT__cpu__DOT__registers__DOT__read_tag;
        CData/*3:0*/ SoC__DOT__cpu__DOT__registers__DOT__write_tag;
        CData/*2:0*/ SoC__DOT__cpu__DOT__fetch__DOT__state;
        CData/*3:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag;
        CData/*3:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache_output_tag;
        CData/*0:0*/ SoC__DOT__cpu__DOT__fetch__DOT__is_R;
        CData/*0:0*/ SoC__DOT__cpu__DOT__fetch__DOT__is_ARITHMETIC;
        CData/*1:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state;
        CData/*0:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw;
        CData/*0:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_request;
        CData/*0:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_ready;
        CData/*0:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__o_ready;
        CData/*3:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state;
        CData/*0:0*/ SoC__DOT__cpu__DOT__decode__DOT__is_B;
        CData/*0:0*/ SoC__DOT__cpu__DOT__decode__DOT__is_I;
        CData/*0:0*/ SoC__DOT__cpu__DOT__decode__DOT__is_R;
        CData/*0:0*/ SoC__DOT__cpu__DOT__decode__DOT__is_S;
        CData/*0:0*/ SoC__DOT__cpu__DOT__decode__DOT__is_U;
        CData/*0:0*/ SoC__DOT__cpu__DOT__decode__DOT__is_ARITHMETIC;
        CData/*3:0*/ SoC__DOT__cpu__DOT__decode__DOT__alu_operation;
        CData/*2:0*/ SoC__DOT__cpu__DOT__decode__DOT__alu_operand1;
        CData/*2:0*/ SoC__DOT__cpu__DOT__decode__DOT__alu_operand2;
        CData/*7:0*/ SoC__DOT__cpu__DOT__execute__DOT__cycle;
        CData/*0:0*/ SoC__DOT__cpu__DOT__execute__DOT__mul_signed;
        CData/*0:0*/ SoC__DOT__cpu__DOT__execute__DOT__div_signed;
        CData/*0:0*/ SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_lt_result;
        CData/*0:0*/ SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__unsigned_lt_result;
        CData/*0:0*/ SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__equal_result;
        CData/*0:0*/ SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__not_equal_result;
        CData/*0:0*/ SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__signed_get_result;
        CData/*0:0*/ SoC__DOT__cpu__DOT__execute__DOT__alu__DOT__unsigned_get_result;
        CData/*4:0*/ SoC__DOT__cpu__DOT__memory__DOT__state;
        CData/*0:0*/ SoC__DOT__cpu__DOT__memory__DOT__dcache_rw;
        CData/*0:0*/ SoC__DOT__cpu__DOT__memory__DOT__dcache_request;
        CData/*0:0*/ SoC__DOT__cpu__DOT__memory__DOT__dcache_ready;
        CData/*7:0*/ SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_byte;
        CData/*7:0*/ SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state;
        CData/*0:0*/ SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw;
        CData/*0:0*/ SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__o_ready;
        SData/*15:0*/ SoC__DOT__vga_address;
    };
    struct {
        SData/*9:0*/ SoC__DOT__video_bus__DOT__fifo__DOT__in;
        SData/*9:0*/ SoC__DOT__video_bus__DOT__fifo__DOT__out;
        SData/*9:0*/ SoC__DOT__vga__DOT__vga_h;
        SData/*9:0*/ SoC__DOT__vga__DOT__vga_v;
        SData/*9:0*/ SoC__DOT__led__DOT__leds;
        SData/*8:0*/ SoC__DOT__uart__DOT__tx__DOT__data;
        SData/*15:0*/ SoC__DOT__cpu__DOT__memory__DOT__bus_rdata_half;
        IData/*31:0*/ SoC__DOT__video_sram_address;
        IData/*31:0*/ SoC__DOT__video_sram_wdata;
        IData/*31:0*/ SoC__DOT__video_sram_rdata;
        IData/*31:0*/ SoC__DOT__rom_rdata;
        IData/*31:0*/ SoC__DOT__ram_rdata;
        IData/*31:0*/ SoC__DOT__sdram_rdata;
        IData/*31:0*/ SoC__DOT__uart_rdata;
        IData/*31:0*/ SoC__DOT__i2c_rdata;
        IData/*31:0*/ SoC__DOT__sd_rdata;
        IData/*31:0*/ SoC__DOT__timer_rdata;
        IData/*31:0*/ SoC__DOT__cpu_address;
        IData/*31:0*/ SoC__DOT__cpu_rdata;
        IData/*31:0*/ SoC__DOT__cpu_wdata;
        IData/*31:0*/ SoC__DOT__cpu_retire_count;
        IData/*18:0*/ SoC__DOT__uart__DOT__rx__DOT__prescale;
        IData/*18:0*/ SoC__DOT__uart__DOT__tx__DOT__prescale;
        IData/*16:0*/ SoC__DOT__timer__DOT__prescale;
        IData/*31:0*/ SoC__DOT__timer__DOT__cycles;
        IData/*31:0*/ SoC__DOT__timer__DOT__ms;
        IData/*31:0*/ SoC__DOT__cpu__DOT__bus_pa_address;
        IData/*31:0*/ SoC__DOT__cpu__DOT__bus_pa_rdata;
        IData/*31:0*/ SoC__DOT__cpu__DOT__bus_pb_address;
        IData/*31:0*/ SoC__DOT__cpu__DOT__bus_pb_rdata;
        IData/*31:0*/ SoC__DOT__cpu__DOT__bus_pb_wdata;
        IData/*31:0*/ SoC__DOT__cpu__DOT__rs1;
        IData/*31:0*/ SoC__DOT__cpu__DOT__rs2;
        IData/*31:0*/ SoC__DOT__cpu__DOT__fetch_instruction;
        IData/*31:0*/ SoC__DOT__cpu__DOT__fetch_pc;
        IData/*31:0*/ SoC__DOT__cpu__DOT__decode_instruction;
        IData/*31:0*/ SoC__DOT__cpu__DOT__decode_pc;
        IData/*31:0*/ SoC__DOT__cpu__DOT__decode_imm;
        IData/*31:0*/ SoC__DOT__cpu__DOT__fwd_rs1;
        IData/*31:0*/ SoC__DOT__cpu__DOT__fwd_rs2;
        IData/*31:0*/ SoC__DOT__cpu__DOT__execute_rd;
        IData/*31:0*/ SoC__DOT__cpu__DOT__execute_pc_next;
        IData/*31:0*/ SoC__DOT__cpu__DOT__execute_mem_address;
        IData/*31:0*/ SoC__DOT__cpu__DOT__execute_mem_wdata;
        IData/*31:0*/ SoC__DOT__cpu__DOT__memory_rd;
        IData/*31:0*/ SoC__DOT__cpu__DOT__memory_pc_next;
        IData/*31:0*/ SoC__DOT__cpu__DOT__writeback_rd;
        IData/*31:0*/ SoC__DOT__cpu__DOT__writeback_pc_next;
        IData/*31:0*/ SoC__DOT__cpu__DOT__bus__DOT__i_pa_wdata;
        IData/*31:0*/ SoC__DOT__cpu__DOT__fetch__DOT__pc;
        IData/*31:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache_rdata;
        WData/*1023:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[32];
        IData/*31:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_rdata;
        IData/*31:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__address;
        IData/*31:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__prefetch;
        IData/*31:0*/ SoC__DOT__cpu__DOT__execute__DOT__alu_operand1;
        IData/*31:0*/ SoC__DOT__cpu__DOT__execute__DOT__alu_operand2;
        IData/*31:0*/ SoC__DOT__cpu__DOT__execute__DOT__alu_result;
        IData/*31:0*/ SoC__DOT__cpu__DOT__execute__DOT__mul_op1;
        IData/*31:0*/ SoC__DOT__cpu__DOT__execute__DOT__mul_op2;
        IData/*31:0*/ SoC__DOT__cpu__DOT__execute__DOT__div_numerator;
        IData/*31:0*/ SoC__DOT__cpu__DOT__execute__DOT__div_denominator;
        IData/*31:0*/ SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p1_result;
        IData/*31:0*/ SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p1_remainder;
    };
    struct {
        IData/*31:0*/ SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p2_result;
        IData/*31:0*/ SoC__DOT__cpu__DOT__execute__DOT__divide__DOT__p2_remainder;
        IData/*31:0*/ SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata;
        IData/*31:0*/ SoC__DOT__cpu__DOT__memory__DOT__dcache_wdata;
        WData/*255:0*/ SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[8];
        WData/*255:0*/ SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[8];
        QData/*63:0*/ SoC__DOT__video_bus__DOT__fifo_wdata;
        QData/*63:0*/ SoC__DOT__video_bus__DOT__fifo__DOT__rdata;
        QData/*63:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_wdata;
        QData/*63:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rdata;
        QData/*63:0*/ SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p1;
        QData/*63:0*/ SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p2;
        QData/*63:0*/ SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_wdata;
        QData/*63:0*/ SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rdata;
        IData/*31:0*/ SoC__DOT__video_sram__DOT__data[64000];
        QData/*63:0*/ SoC__DOT__video_bus__DOT__fifo__DOT__data[1024];
        IData/*31:0*/ SoC__DOT__rom__DOT__data[401];
        IData/*31:0*/ SoC__DOT__ram__DOT__data[16384];
        IData/*31:0*/ SoC__DOT__sdram__DOT__data[33554432];
        CData/*7:0*/ SoC__DOT__uart__DOT__rx__DOT__rx_fifo__DOT__data[4];
        IData/*31:0*/ SoC__DOT__cpu__DOT__registers__DOT__r[32];
        QData/*63:0*/ SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data[1024];
        QData/*63:0*/ SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__data[256];
    };
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    CData/*0:0*/ SoC__DOT____Vcellinp__video_bus__i_cpu_request;
    CData/*0:0*/ SoC__DOT____Vcellinp__rom__i_request;
    CData/*0:0*/ SoC__DOT____Vcellinp__ram__i_request;
    CData/*0:0*/ SoC__DOT____Vcellinp__sdram__i_request;
    CData/*0:0*/ SoC__DOT____Vcellinp__led__i_request;
    CData/*0:0*/ SoC__DOT____Vcellinp__uart__i_request;
    CData/*0:0*/ SoC__DOT____Vcellinp__i2c__i_request;
    CData/*0:0*/ SoC__DOT____Vcellinp__sd__i_request;
    CData/*0:0*/ SoC__DOT__cpu__DOT____Vcellinp__fetch__i_stall;
    CData/*1:0*/ __Vdly__SoC__DOT__cpu__DOT__bus__DOT__state;
    CData/*2:0*/ __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__state;
    CData/*3:0*/ __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache_input_tag;
    CData/*3:0*/ __Vdly__SoC__DOT__cpu__DOT__fetch_tag;
    CData/*3:0*/ __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache_output_tag;
    CData/*0:0*/ __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch_request;
    CData/*1:0*/ __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__state;
    CData/*0:0*/ __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache_rw;
    CData/*0:0*/ __Vdlyvset__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data__v0;
    CData/*3:0*/ __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__prefetch__DOT__state;
    CData/*3:0*/ __Vdly__SoC__DOT__cpu__DOT__decode_tag;
    CData/*3:0*/ __Vdly__SoC__DOT__cpu__DOT__execute_tag;
    CData/*7:0*/ __Vdly__SoC__DOT__cpu__DOT__execute__DOT__cycle;
    CData/*0:0*/ __Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_signed;
    CData/*4:0*/ __Vdly__SoC__DOT__cpu__DOT__memory__DOT__state;
    CData/*3:0*/ __Vdly__SoC__DOT__cpu__DOT__memory_tag;
    CData/*4:0*/ __Vdly__SoC__DOT__cpu__DOT__memory_inst_rd;
    CData/*7:0*/ __Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__state;
    CData/*0:0*/ __Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache_rw;
    CData/*7:0*/ __Vdlyvdim0__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__data__v0;
    CData/*0:0*/ __Vdlyvset__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__data__v0;
    CData/*3:0*/ __Vdly__SoC__DOT__cpu__DOT__writeback_tag;
    CData/*0:0*/ __VinpClk__TOP__SoC__DOT__reset;
    CData/*0:0*/ __Vclklast__TOP__CLOCK_125_p;
    CData/*0:0*/ __Vclklast__TOP____VinpClk__TOP__SoC__DOT__reset;
    CData/*0:0*/ __Vchglast__TOP__SoC__DOT__reset;
    SData/*9:0*/ __Vdlyvdim0__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data__v0;
    IData/*31:0*/ SoC__DOT____Vcellout__video_bus__o_video_rdata;
    IData/*31:0*/ SoC__DOT__video_sram__DOT____Vlvbound1;
    IData/*31:0*/ SoC__DOT__cpu__DOT__bus__DOT__o_pa_rdata__out__en0;
    IData/*31:0*/ SoC__DOT__cpu__DOT__bus__DOT__o_pb_rdata__out__en1;
    IData/*31:0*/ __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__pc;
    WData/*1023:0*/ __Vdly__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__valid[32];
    IData/*31:0*/ __Vdly__SoC__DOT__cpu__DOT__bus_pa_address;
    IData/*31:0*/ __Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_numerator;
    IData/*31:0*/ __Vdly__SoC__DOT__cpu__DOT__execute__DOT__div_denominator;
    IData/*31:0*/ __Vdly__SoC__DOT__cpu__DOT__memory_rd;
    IData/*31:0*/ __Vdly__SoC__DOT__cpu__DOT__memory_pc_next;
    IData/*31:0*/ __Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache_rdata;
    WData/*255:0*/ __Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__valid[8];
    WData/*255:0*/ __Vdly__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__dirty[8];
    QData/*63:0*/ __Vdlyvval__SoC__DOT__cpu__DOT__fetch__DOT__icache__DOT__cache__DOT__data__v0;
    QData/*63:0*/ __Vdly__SoC__DOT__cpu__DOT__execute__DOT__multiply__DOT__p1;
    QData/*63:0*/ __Vdlyvval__SoC__DOT__cpu__DOT__memory__DOT__dcache__DOT__cache__DOT__data__v0;
    CData/*0:0*/ __Vm_traceActivity[5];
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    VSoC__Syms* __VlSymsp;  // Symbol table
    
    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VSoC);  ///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible with respect to DPI scope names.
    VSoC(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~VSoC();
    /// Trace signals in the model; called by application code
    void trace(VerilatedVcdC* tfp, int levels, int options = 0);
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(VSoC__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(VSoC__Syms* symsp, bool first);
  private:
    static QData _change_request(VSoC__Syms* __restrict vlSymsp);
    static QData _change_request_1(VSoC__Syms* __restrict vlSymsp);
  public:
    static void _combo__TOP__6(VSoC__Syms* __restrict vlSymsp);
  private:
    void _ctor_var_reset() VL_ATTR_COLD;
  public:
    static void _eval(VSoC__Syms* __restrict vlSymsp);
  private:
#ifdef VL_DEBUG
    void _eval_debug_assertions();
#endif  // VL_DEBUG
  public:
    static void _eval_initial(VSoC__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _eval_settle(VSoC__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _initial__TOP__1(VSoC__Syms* __restrict vlSymsp) VL_ATTR_COLD;
    static void _sequent__TOP__2(VSoC__Syms* __restrict vlSymsp);
    static void _sequent__TOP__3(VSoC__Syms* __restrict vlSymsp);
    static void _sequent__TOP__5(VSoC__Syms* __restrict vlSymsp);
    static void _sequent__TOP__7(VSoC__Syms* __restrict vlSymsp);
    static void _settle__TOP__4(VSoC__Syms* __restrict vlSymsp) VL_ATTR_COLD;
  private:
    static void traceChgSub0(void* userp, VerilatedVcd* tracep);
    static void traceChgTop0(void* userp, VerilatedVcd* tracep);
    static void traceCleanup(void* userp, VerilatedVcd* /*unused*/);
    static void traceFullSub0(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceFullTop0(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceInitSub0(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceInitTop(void* userp, VerilatedVcd* tracep) VL_ATTR_COLD;
    void traceRegister(VerilatedVcd* tracep) VL_ATTR_COLD;
    static void traceInit(void* userp, VerilatedVcd* tracep, uint32_t code) VL_ATTR_COLD;
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
