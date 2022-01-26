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
