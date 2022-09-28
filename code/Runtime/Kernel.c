#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Runtime/Kernel.h"
#include "Runtime/HAL/CSR.h"
#include "Runtime/HAL/Interrupt.h"
#include "Runtime/HAL/SystemRegisters.h"
#include "Runtime/HAL/Timer.h"

#define TIMER_MS            (volatile uint32_t*)(TIMER_BASE)
#define TIMER_CYCLES_L      (volatile uint32_t*)(TIMER_BASE + 0x1 * 0x04)
#define TIMER_CYCLES_H      (volatile uint32_t*)(TIMER_BASE + 0x2 * 0x04)
#define TIMER_COMPARE_L     (volatile uint32_t*)(TIMER_BASE + 0x9 * 0x04)
#define TIMER_COMPARE_H     (volatile uint32_t*)(TIMER_BASE + 0xa * 0x04)
#define TIMER_ENABLED		(volatile uint32_t*)(TIMER_BASE + 0xe * 0x04)
#define TIMER_RAISE			(volatile uint32_t*)(TIMER_BASE + 0xf * 0x04)

#define KERNEL_MAIN_CLOCK 			100000000
#define KERNEL_SCHEDULE_FREQUENCY	1000
#define KERNEL_TIMER_RATE 			(KERNEL_MAIN_CLOCK / KERNEL_SCHEDULE_FREQUENCY)

typedef struct
{
	uint32_t sp;
	uint32_t epc;
	uint32_t sleep;
	volatile kernel_sig_t* waiting;
}
kernel_thread_t;

static kernel_thread_t g_threads[16];
static int32_t g_current = 0;
static int32_t g_count = 0;
static volatile int32_t g_critical = 0;

static __attribute__((naked)) void kernel_scheduler(uint32_t source)
{
	__asm__ volatile (
		"addi	sp, sp, -128		\n"
		"sw		x4, 16(sp)			\n"
		"sw		x5, 20(sp)			\n"
		"sw		x6, 24(sp)			\n"
		"sw		x7, 28(sp)			\n"
		"sw		x8, 32(sp)			\n"
		"sw		x9, 36(sp)			\n"
		"sw		x10, 40(sp)			\n"
		"sw		x11, 44(sp)			\n"
		"sw		x12, 48(sp)			\n"
		"sw		x13, 52(sp)			\n"
		"sw		x14, 56(sp)			\n"
		"sw		x15, 60(sp)			\n"
		"sw		x16, 64(sp)			\n"
		"sw		x17, 68(sp)			\n"
		"sw		x18, 72(sp)			\n"
		"sw		x19, 76(sp)			\n"
		"sw		x20, 80(sp)			\n"
		"sw		x21, 84(sp)			\n"
		"sw		x22, 88(sp)			\n"
		"sw		x23, 92(sp)			\n"
		"sw		x24, 96(sp)			\n"
		"sw		x25, 100(sp)		\n"
		"sw		x26, 104(sp)		\n"
		"sw		x27, 108(sp)		\n"
		"sw		x28, 112(sp)		\n"
		"sw		x29, 116(sp)		\n"
		"sw		x30, 120(sp)		\n"
		"sw		x31, 124(sp)		\n"
	);

	// Save current interrupt return address and stack pointer.
	{
		volatile kernel_thread_t* t = &g_threads[g_current];
		__asm__ volatile (
			"csrr	%0, mepc\n"
			"mv 	%1, sp\n"
			: "=r" (t->epc), "=r" (t->sp)
		);
	}

	// Select new thread.
	{
		const uint32_t ms = *TIMER_MS;
		for (int32_t i = 0; i < g_count + 1; ++i)
		{
			if (++g_current >= g_count)
				g_current = 0;

			volatile kernel_thread_t* t = &g_threads[g_current];
			if (t->waiting != 0 && t->waiting->counter > 0)
			{
				t->waiting = 0;
				break;
			}
			else if (t->waiting == 0 && t->sleep <= ms)
				break;
		}
	}

	// Restore new thread.
	{
		volatile kernel_thread_t* t = &g_threads[g_current];
		__asm__ volatile (
			"csrw	mepc, %0\n"
			"mv		sp, %1\n"
			:
			: "r" (t->epc), "r" (t->sp)
		);
	}

	__asm__ volatile (
		"lw		x4, 16(sp)			\n"
		"lw		x5, 20(sp)			\n"
		"lw		x6, 24(sp)			\n"
		"lw		x7, 28(sp)			\n"
		"lw		x8, 32(sp)			\n"
		"lw		x9, 36(sp)			\n"
		"lw		x10, 40(sp)			\n"
		"lw		x11, 44(sp)			\n"
		"lw		x12, 48(sp)			\n"
		"lw		x13, 52(sp)			\n"
		"lw		x14, 56(sp)			\n"
		"lw		x15, 60(sp)			\n"
		"lw		x16, 64(sp)			\n"
		"lw		x17, 68(sp)			\n"
		"lw		x18, 72(sp)			\n"
		"lw		x19, 76(sp)			\n"
		"lw		x20, 80(sp)			\n"
		"lw		x21, 84(sp)			\n"
		"lw		x22, 88(sp)			\n"
		"lw		x23, 92(sp)			\n"
		"lw		x24, 96(sp)			\n"
		"lw		x25, 100(sp)		\n"
		"lw		x26, 104(sp)		\n"
		"lw		x27, 108(sp)		\n"
		"lw		x28, 112(sp)		\n"
		"lw		x29, 116(sp)		\n"
		"lw		x30, 120(sp)		\n"
		"lw		x31, 124(sp)		\n"
		"addi	sp, sp, 128			\n"
	);

	// Setup next timer interrupt, do this inline since we
	// cannot touch stack.
	{
		const uint32_t mtimeh = *TIMER_CYCLES_H;
		const uint32_t mtimel = *TIMER_CYCLES_L;
		const uint64_t tc = ( (((uint64_t)mtimeh) << 32) | mtimel ) + KERNEL_TIMER_RATE;
		*TIMER_COMPARE_H = 0xFFFFFFFF;
		*TIMER_COMPARE_L = (uint32_t)(tc & 0x0FFFFFFFFUL);
		*TIMER_COMPARE_H = (uint32_t)(tc >> 32);
	}	

	__asm__ volatile (
		"ret"
	);
}

