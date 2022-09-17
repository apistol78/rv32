#pragma once

#include "Runtime/HAL/Common.h"

#define SR_REG_FLAGS         0x00
#define SR_REG_LEDS          0x01
#define SR_REG_FREQUENCY     0x02
#define SR_REG_DEVICE_ID     0x03
#define SR_REG_RAM_SIZE      0x04
#define SR_REG_COLD_RESET    0x05

#define SR_DEVICE_ID_RV32T   1
#define SR_DEVICE_ID_T_CV_GX 2
#define SR_DEVICE_ID_Q_CV_2  3
#define SR_DEVICE_ID_Q_T7    4
#define SR_DEVICE_ID_RV32    5
#define SR_DEVICE_ID_Q_CV_5  6

EXTERN_C void sysreg_write(uint8_t reg, uint32_t value);

EXTERN_C void sysreg_modify(uint8_t reg, uint32_t mask, uint32_t value);

EXTERN_C uint32_t sysreg_read(uint8_t reg);
