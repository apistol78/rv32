#include <stdio.h>
#include <stdlib.h>
#include "Runtime/HAL.h"
#include "Runtime/HAL/Print.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/Video.h"

void measure()
{
	static uint32_t last_cycles = 0;
	static uint32_t last_retire = 0;
	static uint32_t last_hit = 0;
	static uint32_t last_miss = 0;
	static uint32_t last_dc_hit = 0;
	static uint32_t last_dc_miss = 0;

	uint32_t cycles = timer_get_cycles();
	uint32_t retire = timer_get_retire();
	uint32_t icache_hit = timer_get_icache_hit();
	uint32_t icache_miss = timer_get_icache_miss();
	uint32_t dcache_hit = timer_get_dcache_hit();
	uint32_t dcache_miss = timer_get_dcache_miss();

	{
		uint32_t dc = cycles - last_cycles;
		uint32_t dr = retire - last_retire;
		uint32_t cpi = (dc * 100) / dr;
		printf("%d cycles/inst\n", cpi);
	}
	{
		uint32_t dh = icache_hit - last_hit;
		uint32_t dm = icache_miss - last_miss;
		uint32_t ratio = (dh * 100) / (dh + dm);
		printf("%u icache hit\n", dh);
		printf("%u icache miss\n", dm);
		printf("%u %% icache ratio\n", ratio);
	}
	{
		uint32_t dh = dcache_hit - last_dc_hit;
		uint32_t dm = dcache_miss - last_dc_miss;
		uint32_t ratio = (dh * 100) / (dh + dm);
		printf("%u dcache hit\n", dh);
		printf("%u dcache miss\n", dm);
		printf("%u %% dcache ratio\n", ratio);
	}

	last_cycles = timer_get_cycles();
	last_retire = timer_get_retire();
	last_hit = timer_get_icache_hit();
	last_miss = timer_get_icache_miss();
	last_dc_hit = timer_get_dcache_hit();
	last_dc_miss = timer_get_dcache_miss();
}

void main()
{
	hal_init();

	printf("STARFIELD\n");

	int star[100][3];
	for (int i = 0; i < 100; ++i)
	{
		star[i][0] = rand() % 320;
		star[i][1] = rand() % 200;
		star[i][2] = (rand() % 6) + 1;
	}

	printf("RANDOMIZED...\n");

	volatile uint32_t* palette = VIDEO_PALETTE_BASE;
	volatile uint32_t* video = VIDEO_DATA_BASE;

		for (uint32_t i = 0; i < 256; ++i)
		{
			uint8_t r = (uint8_t)rand();
			uint8_t g = (uint8_t)rand();
			uint8_t b = (uint8_t)rand();
			palette[i] = (r << 16) | (g << 8) | b;
		}

	palette[0] = 0x00ff0000;
	palette[1] = 0x0000ff00;
	palette[2] = 0x000000ff;
	palette[3] = 0x00ffff00;

	for (;;)
	{

		for (uint32_t i = 0; i < 320 * 200 / 4; ++i)
		{
			uint32_t a = (uint8_t)rand();
			uint32_t b = (uint8_t)rand();
			uint32_t c = (uint8_t)rand();
			uint32_t d = (uint8_t)rand();
			video[i] = (a << 24) | (b << 16) | (c << 8) | d;
		}

		measure();
	}
}
