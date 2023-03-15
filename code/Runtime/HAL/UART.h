/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include "Runtime/HAL/Common.h"

EXTERN_C void uart_tx_u8(uint8_t port, uint8_t data);

EXTERN_C uint32_t uart_rx_full(uint8_t port);

EXTERN_C uint32_t uart_rx_empty(uint8_t port);

EXTERN_C uint8_t uart_rx_u8(uint8_t port);

EXTERN_C uint16_t uart_rx_u16(uint8_t port);

EXTERN_C uint32_t uart_rx_u32(uint8_t port);