#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Runtime/Kernel.h"
#include "Runtime/HAL/CSR.h"
#include "Runtime/HAL/Interrupt.h"
#include "Runtime/HAL/SystemRegisters.h"
#include "Runtime/HAL/Timer.h"

// #define TIMER_MS            (volatile uint32_t*)(TIMER_BASE + 0x0 * 0x04)
// #define TIMER_CYCLES_L      (volatile uint32_t*)(TIMER_BASE + 0x1 * 0x04)
// #define TIMER_CYCLES_H      (volatile uint32_t*)(TIMER_BASE + 0x2 * 0x04)
// #define TIMER_COMPARE_L     (volatile uint32_t*)(TIMER_BASE + 0x3 * 0x04)
// #define TIMER_COMPARE_H     (volatile uint32_t*)(TIMER_BASE + 0x4 * 0x04)
#define TIMER_COUNTDOWN		(volatile uint32_t*)(TIMER_BASE + 0x5 * 0x04)

#define KERNEL_MAIN_CLOCK 			100000000
#define KERNEL_SCHEDULE_FREQUENCY	600
#define KERNEL_TIMER_RATE 			(KERNEL_MAIN_CLOCK / KERNEL_SCHEDULE_FREQUENCY)

typedef struct
{
	uint32_t sp;
	uint32_t epc;
	uint32_t sleep;
	volatile kernel_sig_t* waiting;
}
kernel_thread_t;

static volatile kernel_thread_t g_threads[16];
static volatile int32_t g_current = 0;
static volatile int32_t g_count = 0;
static volatile int32_t g_critical = 0;
static volatile int32_t g_schedule = 0;

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

#ifdef __riscv_flen
	__asm__ volatile (
		"addi	sp, sp, -128		\n"
		"fsw	f0, 0(sp)			\n"
		"fsw	f1, 4(sp)			\n"
		"fsw	f2, 8(sp)			\n"
		"fsw	f3, 16(sp)			\n"
		"fsw	f4, 20(sp)			\n"
		"fsw	f5, 24(sp)			\n"
		"fsw	f6, 28(sp)			\n"
		"fsw	f7, 32(sp)			\n"
		"fsw	f8, 36(sp)			\n"
		"fsw	f9, 40(sp)			\n"
		"fsw	f10, 44(sp)			\n"
		"fsw	f11, 48(sp)			\n"
		"fsw	f12, 52(sp)			\n"
		"fsw	f13, 56(sp)			\n"
		"fsw	f14, 60(sp)			\n"
		"fsw	f15, 64(sp)			\n"
		"fsw	f16, 68(sp)			\n"
		"fsw	f17, 72(sp)			\n"
		"fsw	f18, 76(sp)			\n"
		"fsw	f19, 80(sp)			\n"
		"fsw	f20, 84(sp)			\n"
		"fsw	f21, 88(sp)			\n"
		"fsw	f22, 92(sp)			\n"
		"fsw	f23, 96(sp)			\n"
		"fsw	f24, 100(sp)		\n"
		"fsw	f25, 104(sp)		\n"
		"fsw	f26, 108(sp)		\n"
		"fsw	f27, 112(sp)		\n"
		"fsw	f28, 116(sp)		\n"
		"fsw	f29, 120(sp)		\n"
		"fsw	f30, 124(sp)		\n"
		"fsw	f31, 128(sp)		\n"
	);
#endif

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
		uint32_t ms;
		__asm__ volatile (
			"rdtime %0"
			: "=r" (ms)
		);

		int32_t next = g_current;
		for (int32_t i = 0; i < g_count; ++i)
		{
			if (++next >= g_count)
				next = 0;
			volatile kernel_thread_t* t = &g_threads[next];
			if (t->sleep <= ms)
				break;
		}
		g_current = next;

		// Write new current thread to scratch so we can debug scheduling.
		// ++g_schedule;
		// __asm__ volatile (
		// 	"csrw	mscratch, %0\n"
		// 	:
		// 	: "r" ((g_schedule << 16) | g_current)
		// );
	}

	// Setup next timer interrupt, do this inline since we
	// cannot touch stack.
	*TIMER_COUNTDOWN = KERNEL_TIMER_RATE;

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


