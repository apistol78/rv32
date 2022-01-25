#include "Runtime/HAL/DMA.h"

#define DMA_FROM    (volatile uint32_t*)0x70000000
#define DMA_TO		(volatile uint32_t*)0x70000004
#define DMA_COUNT	(volatile uint32_t*)0x70000008
#define DMA_RUN		(volatile uint32_t*)0x7000000c

uint32_t dma_copy(void* dst, const void* src, uint32_t count)
{
	*DMA_FROM = (uint32_t)src;
	*DMA_TO = (uint32_t)dst;
	*DMA_COUNT = count;
	*DMA_RUN = 0x01010101;
}
