#include <stdio.h>
#include "doomgeneric.h"
#include "doomkeys.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/UART.h"
#include "Runtime/HAL/Video.h"

void DG_Init()
{
	runtime_init();
}

void DG_DrawFrame()
{
}

void DG_DrawFrame2(const uint8_t* frame, const uint32_t* colors)
{
	static int count = 0;
	if (++count >= 60)
	{
		static uint32_t last_ms = 0;
		uint32_t ms = timer_get_ms();
		printf("%d fps\n", (60 * 1000) / (ms - last_ms));
		last_ms = ms;
		count = 0;
	}

	for (uint32_t i = 0; i < 256; ++i)
		video_set_palette(i, colors[i]);

	video_swap();
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
	if (uart_rx_empty(0))
		return 0;

	uint8_t v = uart_rx_u8(0);
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
