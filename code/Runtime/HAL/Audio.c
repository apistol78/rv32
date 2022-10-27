#include "Runtime/HAL/Audio.h"
#include "Runtime/HAL/Interrupt.h"
#include "Runtime/HAL/SystemRegisters.h"

static uint32_t s_channels = 0;

void audio_init()
{
	switch(sysreg_read(SR_REG_DEVICE_ID))
	{
	case SR_DEVICE_ID_RV32:
	case SR_DEVICE_ID_RV32T:
	case SR_DEVICE_ID_Q_CV_2:
	case SR_DEVICE_ID_Q_CV_5:
		s_channels = 1;
		break;

	default:
		s_channels = 2;
		break;
	}
}

void audio_set_playback_rate(uint32_t rate)
{
	volatile int32_t* audio = (volatile int32_t*)AUDIO_BASE;
	const uint32_t freq = sysreg_read(SR_REG_FREQUENCY);
	audio[1] = freq / rate;
}

uint32_t audio_get_queued()
{
	return *(volatile uint32_t*)AUDIO_BASE;
}

void audio_play_mono(const int16_t* samples, uint32_t nsamples)
{
	volatile int32_t* audio = (volatile int32_t*)AUDIO_BASE;
	if (s_channels == 1)
	{
		uint32_t i = 0;
		for (; nsamples >= 4 && i < nsamples - 4; i += 4)
		{
			*audio = samples[i];
			*audio = samples[i + 1];
			*audio = samples[i + 2];
			*audio = samples[i + 3];
		}
		for (; i < nsamples; ++i)
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
		uint32_t i = 0;
		for (; nsamples >= 2 * 8 && i < nsamples - 2 * 8; i += 2 * 8)
		{
			{
				const int32_t lh = samples[i + 0];
				const int32_t rh = samples[i + 1];
				*audio = (int16_t)((lh + rh) >> 1);
			}
			{
				const int32_t lh = samples[i + 2];
				const int32_t rh = samples[i + 3];
				*audio = (int16_t)((lh + rh) >> 1);
			}
			{
				const int32_t lh = samples[i + 4];
				const int32_t rh = samples[i + 5];
				*audio = (int16_t)((lh + rh) >> 1);
			}
			{
				const int32_t lh = samples[i + 6];
				const int32_t rh = samples[i + 7];
				*audio = (int16_t)((lh + rh) >> 1);
			}
			{
				const int32_t lh = samples[i + 8];
				const int32_t rh = samples[i + 9];
				*audio = (int16_t)((lh + rh) >> 1);
			}
			{
				const int32_t lh = samples[i + 10];
				const int32_t rh = samples[i + 11];
				*audio = (int16_t)((lh + rh) >> 1);
			}
			{
				const int32_t lh = samples[i + 12];
				const int32_t rh = samples[i + 13];
				*audio = (int16_t)((lh + rh) >> 1);
			}
			{
				const int32_t lh = samples[i + 14];
				const int32_t rh = samples[i + 15];
				*audio = (int16_t)((lh + rh) >> 1);
			}			
		}
		for (; i < nsamples; i += 2)
		{
			const int32_t lh = samples[i + 0];
			const int32_t rh = samples[i + 1];
			*audio = (int16_t)((lh + rh) >> 1);
		}
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