#ifdef __riscv_flen
	__asm__ volatile (
		"flw	f0, 0(sp)			\n"
		"flw	f1, 4(sp)			\n"
		"flw	f2, 8(sp)			\n"
		"flw	f3, 16(sp)			\n"
		"flw	f4, 20(sp)			\n"
		"flw	f5, 24(sp)			\n"
		"flw	f6, 28(sp)			\n"
		"flw	f7, 32(sp)			\n"
		"flw	f8, 36(sp)			\n"
		"flw	f9, 40(sp)			\n"
		"flw	f10, 44(sp)			\n"
		"flw	f11, 48(sp)			\n"
		"flw	f12, 52(sp)			\n"
		"flw	f13, 56(sp)			\n"
		"flw	f14, 60(sp)			\n"
		"flw	f15, 64(sp)			\n"
		"flw	f16, 68(sp)			\n"
		"flw	f17, 72(sp)			\n"
		"flw	f18, 76(sp)			\n"
		"flw	f19, 80(sp)			\n"
		"flw	f20, 84(sp)			\n"
		"flw	f21, 88(sp)			\n"
		"flw	f22, 92(sp)			\n"
		"flw	f23, 96(sp)			\n"
		"flw	f24, 100(sp)		\n"
		"flw	f25, 104(sp)		\n"
		"flw	f26, 108(sp)		\n"
		"flw	f27, 112(sp)		\n"
		"flw	f28, 116(sp)		\n"
		"flw	f29, 120(sp)		\n"
		"flw	f30, 124(sp)		\n"
		"flw	f31, 128(sp)		\n"
		"addi	sp, sp, 128			\n"
	);
#endif

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

	__asm__ volatile (
		"ret"
	);
}

static void* kernel_alloc_stack()
{
	const int32_t stackSize = 0x60000;
	uint8_t* stack = malloc(stackSize);
	if (stack)
	{
		memset(stack, 0, stackSize);
		return stack + stackSize - 0x2000;
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
	g_schedule = 0;

	// Setup timer interrupt for kernel scheduler.
	interrupt_set_handler(IRQ_SOURCE_TIMER, kernel_scheduler);
	*TIMER_COUNTDOWN = KERNEL_TIMER_RATE;

	// Ensure interrupts are enabled.
	csr_set_bits_mstatus(MSTATUS_MIE_BIT_MASK);
	csr_clr_bits_mstatus(MIP_MTI_BIT_MASK);
}

uint32_t kernel_create_thread(kernel_thread_fn_t fn)
{
	kernel_enter_critical();
	volatile kernel_thread_t* t = &g_threads[g_count];
	t->sp = (uint32_t)kernel_alloc_stack();
	t->epc = (uint32_t)fn;
	t->sleep = 0;
	t->waiting = 0;
	const uint32_t tid = g_count++;
	kernel_leave_critical();
	return tid;
}

uint32_t kernel_current_thread()
{
	return g_current;
}

void kernel_yield()
{
	csr_set_bits_mip(MIP_MTI_BIT_MASK);
}

void kernel_sleep(uint32_t ms)
{
	const uint32_t fin_ms = timer_get_ms() + ms;
	volatile kernel_thread_t* t = &g_threads[g_current];
	t->sleep = fin_ms;
	do
	{
		kernel_yield();
	}
	while (t->sleep >= timer_get_ms());
	t->sleep = 0;
}

void kernel_enter_critical()
{
	if (g_critical++ == 0)
		csr_clr_bits_mstatus(MSTATUS_MIE_BIT_MASK);
}

void kernel_leave_critical()
{
	if (--g_critical == 0)
		csr_set_bits_mstatus(MSTATUS_MIE_BIT_MASK);
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
		kernel_leave_critical();
	}
}

void kernel_cs_unlock(volatile kernel_cs_t* cs)
{
	kernel_enter_critical();
	if (cs->counter > 0)
		cs->counter--;
	kernel_leave_critical();
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
	while (fin_ms >= timer_get_ms())
	{
		if (sig->counter > 0)
			break;
		kernel_yield();
	}

	const int32_t result = (sig->counter > 0) ? 1 : 0;
	t->waiting = 0;

	return result;
}
