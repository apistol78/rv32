#pragma once

#include "Runtime/HAL/Common.h"

EXTERN_C int32_t video_init();

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
