#include "Runtime/HAL/Timer.h"

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
