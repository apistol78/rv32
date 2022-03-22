#pragma once

#include "Kernal/HAL/Common.h"

EXTERN_C void i2c_write(uint8_t deviceAddr, uint8_t controlAddr, uint8_t controlData);

EXTERN_C void i2c_read(uint8_t deviceAddr, uint8_t controlAddr, uint8_t* outControlData);
