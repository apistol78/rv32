#include "Runtime/HAL/Timer.h"

uint32_t timer_get_ms()
{
	uint32_t ms;
	__asm__ volatile (
		"rdtime %0"
		: "=r" (ms)
	);
	return ms;
}

void timer_wait_ms(uint32_t ms)
{
	const uint32_t until = timer_get_ms() + ms;
	while (timer_get_ms() < until)
	{
		__asm__ volatile (
			"nop	\n"
			"nop	\n"
			"nop	\n"
			"nop	\n"
			"nop	\n"
		);
	}
}

uint64_t timer_get_cycles()
{
	uint32_t mtimeh;
	uint32_t mtimel;
	__asm__ volatile (
		"rdcycleh	%0\n"
		"rdcycle	%1\n"
		: "=r" (mtimeh), "=r" (mtimel)
	);
	return (uint64_t)(
		(((uint64_t)mtimeh) << 32) | mtimel
	);
}
