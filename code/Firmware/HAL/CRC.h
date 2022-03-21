#pragma once

#include "Firmware/HAL/Common.h"

EXTERN_C uint8_t crc7_byte(uint8_t crc, uint8_t data);

EXTERN_C uint8_t crc7(uint8_t crc, const uint8_t *buffer, int len);
