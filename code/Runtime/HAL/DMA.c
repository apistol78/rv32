#include "Runtime/Kernel.h"
#include "Runtime/HAL/DMA.h"

#define DMA_FROM    (volatile uint32_t*)(DMA_BASE)
#define DMA_TO		(volatile uint32_t*)(DMA_BASE + 0x04)
#define DMA_COUNT	(volatile uint32_t*)(DMA_BASE + 0x08)
#define DMA_RUN		(volatile uint32_t*)(DMA_BASE + 0x0c)

uint32_t dma_write(void* dst, uint32_t count, uint32_t value)
{
	*DMA_FROM = value;
	*DMA_TO = (uint32_t)dst;
	*DMA_COUNT = count;
	*DMA_RUN = 1;
	return 0;
}

uint32_t dma_copy(void* dst, const void* src, uint32_t count)
{
	*DMA_FROM = (uint32_t)src;
	*DMA_TO = (uint32_t)dst;
	*DMA_COUNT = count;
	*DMA_RUN = 2;
	return 0;
}

void dma_wait()
{
	while (*DMA_RUN != 0)
		kernel_yield();
}
