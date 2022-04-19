#pragma once

#include "Runtime/HAL/Common.h"

#define TIMER_DEVICE_ID_RV32T   1
#define TIMER_DEVICE_ID_T_CV_GX 2
#define TIMER_DEVICE_ID_Q_CV_2  3
#define TIMER_DEVICE_ID_Q_T7    4
#define TIMER_DEVICE_ID_RV32    5

EXTERN_C uint32_t timer_get_ms();

EXTERN_C void timer_wait_ms(uint32_t ms);

EXTERN_C uint64_t timer_get_cycles();

EXTERN_C void timer_set_compare(uint64_t offset);

EXTERN_C uint32_t timer_get_frequency();

EXTERN_C uint32_t timer_get_device_id();
