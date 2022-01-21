#include "Runtime/HAL/Timer.h"

#define TIMER_MS            (volatile uint32_t*)0x60000000
#define TIMER_CYCLES        (volatile uint32_t*)0x60000004
#define TIMER_RETIRE        (volatile uint32_t*)0x60000008
#define TIMER_ICACHE_HIT    (volatile uint32_t*)0x6000000c
#define TIMER_ICACHE_MISS   (volatile uint32_t*)0x60000010
#define TIMER_DCACHE_HIT    (volatile uint32_t*)0x60000014
#define TIMER_DCACHE_MISS   (volatile uint32_t*)0x60000018

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

uint32_t timer_get_cycles()
{
    return *TIMER_CYCLES;
}

uint32_t timer_get_retire()
{
    return *TIMER_RETIRE;
}

uint32_t timer_get_icache_hit()
{
    return *TIMER_ICACHE_HIT;
}

uint32_t timer_get_icache_miss()
{
    return *TIMER_ICACHE_MISS;
}

uint32_t timer_get_dcache_hit()
{
    return *TIMER_DCACHE_HIT;
}

uint32_t timer_get_dcache_miss()
{
    return *TIMER_DCACHE_MISS;
}
