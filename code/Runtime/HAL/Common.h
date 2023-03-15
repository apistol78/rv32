/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include <stdint.h>

// Peripherials attached to bridge.
#define UART_0_BASE	0x51000000
#define UART_1_BASE	0x52000000
#define I2C_BASE	0x53000000
#define SD_BASE		0x54000000
#define TIMER_BASE	0x55000000
#define AUDIO_BASE	0x56000000
#define DMA_BASE	0x57000000
#define PLIC_BASE	0x58000000
#define SYSREG_BASE	0x59000000
#define VIDEO_BASE  0x5a000000

#define NO_OPTIMIZE	__attribute__((optimize("O0")))

#if defined(__cplusplus)
#   define EXTERN_C	extern "C"
#else
#   define EXTERN_C
#endif
