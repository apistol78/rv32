#include <stdio.h>
#include "doomgeneric.h"
#include "Runtime/HAL.h"
#include "Runtime/HAL/Timer.h"

uint32_t video_page = 0;

void DG_Init()
{
	hal_init();
}

void DG_DrawFrame()
{
	uint32_t offset = 0;
	for (int y = 0; y < DOOMGENERIC_RESY; ++y)
	{
		for (int x = 0; x < DOOMGENERIC_RESX; ++x)
		{
			uint32_t pixel = DG_ScreenBuffer[offset];
			(VIDEO_BASE)[offset] = pixel;
			++offset;
		}
	}
	video_page = 1 - video_page;
	*VIDEO_CTRL = video_page;
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
	return 0;
}

void DG_SetWindowTitle(const char* title)
{
}
