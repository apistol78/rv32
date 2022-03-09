#pragma once

#include "Runtime/HAL/Common.h"

#define IRQ_SOURCE_TIMER	0
#define IRQ_SOURCE_PLIC_0	1
#define IRQ_SOURCE_PLIC_1	2
#define IRQ_SOURCE_PLIC_2	3
#define IRQ_SOURCE_PLIC_3	4
#define IRQ_SOURCE_ECALL	5

typedef void (*irq_handler_t)(uint32_t source);

EXTERN_C void interrupt_init();

EXTERN_C void interrupt_set_handler(uint32_t source, irq_handler_t handler);

EXTERN_C void interrupt_enable();

EXTERN_C void interrupt_disable();
