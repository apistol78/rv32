/*
   Simple machine mode timer driver for RISC-V standard timer.
   SPDX-License-Identifier: Unlicense

   (https://five-embeddev.com/) 

*/

#include "timer.h"


#define TIMER_CYCLES_L      (volatile uint32_t*)0xa0000004
#define TIMER_CYCLES_H      (volatile uint32_t*)0xa0000008
#define TIMER_COMPARE_L     (volatile uint32_t*)0xa000000c
#define TIMER_COMPARE_H     (volatile uint32_t*)0xa0000010

uint64_t timer_get_cycles()
{
	uint32_t mtimeh;
	uint32_t mtimel;
	do
	{
		mtimeh = *TIMER_CYCLES_H;
		mtimel = *TIMER_CYCLES_L;
	}
	while (mtimeh != *TIMER_CYCLES_H);
	return (uint64_t)(
		(((uint64_t)mtimeh) << 32) | mtimel
	);
}

void timer_set_compare(uint64_t offset)
{
	uint64_t tc = timer_get_cycles() + offset;
	*TIMER_COMPARE_H = 0xFFFFFFFF;
	*TIMER_COMPARE_L = (uint32_t)(tc & 0x0FFFFFFFFUL);
	*TIMER_COMPARE_H = (uint32_t)(tc >> 32);
}


void mtimer_set_raw_time_cmp(uint64_t clock_offset) {
    timer_set_compare(clock_offset);
}
 
uint64_t mtimer_get_raw_time(void) {
    return timer_get_cycles();
} 
