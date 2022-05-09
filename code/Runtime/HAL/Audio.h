#pragma once

#include "Runtime/HAL/Common.h"

EXTERN_C void audio_init();

EXTERN_C uint32_t audio_get_queued();

EXTERN_C void audio_play_mono(const int16_t* samples, uint32_t nsamples);

EXTERN_C void audio_play_stereo(const int16_t* samples, uint32_t nsamples);
