#pragma once

#include "Firmware/HAL/Common.h"

EXTERN_C int32_t sd_init();

EXTERN_C int32_t sd_read_block512(uint32_t block, uint8_t* buffer, uint32_t bufferLen);
