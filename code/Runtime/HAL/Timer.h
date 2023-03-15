/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include "Runtime/HAL/Common.h"

inline volatile uint32_t timer_get_ms()
{
 	volatile uint32_t ms;
	__asm__ volatile (
		"rdtime %0"
		: "=r" (ms)
	);
	return ms;   
}

inline volatile uint64_t timer_get_cycles()
{
	volatile uint32_t mtimeh;
	volatile uint32_t mtimel;
	volatile uint32_t tmp;

	for (;;)
	{
		__asm__ volatile ( "rdcycleh %0" : "=r" (tmp) );
		__asm__ volatile ( "rdcycle  %0" : "=r" (mtimel) );
		__asm__ volatile ( "rdcycleh %0" : "=r" (mtimeh) );
		if (mtimeh == tmp)
			break;
	}

	return (uint64_t)(
		(((uint64_t)mtimeh) << 32) | mtimel
	);
}

EXTERN_C void timer_wait_ms(uint32_t ms);
