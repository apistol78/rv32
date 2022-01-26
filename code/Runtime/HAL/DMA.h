#pragma once

#include "Runtime/HAL/Common.h"

uint32_t dma_copy(void* dst, const void* src, uint32_t count);

void dma_wait();