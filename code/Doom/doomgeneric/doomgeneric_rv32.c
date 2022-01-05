#include "doomgeneric.h"
#include "Runtime/HAL.h"

uint32_t video_page = 0;

void DG_Init()
{
	hal_init();
}

void DG_DrawFrame()
{
	for (int y = 0; y < DOOMGENERIC_RESY; ++y)
	{
		for (int x = 0; x < DOOMGENERIC_RESX; ++x)
		{
			uint32_t offset = y * DOOMGENERIC_RESX + x;
			uint32_t pixel = DG_ScreenBuffer[offset];
			(VIDEO_BASE)[offset] = pixel;
		}
	}
	video_page = 1 - video_page;
	*VIDEO_CTRL = video_page;
}

void DG_SleepMs(uint32_t ms)
{
}

uint32_t DG_GetTicksMs()
{
	return 1000;
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
	return 0;
}

void DG_SetWindowTitle(const char* title)
{
}
