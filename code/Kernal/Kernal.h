#pragma once

#include "Kernal/HAL/Common.h"

typedef struct
{
	uint32_t major;
    uint32_t minor; 

	int32_t (*file_open)(const char* name);
	void (*file_close)(int32_t fd);
	int32_t (*file_size)(int32_t fd);
	int32_t (*file_seek)(int32_t file, int32_t offset, int32_t from);
	int32_t (*file_write)(int32_t file, const void* ptr, int32_t len);
	int32_t (*file_read)(int32_t file, void* ptr, int32_t len);

    void (*uart_tx_u8)(uint8_t port, uint8_t data);
    uint32_t (*uart_rx_full)(uint8_t port);
    uint32_t (*uart_rx_empty)(uint8_t port);
    uint8_t (*uart_rx_u8)(uint8_t port);

    uint32_t (*timer_get_ms)();
    void (*timer_wait_ms)(uint32_t ms);
    uint64_t (*timer_get_cycles)();
    void (*timer_set_compare)(uint64_t offset);

	uint32_t (*dma_copy)(void* dst, const void* src, uint32_t count);
	void (*dma_wait)();
}
kernal_t;
