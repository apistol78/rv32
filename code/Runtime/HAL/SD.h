#pragma once

#include "Runtime/HAL/Common.h"

void sd_init();

int sd_read_block512(uint32_t block, uint8_t* buffer, uint32_t bufferLen);
