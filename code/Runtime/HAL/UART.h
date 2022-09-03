#pragma once

#include "Runtime/HAL/Common.h"

EXTERN_C void uart_tx_u8(uint8_t port, uint8_t data);

EXTERN_C uint32_t uart_rx_full(uint8_t port);

EXTERN_C uint32_t uart_rx_empty(uint8_t port);

EXTERN_C uint8_t uart_rx_u8(uint8_t port);

EXTERN_C uint16_t uart_rx_u16(uint8_t port);

EXTERN_C uint32_t uart_rx_u32(uint8_t port);