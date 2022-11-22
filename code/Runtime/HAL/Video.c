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

#define MAX_WIDTH 640
#define MAX_HEIGHT 400

#define VIDEO_DATA_BASE     VIDEO_BASE
#define VIDEO_PALETTE_BASE  (VIDEO_BASE + 0x00800000)
#define VIDEO_CONTROL_BASE	(VIDEO_BASE + 0x00800400)

static void* s_primary_target = 0;
static uint32_t s_visible_offset = 0;
static uint32_t s_hidden_offset = MAX_WIDTH * MAX_HEIGHT;
static volatile kernel_sig_t s_vblank_signal;
static volatile int s_vblank = 0;
static int32_t s_mode = 0;

static const struct
{
	int32_t width;
	int32_t height;
	int32_t pixels;
	uint8_t skip;
}
c_modes[] =
{
	{ 320, 200, 320 * 200, 0b11 },
	{ 640, 200, 640 * 200, 0b10 },
	{ 320, 400, 320 * 400, 0b01 },
	{ 640, 400, 640 * 400, 0b00 }
};

static void video_interrupt_handler()
{
	kernel_sig_raise(&s_vblank_signal);
	s_vblank++;
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

	s_primary_target = (uint32_t*)VIDEO_DATA_BASE;

	video_set_mode(VMODE_320_200_8);

	kernel_sig_init(&s_vblank_signal);
	interrupt_set_handler(IRQ_SOURCE_PLIC_0, video_interrupt_handler);
	return 0;
}

int32_t video_set_mode(int32_t mode)
{
	volatile uint32_t* control = (volatile uint32_t*)VIDEO_CONTROL_BASE;
	control[1] = c_modes[mode].width;
	control[2] = c_modes[mode].skip;
	s_mode = mode;
}

void* video_create_target()
{
	void* target = malloc(c_modes[s_mode].pixels);
	memset(target, 0, c_modes[s_mode].pixels);
	return target;
}

void video_destroy_target(void* target)
{
	free(target);
}

int32_t video_get_resolution_width()
{
	return c_modes[s_mode].width;
}

int32_t video_get_resolution_height()
{
	return c_modes[s_mode].height;
}

void video_set_palette(uint8_t index, uint32_t color)
{
	volatile uint32_t* palette = (volatile uint32_t*)VIDEO_PALETTE_BASE;
	palette[index] = color;
}

void* video_get_primary_target()
{
	uint8_t* ptr = (uint8_t*)s_primary_target;
	return ptr + s_visible_offset;
}

void* video_get_secondary_target()
{
	uint8_t* ptr = (uint8_t*)s_primary_target;
	return ptr + s_hidden_offset;
}

void video_clear(uint8_t idx)
{
	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();
	memset(framebuffer, idx, c_modes[s_mode].pixels);
}

void video_blit(const void* source)
{
	void* target = video_get_secondary_target();

	// Copy data onto hidden part of framebuffer.
	// __asm__ volatile ( "fence" );
	// dma_copy(ptr + s_hidden_offset, source, WIDTH * HEIGHT / 4);
	memcpy(target, source, c_modes[s_mode].pixels);

	// Ensure DMA transfer is complete.
	// dma_wait();
}

void video_present(int32_t waitVblank)
{
	// Wait until vblank occurs.
	while (waitVblank > 0)
	{
		if (kernel_sig_try_wait(&s_vblank_signal, 400) == 0)
			printf("WARN: vblank wait failed (%d)\n", s_vblank);
		--waitVblank;
	}

	// Swap offsets.
	const uint32_t tmp = s_hidden_offset;
	s_hidden_offset = s_visible_offset;
	s_visible_offset = tmp;

	// Set video offset to start reading from previously hidden part.
	volatile uint32_t* control = (volatile uint32_t*)VIDEO_CONTROL_BASE;
	control[0] = s_visible_offset;
}