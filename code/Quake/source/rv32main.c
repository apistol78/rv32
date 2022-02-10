#ifndef PC

#include <stdio.h>
#include "Runtime/Runtime.h"
#include "Runtime/HAL/Timer.h"

void quake_main(int argc,char **argv);

uint64_t get_time(void)
{
    // uint64_t v_cycle = read_cycle();
    // return v_cycle * 1000000 / sysctl_clock_get_freq(SYSCTL_CLOCK_CPU);
	return timer_get_ms() * 1000ULL;
}

int main(void)
{
	runtime_init();
	quake_main(0,0);
	while(1);
}

#endif
