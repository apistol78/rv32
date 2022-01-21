#include <stdio.h>
#include "doomgeneric.h"
#include "doomkeys.h"
#include "Runtime/HAL.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/UART.h"
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

void DG_Init()
{
	hal_init();
}

void DG_DrawFrame()
{
}

void DG_DrawFrame2(const uint8_t* frame, const uint32_t* colors)
{
	//measure();

	static uint32_t last_ms = 0;
	uint32_t ms = timer_get_ms();
	printf("%d fps\n", 1000 / (ms - last_ms));
	last_ms = ms;

	for (uint32_t i = 0; i < 256; ++i)
		(VIDEO_PALETTE_BASE)[i] = colors[i];

	const uint32_t* src = (const uint32_t*)frame;
	for (uint32_t i = 0; i < 320 * 200 / 4; ++i)
		(VIDEO_DATA_BASE)[i] = src[i];

	// uint32_t en = timer_get_ms();
	// printf("%d ms\n", en - st);
}

void DG_SleepMs(uint32_t ms)
{
	timer_wait_ms(ms);
}

uint32_t DG_GetTicksMs()
{
	return timer_get_ms();
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
	if (uart_rx_empty())
		return 0;

	uint8_t v = uart_rx_u8();
	switch (v & 0x7f)
	{
	case 1:	// Left
		*doomKey = KEY_LEFTARROW;
		break;
	case 2:	// Right
		*doomKey = KEY_RIGHTARROW;
		break;
	case 3:	// Up
		*doomKey = KEY_UPARROW;
		break;
	case 4: // Down
		*doomKey = KEY_DOWNARROW;
		break;
	case 5:	// Use
		*doomKey = KEY_USE;
		break;
	case 6:	// Fire
		*doomKey = KEY_FIRE;
		break;
	case 7:	// Escape
		*doomKey = KEY_ESCAPE;
		break;
	case 8:	// Enter
		*doomKey = KEY_ENTER;
		break;
	}

	if (v & 0x80)
		*pressed = 1;
	else
		*pressed = 0;

	return 1;
}

void DG_SetWindowTitle(const char* title)
{
}
