#include <stdlib.h>
#include <string.h>
#include "Runtime/HAL/ADV7513.h"
#include "Runtime/HAL/DMA.h"
#include "Runtime/HAL/SIL9024A.h"
#include "Runtime/HAL/SystemRegisters.h"
#include "Runtime/HAL/Video.h"

#define WIDTH 320
#define HEIGHT 200

#define VIDEO_DATA_BASE     VIDEO_BASE
#define VIDEO_PALETTE_BASE  (VIDEO_BASE + 0x00800000)
#define VIDEO_CONTROL_BASE  (VIDEO_BASE + 0x00810000)

static void* primary_target = 0;
static void* secondary_target = 0;
static int32_t current = 0;

int32_t video_init()
{
	volatile uint32_t* control = (volatile uint32_t*)VIDEO_CONTROL_BASE;

	const uint32_t deviceId = sysreg_read(SR_REG_DEVICE_ID);
	if (deviceId == SR_DEVICE_ID_T_CV_GX)
	{
		if (adv7513_init())
		{
			// printf("Failed to initialize ADV7513; unable to initialize video.\n");
			return 1;
		}
	}
	else if (
		deviceId == SR_DEVICE_ID_Q_CV_2 ||
		deviceId == SR_DEVICE_ID_Q_CV_5 ||
		deviceId == SR_DEVICE_ID_Q_T7
	)
	{
		if (sil9024a_init())
		{
			// printf("Failed to initialize SIL9024A; unable to initialize video.\n");
			return 1;
		}		
	}

	primary_target = (uint32_t*)VIDEO_DATA_BASE;
	if ((secondary_target = malloc(WIDTH * HEIGHT)) == 0)
		return 1;		

	control[0] = 0;	// GPU read offset
	control[1] = 0;	// CPU write offset	

	video_clear(0);
	video_swap();

	return 0;
}

int32_t video_get_resolution_width()
{
	return WIDTH;
}

int32_t video_get_resolution_height()
{
	return HEIGHT;
}

void video_set_palette(uint8_t index, uint32_t color)
{
	volatile uint32_t* palette = (volatile uint32_t*)VIDEO_PALETTE_BASE;
	palette[index] = color;
}

void* video_get_secondary_target()
{
	return secondary_target;
}

void video_clear(uint8_t idx)
{
	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();
	for (uint32_t i = 0; i < WIDTH * HEIGHT; ++i)
		framebuffer[i] = idx;	
}

void video_swap()
{
	memcpy(primary_target, secondary_target, WIDTH * HEIGHT);
}

void video_blit(const uint8_t* source)
{
	memcpy(primary_target, source, WIDTH * HEIGHT);
}
