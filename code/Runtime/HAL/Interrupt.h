/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include "Runtime/HAL/Common.h"

#define IRQ_SOURCE_TIMER	0
#define IRQ_SOURCE_PLIC_0	1
#define IRQ_SOURCE_PLIC_1	2
#define IRQ_SOURCE_PLIC_2	3
#define IRQ_SOURCE_PLIC_3	4
#define IRQ_SOURCE_ECALL	5

typedef void (*irq_handler_t)(uint32_t source);

EXTERN_C void interrupt_init();

EXTERN_C void interrupt_set_handler(uint32_t source, irq_handler_t handler);

EXTERN_C irq_handler_t* interrupt_get_handler(uint32_t source);

EXTERN_C void interrupt_enable();

EXTERN_C void interrupt_disable();
