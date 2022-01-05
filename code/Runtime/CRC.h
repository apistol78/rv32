#pragma once

#include "Common.h"

uint8_t crc7_byte(uint8_t crc, uint8_t data);

uint8_t crc7(uint8_t crc, const uint8_t *buffer, int len);
