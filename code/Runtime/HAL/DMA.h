/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include "Runtime/HAL/Common.h"

EXTERN_C uint32_t dma_write(void* dst, uint32_t count, uint32_t value);

EXTERN_C uint32_t dma_copy(void* dst, const void* src, uint32_t count);

EXTERN_C void dma_wait();
