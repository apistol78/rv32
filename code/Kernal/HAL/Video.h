#pragma once

#define VIDEO_DATA_BASE     (volatile uint32_t*)0x30000000
#define VIDEO_PALETTE_BASE  (volatile uint32_t*)0x31000000

EXTERN_C int32_t video_init();
