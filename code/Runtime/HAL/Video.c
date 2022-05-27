#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Runtime/HAL/ADV7513.h"
#include "Runtime/HAL/SIL9024A.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/Video.h"

#define VIDEO_DATA_BASE     VIDEO_BASE
#define VIDEO_PALETTE_BASE  (VIDEO_BASE + 0x00800000)
#define VIDEO_CONTROL_BASE  (VIDEO_BASE + 0x00810000)

static void* primary_target = 0;
static void* secondary_target = 0;
static int32_t current = 0;

int32_t video_init()
{
	if (timer_get_device_id() == TIMER_DEVICE_ID_RV32T)
	{
		secondary_target = (uint32_t*)VIDEO_DATA_BASE;
	}
	else if (timer_get_device_id() == TIMER_DEVICE_ID_T_CV_GX)
	{
		if (adv7513_init())
		{
			printf("Failed to initialize ADV7513; unable to initialize video.\n");
			return 1;
		}
		secondary_target = (uint32_t*)VIDEO_DATA_BASE;
	}
	else if (timer_get_device_id() == TIMER_DEVICE_ID_Q_CV_2)
	{
		if (sil9024a_init())
		{
			printf("Failed to initialize SIL9024A; unable to initialize video.\n");
			return 1;
		}		
		//secondary_target = (uint32_t*)VIDEO_DATA_BASE;
		primary_target = (uint32_t*)VIDEO_DATA_BASE;
		if ((secondary_target = malloc(320 * 200)) == 0)
			return 1;		
	}
	else if (timer_get_device_id() == TIMER_DEVICE_ID_Q_T7)
	{
		if (sil9024a_init())
		{
			printf("Failed to initialize SIL9024A; unable to initialize video.\n");
			return 1;
		}		
		secondary_target = (uint32_t*)VIDEO_DATA_BASE;
	}
	else if (timer_get_device_id() == TIMER_DEVICE_ID_RV32)
	{	
		primary_target = (uint32_t*)VIDEO_DATA_BASE;
		if ((secondary_target = malloc(320 * 200)) == 0)
			return 1;
	}
	else
	{
		printf("Unknown device; unable to initialize video.\n");
		return 1;
	}

	volatile uint32_t* control = (volatile uint32_t*)VIDEO_CONTROL_BASE;
	control[0] = 0;			// GPU read offset
	control[1] = 320 * 240;	// CPU write offset

	return 0;
}

int32_t video_get_resolution_width()
{
	return 320;
}

int32_t video_get_resolution_height()
{
	return 240;
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

void video_swap()
{
	if (timer_get_device_id() == TIMER_DEVICE_ID_RV32 || timer_get_device_id() == TIMER_DEVICE_ID_Q_CV_2)
	{
		memcpy(primary_target, secondary_target, 320 * 240);
	}
	else
	{
		volatile uint32_t* control = (volatile uint32_t*)VIDEO_CONTROL_BASE;
		if (current == 0)
		{
			control[0] = 320 * 240;	// GPU read offset
			control[1] = 0;			// CPU write offset
		}
		else
		{
			control[0] = 0;			// GPU read offset
			control[1] = 320 * 240;	// CPU write offset
		}
		current = 1 - current;
	}
}
