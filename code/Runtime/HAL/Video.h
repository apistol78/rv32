#pragma once

#include "Runtime/HAL/Common.h"

EXTERN_C int32_t video_init();

EXTERN_C int32_t video_get_resolution_width();

EXTERN_C int32_t video_get_resolution_height();

EXTERN_C void video_set_palette(uint8_t index, uint32_t color);

EXTERN_C void* video_get_secondary_target();

EXTERN_C void video_clear(uint8_t idx);

EXTERN_C void video_swap(int32_t waitVblank);

EXTERN_C void video_blit(int32_t waitVblank, const uint8_t* source);