static void* kernel_alloc_stack()
{
	const int32_t stackSize = 0x40000;
	uint8_t* stack = malloc(stackSize);
	if (stack)
	{
		memset(stack, 0, stackSize);
		return stack + stackSize - 0x1000;
	}
	else
		return 0;
}

void kernel_init()
{
	volatile kernel_thread_t* t;

	// Initialize main thread.
	t = &g_threads[0];
	t->sp = 0;
	t->epc = 0;
	t->sleep = 0;
	t->waiting = 0;

	// Initialize counters.
	g_current = 0;
	g_count = 1;
	g_critical = 0;

	// Setup timer interrupt for kernel scheduler.
	timer_set_compare(KERNEL_TIMER_RATE);
	interrupt_set_handler(IRQ_SOURCE_TIMER, kernel_scheduler);
	timer_enable(0);
}

void kernel_create_thread(kernel_thread_fn_t fn)
{
	kernel_enter_critical();
	volatile kernel_thread_t* t = &g_threads[g_count];
	t->sp = (uint32_t)kernel_alloc_stack();
	t->epc = (uint32_t)fn;
	t->sleep = 0;
	t->waiting = 0;
	++g_count;
	kernel_leave_critical();
}

uint32_t kernel_current_thread()
{
	return g_current;
}

void kernel_yield()
{
	csr_read_set_bits_mip(MIP_MTI_BIT_MASK);
}

void kernel_sleep(uint32_t ms)
{
	volatile kernel_thread_t* t = &g_threads[g_current];

	const uint32_t fin_ms = timer_get_ms() + ms;
	t->sleep = fin_ms;

	do
	{
		kernel_yield();
	}
	while (t->sleep >= timer_get_ms());
}

void kernel_enter_critical()
{
	if (g_critical++ == 0)
		csr_clr_bits_mie(MIE_MTI_BIT_MASK);
}

void kernel_leave_critical()
{
	if (--g_critical == 0)
		csr_set_bits_mie(MIE_MTI_BIT_MASK);
}

void kernel_cs_init(volatile kernel_cs_t* cs)
{
	cs->counter = 0;
}

void kernel_cs_lock(volatile kernel_cs_t* cs)
{
	for (;;)
	{
		while (cs->counter != 0)
			kernel_yield();
		kernel_enter_critical();
		if (cs->counter == 0)
		{
			cs->counter++;
			kernel_leave_critical();
			return;
		}
	}
}

void kernel_cs_unlock(volatile kernel_cs_t* cs)
{
	if (cs->counter > 0)
		cs->counter--;
}

void kernel_sig_init(volatile kernel_sig_t* sig)
{
	sig->counter = 0;
}

void kernel_sig_raise(volatile kernel_sig_t* sig)
{
	sig->counter = 1;
	kernel_yield();
}

void kernel_sig_wait(volatile kernel_sig_t* sig)
{
	sig->counter = 0;

	volatile kernel_thread_t* t = &g_threads[g_current];
	t->waiting = sig;
	
	while (t->waiting)
		kernel_yield();
}

int32_t kernel_sig_try_wait(volatile kernel_sig_t* sig, uint32_t timeout)
{
	sig->counter = 0;

	volatile kernel_thread_t* t = &g_threads[g_current];
	t->waiting = sig;

	const uint32_t fin_ms = timer_get_ms() + timeout;
	do
	{
		if (!t->waiting)
			break;
		kernel_yield();
	}
	while (fin_ms >= timer_get_ms());

	const int32_t result = (t->waiting == 0) ? 1 : 0;
	t->waiting = 0;

	return result;
}
