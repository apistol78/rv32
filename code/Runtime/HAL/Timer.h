#pragma once

#include "Runtime/HAL/Common.h"

uint32_t timer_get_ms();

void timer_wait_ms(uint32_t ms);

uint64_t timer_get_cycles();

void timer_set_compare(uint64_t offset);

