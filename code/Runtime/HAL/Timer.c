#include "Runtime/Kernel.h"
#include "Runtime/HAL/Timer.h"

#define TIMER_MS            (volatile uint32_t*)(TIMER_BASE)
#define TIMER_CYCLES_L      (volatile uint32_t*)(TIMER_BASE + 0x1 * 0x04)
#define TIMER_CYCLES_H      (volatile uint32_t*)(TIMER_BASE + 0x2 * 0x04)
#define TIMER_COMPARE_L     (volatile uint32_t*)(TIMER_BASE + 0x9 * 0x04)
#define TIMER_COMPARE_H     (volatile uint32_t*)(TIMER_BASE + 0xa * 0x04)
#define TIMER_ENABLED		(volatile uint32_t*)(TIMER_BASE + 0xe * 0x04)
#define TIMER_RAISE			(volatile uint32_t*)(TIMER_BASE + 0xf * 0x04)

void timer_enable(int32_t channel)
{
	*TIMER_ENABLED |= 1 << channel;
}

void timer_disable(int32_t channel)
{
	*TIMER_ENABLED &= ~(1 << channel);
}

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

uint64_t timer_get_cycles(int32_t channel)
{
	uint32_t mtimeh;
	uint32_t mtimel;
	do
	{
		mtimeh = (TIMER_CYCLES_H)[channel * 2];
		mtimel = (TIMER_CYCLES_L)[channel * 2];
	}
	while (mtimeh != (TIMER_CYCLES_H)[channel * 2]);
	return (uint64_t)(
		(((uint64_t)mtimeh) << 32) | mtimel
	);
}

void timer_set_compare(uint64_t offset)
{
	const uint64_t tc = timer_get_cycles(0) + offset;
	*TIMER_COMPARE_H = 0xFFFFFFFF;
	*TIMER_COMPARE_L = (uint32_t)(tc & 0x0FFFFFFFFUL);
	*TIMER_COMPARE_H = (uint32_t)(tc >> 32);
}

void timer_raise()
{
	*TIMER_RAISE = 1;
}
