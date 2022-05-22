#include <stdio.h>
#include "Runtime/HAL/Audio.h"
#include "Runtime/HAL/Interrupt.h"

static void audio_interrupt_handler()
{
	// Called when audio device's queue is half full of samples.
}

void audio_init()
{
	//interrupt_set_handler(IRQ_SOURCE_PLIC_1, audio_interrupt_handler);
}

uint32_t audio_get_queued()
{
	volatile uint32_t* audio = (volatile uint32_t*)AUDIO_BASE;
	return *audio & 255;
}

void audio_play_mono(const int16_t* samples, uint32_t nsamples)
{
	volatile int32_t* audio = (volatile int32_t*)AUDIO_BASE;
	for (uint32_t i = 0; i < nsamples; ++i)
	{
		*audio = samples[i];
		*audio = samples[i];
	}
}

void audio_play_stereo(const int16_t* samples, uint32_t nsamples)
{
	volatile int32_t* audio = (volatile int32_t*)AUDIO_BASE;
	for (uint32_t i = 0; i < nsamples; i += 2)
	{
		*audio = samples[i + 0];
		*audio = samples[i + 1];
	}
}
