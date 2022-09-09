#include "Runtime/Kernel.h"
#include "Runtime/HAL/Timer.h"

#define TIMER_MS            (volatile uint32_t*)(TIMER_BASE)
#define TIMER_CYCLES_L      (volatile uint32_t*)(TIMER_BASE + 0x04)
#define TIMER_CYCLES_H      (volatile uint32_t*)(TIMER_BASE + 0x08)
#define TIMER_COMPARE_L     (volatile uint32_t*)(TIMER_BASE + 0x0c)
#define TIMER_COMPARE_H     (volatile uint32_t*)(TIMER_BASE + 0x10)
#define TIMER_RAISE			(volatile uint32_t*)(TIMER_BASE + 0x14)

uint32_t timer_get_ms()
{
	return *TIMER_MS;
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

	kernel_enter_critical();

	do
	{
		mtimeh = *TIMER_CYCLES_H;
		mtimel = *TIMER_CYCLES_L;
	}
	while (mtimeh != *TIMER_CYCLES_H);

	kernel_leave_critical();

	return (uint64_t)(
		(((uint64_t)mtimeh) << 32) | mtimel
	);
}

void timer_set_compare(uint64_t offset)
{
	kernel_enter_critical();
	const uint64_t tc = timer_get_cycles() + offset;
	*TIMER_COMPARE_H = 0xFFFFFFFF;
	*TIMER_COMPARE_L = (uint32_t)(tc & 0x0FFFFFFFFUL);
	*TIMER_COMPARE_H = (uint32_t)(tc >> 32);
	kernel_leave_critical();
}

void timer_raise()
{
	*TIMER_RAISE = 1;
}
