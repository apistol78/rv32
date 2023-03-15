/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include "Runtime/HAL/Common.h"

#define VMODE_320_200_8 0
#define VMODE_640_200_8 1
#define VMODE_320_400_8 2
#define VMODE_640_400_8 3

EXTERN_C int32_t video_init();

EXTERN_C int32_t video_set_mode(int32_t mode);

EXTERN_C void* video_create_target();

EXTERN_C void video_destroy_target(void* target);

EXTERN_C int32_t video_get_resolution_width();

EXTERN_C int32_t video_get_resolution_height();

EXTERN_C void video_set_palette(uint8_t index, uint32_t color);

EXTERN_C void* video_get_primary_target();

EXTERN_C void* video_get_secondary_target();

EXTERN_C void video_clear(uint8_t idx);

EXTERN_C void video_blit(const void* source);

EXTERN_C void video_present(int32_t waitVblank);

EXTERN_C void video_wait_vblank();
