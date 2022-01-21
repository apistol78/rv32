#pragma once

#include <stdint.h>

#define LED_BASE     (volatile uint32_t*)0x50000000
#define UART_BASE    (volatile uint32_t*)0x50000010
#define UART_STATUS  (volatile uint32_t*)0x50000014
#define GPIO_CTRL    (volatile uint32_t*)0x50000020
#define GPIO_IO      (volatile uint32_t*)0x50000024
#define I2C_BASE     (volatile uint32_t*)0x50000030
#define SD_BASE      (volatile uint32_t*)0x50000040

#define NO_OPTIMIZE __attribute__((optimize("O0")))