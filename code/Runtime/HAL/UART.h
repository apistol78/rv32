#pragma once

#include "Runtime/HAL/Common.h"

void uart_tx_u8(uint8_t data);

uint32_t uart_rx_full();

uint32_t uart_rx_empty();

uint8_t uart_rx_u8();
