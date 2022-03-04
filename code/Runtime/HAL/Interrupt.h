#pragma once

#include "Runtime/HAL/Common.h"

typedef void (*irq_handler_t)();

EXTERN_C void interrupt_init();

EXTERN_C void interrupt_set_handler(uint32_t source, irq_handler_t handler);

EXTERN_C void interrupt_enable();

EXTERN_C void interrupt_disable();
