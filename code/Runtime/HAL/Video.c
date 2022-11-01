#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Runtime/Kernel.h"
#include "Runtime/HAL/ADV7513.h"
#include "Runtime/HAL/DMA.h"
#include "Runtime/HAL/Interrupt.h"
#include "Runtime/HAL/SIL9024A.h"
#include "Runtime/HAL/SystemRegisters.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/Video.h"

#define WIDTH 320
#define HEIGHT 200

#define VIDEO_DATA_BASE     VIDEO_BASE
#define VIDEO_PALETTE_BASE  (VIDEO_BASE + 0x00800000)
#define VIDEO_CONTROL_BASE	(VIDEO_BASE + 0x00800400)

static void* primary_target = 0;
static void* secondary_target = 0;

static uint32_t s_visible_offset = 0;
static uint32_t s_hidden_offset = WIDTH * HEIGHT;

static volatile kernel_sig_t vblank_signal;
static volatile int vblank = 0;

static void video_interrupt_handler()
{
	kernel_sig_raise(&vblank_signal);
	vblank++;
}

int32_t video_init()
{
	const uint32_t deviceId = sysreg_read(SR_REG_DEVICE_ID);
	if (deviceId == SR_DEVICE_ID_T_CV_GX)
	{
		if (adv7513_init())
			return 1;
	}
	else if (
		deviceId == SR_DEVICE_ID_Q_CV_2 ||
		deviceId == SR_DEVICE_ID_Q_CV_5 ||
		deviceId == SR_DEVICE_ID_Q_T7
	)
	{
		if (sil9024a_init())
			return 1;
	}

	primary_target = (uint32_t*)VIDEO_DATA_BASE;
	if ((secondary_target = malloc(WIDTH * HEIGHT)) == 0)
		return 1;		

	video_clear(0);
	memcpy(primary_target, secondary_target, WIDTH * HEIGHT);

	kernel_sig_init(&vblank_signal);
	interrupt_set_handler(IRQ_SOURCE_PLIC_0, video_interrupt_handler);
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

void* video_get_primary_target()
{
	return primary_target;
}

void* video_get_secondary_target()
{
	return secondary_target;
}

void video_clear(uint8_t idx)
{
	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();
	memset(framebuffer, idx, WIDTH * HEIGHT);
}

void video_swap(int32_t waitVblank)
{
	video_blit(waitVblank, secondary_target);
}

void video_blit(int32_t waitVblank, const uint8_t* source)
{
	uint8_t* ptr = (uint8_t*)primary_target;

	// Copy data onto hidden part of framebuffer.
	// __asm__ volatile ( "fence" );
	// dma_copy(ptr + s_hidden_offset, source, WIDTH * HEIGHT / 4);
	memcpy(ptr + s_hidden_offset, source, WIDTH * HEIGHT);

	// Wait until vblank occurs.
	// \todo in case vblank occurs during "fence" we should
	// track number of vblanks when entering this function and compensate...
	while (waitVblank > 0)
	{
		if (kernel_sig_try_wait(&vblank_signal, 400) == 0)
			printf("WARN: vblank wait failed (%d)\n", vblank);
		--waitVblank;
	}

	// Ensure DMA transfer is complete.
	// dma_wait();

	// Swap offsets.
	uint32_t tmp = s_hidden_offset;
	s_hidden_offset = s_visible_offset;
	s_visible_offset = tmp;

	// Set video offset to start reading from previously hidden part.
	volatile uint32_t* control = (volatile uint32_t*)VIDEO_CONTROL_BASE;
	control[0] = s_visible_offset;
}
