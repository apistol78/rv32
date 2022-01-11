#pragma once

#include "Runtime/HAL/Common.h"

int32_t sd_init();

int32_t sd_read_block512(uint32_t block, uint8_t* buffer, uint32_t bufferLen);
