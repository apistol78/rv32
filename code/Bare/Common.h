#pragma once

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

#define LED_BASE    (volatile uint32_t*)0x50000000
#define UART_BASE   (volatile uint32_t*)0x50000010
#define GPIO_CTRL   (volatile uint32_t*)0x50000020
#define GPIO_IO     (volatile uint32_t*)0x50000024
#define I2C_BASE    (volatile uint32_t*)0x50000030
#define SD_BASE     (volatile uint32_t*)0x50000040
#define VIDEO_BASE  (volatile uint32_t*)0x40000000
#define VIDEO_CTRL  (volatile uint32_t*)0x4ffffff0