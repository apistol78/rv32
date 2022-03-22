#pragma once

#include "Kernal/HAL/Common.h"

typedef struct
{
	uint32_t sp;
	uint32_t epc;
	uint32_t sleep;
}
kernel_thread_t;

typedef struct
{
	uint32_t counter;
}
kernel_cs_t;

typedef void (*kernel_thread_fn_t)();

EXTERN_C void kernel_init();

EXTERN_C void kernel_create_thread(kernel_thread_fn_t fn);

EXTERN_C void kernel_yield();

EXTERN_C void kernel_sleep(uint32_t ms);

EXTERN_C void kernel_cs_lock(volatile kernel_cs_t* cs);

EXTERN_C void kernel_cs_unlock(volatile kernel_cs_t* cs);
