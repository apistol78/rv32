#include "Runtime/HAL/Timer.h"

uint32_t timer_get_ms()
{
    return *TIMER_BASE;
}

void timer_wait_ms(uint32_t ms)
{
    uint32_t until = timer_get_ms() + ms;
    while (timer_get_ms() < until)
        ;
}
