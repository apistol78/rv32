#pragma once

#define VIDEO_DATA_BASE     (volatile uint32_t*)0x40000000
#define VIDEO_PALETTE_BASE  (volatile uint32_t*)0x41000000

int32_t video_init();
