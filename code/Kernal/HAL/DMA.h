#pragma once

#include "Kernal/HAL/Common.h"

EXTERN_C uint32_t dma_copy(void* dst, const void* src, uint32_t count);

EXTERN_C void dma_wait();
