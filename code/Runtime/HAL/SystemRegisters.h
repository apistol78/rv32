#pragma once

#include "Runtime/HAL/Common.h"

#define SR_REG_BM0  0x00
#define SR_REG_LEDS 0x01

EXTERN_C void sysreg_write(uint8_t reg, uint32_t value);

EXTERN_C void sysreg_modify(uint8_t reg, uint32_t mask, uint32_t value);

EXTERN_C uint32_t sysreg_read(uint8_t reg);
