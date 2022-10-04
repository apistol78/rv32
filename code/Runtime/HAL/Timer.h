#pragma once

#include "Runtime/HAL/Common.h"

inline uint32_t timer_get_ms()
{
 	uint32_t ms;
	__asm__ volatile (
		"rdtime %0"
		: "=r" (ms)
	);
	return ms;   
}

inline uint64_t timer_get_cycles()
{
	uint32_t mtimeh;
	uint32_t mtimel;
	__asm__ volatile (
"again:\n"		
		"rdcycleh	x6\n"
		"rdcycle	%1\n"
		"rdcycleh	%0\n"
		"bne		x6, %0, again\n"
		: "=r" (mtimeh), "=r" (mtimel)
	);
	return (uint64_t)(
		(((uint64_t)mtimeh) << 32) | mtimel
	);
}

EXTERN_C void timer_wait_ms(uint32_t ms);
