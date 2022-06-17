#include <stdio.h>
#include "Runtime/HAL/Audio.h"
#include "Runtime/HAL/Interrupt.h"
#include "Runtime/HAL/SystemRegisters.h"

static uint32_t s_channels = 0;

static void audio_interrupt_handler()
{
	// Called when audio device's queue is half full of samples.
}

void audio_init()
{
	switch(sysreg_read(SR_REG_DEVICE_ID))
	{
	case SR_DEVICE_ID_RV32T:
	case SR_DEVICE_ID_Q_CV_2:
		s_channels = 1;
		break;

	default:
		s_channels = 2;
		break;
	}

	interrupt_set_handler(IRQ_SOURCE_PLIC_1, audio_interrupt_handler);
}

uint32_t audio_get_queued()
{
	volatile uint32_t* audio = (volatile uint32_t*)AUDIO_BASE;
	return *audio & 255;
}

void audio_play_mono(const int16_t* samples, uint32_t nsamples)
{
	volatile int32_t* audio = (volatile int32_t*)AUDIO_BASE;
	if (s_channels == 1)
	{
		for (uint32_t i = 0; i < nsamples; ++i)
			*audio = samples[i];
	}
	else if (s_channels == 2)
	{
		for (uint32_t i = 0; i < nsamples; ++i)
		{
			*audio = samples[i];
			*audio = samples[i];
		}
	}
}

void audio_play_stereo(const int16_t* samples, uint32_t nsamples)
{
	volatile int32_t* audio = (volatile int32_t*)AUDIO_BASE;
	if (s_channels == 1)
	{
		for (uint32_t i = 0; i < nsamples; i += 2)
			*audio = (samples[i + 0] + samples[i + 1]) >> 1;
	}
	else if (s_channels == 2)
	{
		for (uint32_t i = 0; i < nsamples; i += 2)
		{
			*audio = samples[i + 0];
			*audio = samples[i + 1];
		}
	}
}
