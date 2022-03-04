#include "Runtime/HAL/Interrupt.h"
#include "Runtime/HAL/Timer.h"

uint32_t timer_get_ms()
{
	return *TIMER_MS;
}

void timer_wait_ms(uint32_t ms)
{
	uint32_t until = timer_get_ms() + ms;
	while (timer_get_ms() < until)
		;
}

uint64_t timer_get_cycles()
{
	uint32_t mtimeh;
	uint32_t mtimel;

	interrupt_disable();

	do
	{
		mtimeh = *TIMER_CYCLES_H;
		mtimel = *TIMER_CYCLES_L;
	}
	while (mtimeh != *TIMER_CYCLES_H);

	interrupt_enable();

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
