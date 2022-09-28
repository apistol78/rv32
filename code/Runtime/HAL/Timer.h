#pragma once

#include "Runtime/HAL/Common.h"

EXTERN_C void timer_enable(int32_t channel);

EXTERN_C void timer_disable(int32_t channel);

EXTERN_C uint32_t timer_get_ms();

EXTERN_C void timer_wait_ms(uint32_t ms);

EXTERN_C uint64_t timer_get_cycles(int32_t channel);

EXTERN_C void timer_set_compare(uint64_t offset);

EXTERN_C void timer_raise();
