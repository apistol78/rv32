/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include "Runtime/HAL/Common.h"

EXTERN_C void audio_init();

EXTERN_C void audio_set_playback_rate(uint32_t rate);

EXTERN_C uint32_t audio_get_queued();

EXTERN_C void audio_play_mono(const int16_t* samples, uint32_t nsamples);

EXTERN_C void audio_play_stereo(const int16_t* samples, uint32_t nsamples);
