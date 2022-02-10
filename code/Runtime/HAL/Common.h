#pragma once

#include <stdint.h>

#define LED_BASE     		(volatile uint32_t*)0x40000000

#define GPIO_CTRL    		(volatile uint32_t*)0x60000000
#define GPIO_IO      		(volatile uint32_t*)0x60000004

#define I2C_BASE     		(volatile uint32_t*)0x70000000

#define SD_BASE      		(volatile uint32_t*)0x80000000

#define DMA_FROM     		(volatile uint32_t*)0x90000000
#define DMA_TO		 		(volatile uint32_t*)0x90000004
#define DMA_COUNT	 		(volatile uint32_t*)0x90000008
#define DMA_RUN			 	(volatile uint32_t*)0x9000000c

#define TIMER_MS            (volatile uint32_t*)0xa0000000
#define TIMER_CYCLES_L      (volatile uint32_t*)0xa0000004
#define TIMER_CYCLES_H      (volatile uint32_t*)0xa0000008
#define TIMER_COMPARE_L     (volatile uint32_t*)0xa000000c
#define TIMER_COMPARE_H     (volatile uint32_t*)0xa0000010

#define NO_OPTIMIZE __attribute__((optimize("O0")))