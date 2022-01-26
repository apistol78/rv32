#include "Runtime/HAL/DMA.h"

uint32_t dma_copy(void* dst, const void* src, uint32_t count)
{
	*DMA_FROM = (uint32_t)src;
	*DMA_TO = (uint32_t)dst;
	*DMA_COUNT = count;
	*DMA_RUN = 0x01010101;
}

void dma_wait()
{
	while (*DMA_RUN != 0)
	{
		__asm__ volatile ( "nop" );
		__asm__ volatile ( "nop" );
		__asm__ volatile ( "nop" );
		__asm__ volatile ( "nop" );
		__asm__ volatile ( "nop" );
		__asm__ volatile ( "nop" );
		__asm__ volatile ( "nop" );
		__asm__ volatile ( "nop" );
		__asm__ volatile ( "nop" );
		__asm__ volatile ( "nop" );
		__asm__ volatile ( "nop" );
		__asm__ volatile ( "nop" );
		__asm__ volatile ( "nop" );
		__asm__ volatile ( "nop" );
		__asm__ volatile ( "nop" );
		__asm__ volatile ( "nop" );
	}
}
