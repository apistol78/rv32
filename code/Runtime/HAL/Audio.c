#include "Runtime/HAL/Audio.h"

void audio_init()
{
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
