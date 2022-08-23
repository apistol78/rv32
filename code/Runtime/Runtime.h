#pragma once

#include "Runtime/HAL/Common.h"
#include "Runtime/printf/printf.h"

EXTERN_C int32_t runtime_init();

EXTERN_C void runtime_update();

EXTERN_C void runtime_warm_restart();

EXTERN_C void runtime_cold_restart();
