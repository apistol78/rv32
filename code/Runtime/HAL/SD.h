#pragma once

#include "Runtime/HAL/Common.h"

#define SD_MODE_SW  0
#define SD_MODE_HW  1

EXTERN_C int32_t sd_init(int32_t mode);

EXTERN_C int32_t sd_read_block512(uint32_t block, uint8_t* buffer, uint32_t bufferLen);

EXTERN_C int32_t sd_write_block512(uint32_t block, const uint8_t* buffer, uint32_t bufferLen);