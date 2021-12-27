#pragma once

#include "Common.h"

void i2c_write(uint8_t deviceAddr, uint8_t controlAddr, uint8_t controlData);

void i2c_read(uint8_t deviceAddr, uint8_t controlAddr, uint8_t* outControlData);
