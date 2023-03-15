/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include "Runtime/HAL/Common.h"

EXTERN_C uint8_t crc7_byte(uint8_t crc, uint8_t data);

EXTERN_C uint8_t crc7(uint8_t crc, const uint8_t *buffer, int len);

EXTERN_C uint16_t crc16(const uint8_t *buffer, int len);