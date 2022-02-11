#include <stdio.h>
#include <stdint.h>
#include "Runtime/Runtime.h"
#include "Runtime/HAL/Timer.h"

void quake_main(int argc,char **argv);

uint64_t get_time(void)
{
	return timer_get_ms() * 1000ULL;
}

int main(void)
{
	runtime_init();
	quake_main(0,0);
	while(1);
}
