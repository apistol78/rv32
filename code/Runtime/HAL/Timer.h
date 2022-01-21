#pragma once

#include "Runtime/HAL/Common.h"

uint32_t timer_get_ms();

void timer_wait_ms(uint32_t ms);

uint32_t timer_get_cycles();

uint32_t timer_get_retire();

uint32_t timer_get_icache_hit();

uint32_t timer_get_icache_miss();

uint32_t timer_get_dcache_hit();

uint32_t timer_get_dcache_miss();
