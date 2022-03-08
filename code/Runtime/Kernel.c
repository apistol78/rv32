#include <stdlib.h>
#include <string.h>
#include "Runtime/Kernel.h"
#include "Runtime/HAL/CSR.h"
#include "Runtime/HAL/Interrupt.h"
#include "Runtime/HAL/Timer.h"

#define KERNEL_TIMER_RATE 1000000

static kernel_thread_t g_threads[16];
static int32_t g_current = 0;
static int32_t g_count = 1;

static void kernel_scheduler()
{
	__asm__ volatile (
		"addi	sp, sp, -256		\n"
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
		"fsw	f0, 128(sp)			\n"
		"fsw	f1, 132(sp)			\n"
		"fsw	f2, 136(sp)			\n"
		"fsw	f3, 140(sp)			\n"
		"fsw	f4, 144(sp)			\n"
		"fsw	f5, 148(sp)			\n"
		"fsw	f6, 152(sp)			\n"
		"fsw	f7, 156(sp)			\n"
		"fsw	f8, 160(sp)			\n"
		"fsw	f9, 164(sp)			\n"
		"fsw	f10, 168(sp)		\n"
		"fsw	f11, 172(sp)		\n"
		"fsw	f12, 176(sp)		\n"
		"fsw	f13, 180(sp)		\n"
		"fsw	f14, 184(sp)		\n"
		"fsw	f15, 188(sp)		\n"
		"fsw	f16, 192(sp)		\n"
		"fsw	f17, 196(sp)		\n"
		"fsw	f18, 200(sp)		\n"
		"fsw	f19, 204(sp)		\n"
		"fsw	f20, 208(sp)		\n"
		"fsw	f21, 212(sp)		\n"
		"fsw	f22, 216(sp)		\n"
		"fsw	f23, 220(sp)		\n"
		"fsw	f24, 224(sp)		\n"
		"fsw	f25, 228(sp)		\n"
		"fsw	f26, 232(sp)		\n"
		"fsw	f27, 236(sp)		\n"
		"fsw	f28, 240(sp)		\n"
		"fsw	f29, 244(sp)		\n"
		"fsw	f30, 248(sp)		\n"
		"fsw	f31, 252(sp)		\n"
	);

	// Save current interrupt return address and stack pointer.
	{
		kernel_thread_t* t = &g_threads[g_current];
		t->epc = csr_read_mepc();

		__asm__ volatile (
			"mv 	%0, sp\n"
			: "=r" (t->sp)
		);
	}

	// Update thread states.
	for (uint32_t i = 0; i < g_count; ++i)
	{
		if (g_threads[i].sleep > 0)
			g_threads[i].sleep--;
	}

	// Select new thread.
	for (;;)
	{
		if (++g_current >= g_count)
			g_current = 0;
		if (g_threads[g_current].sleep == 0)
			break;
	}

	// Restore new thread, patch current return address into it's stack.
	{
		kernel_thread_t* t = &g_threads[g_current];
		csr_write_mepc(t->epc);

		__asm__ volatile (
			"mv		sp, %0\n"
			:
			: "r" (t->sp)
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
		"flw	f0, 128(sp)			\n"
		"flw	f1, 132(sp)			\n"
		"flw	f2, 136(sp)			\n"
		"flw	f3, 140(sp)			\n"
		"flw	f4, 144(sp)			\n"
		"flw	f5, 148(sp)			\n"
		"flw	f6, 152(sp)			\n"
		"flw	f7, 156(sp)			\n"
		"flw	f8, 160(sp)			\n"
		"flw	f9, 164(sp)			\n"
		"flw	f10, 168(sp)		\n"
		"flw	f11, 172(sp)		\n"
		"flw	f12, 176(sp)		\n"
		"flw	f13, 180(sp)		\n"
		"flw	f14, 184(sp)		\n"
		"flw	f15, 188(sp)		\n"
		"flw	f16, 192(sp)		\n"
		"flw	f17, 196(sp)		\n"
		"flw	f18, 200(sp)		\n"
		"flw	f19, 204(sp)		\n"
		"flw	f20, 208(sp)		\n"
		"flw	f21, 212(sp)		\n"
		"flw	f22, 216(sp)		\n"
		"flw	f23, 220(sp)		\n"
		"flw	f24, 224(sp)		\n"
		"flw	f25, 228(sp)		\n"
		"flw	f26, 232(sp)		\n"
		"flw	f27, 236(sp)		\n"
		"flw	f28, 240(sp)		\n"
		"flw	f29, 244(sp)		\n"
		"flw	f30, 248(sp)		\n"
		"flw	f31, 252(sp)		\n"
		"addi	sp, sp, 256\n"
	);

	timer_set_compare(KERNEL_TIMER_RATE);
}

static void* kernel_alloc_stack()
{
	uint8_t* stack = malloc(65536);
	if (stack)
	{
		memset(stack, 0, 65536);
		return stack + 65536 - 256;
	}
	else
		return 0;
}

void kernel_init()
{
	// Initialize main thread.
	kernel_thread_t* t = &g_threads[0];
	t->sp = 0;
	t->epc = 0;
	t->sleep = 0;

	// Setup timer interrupt for kernel scheduler.
	timer_set_compare(KERNEL_TIMER_RATE);
	interrupt_set_handler(0, kernel_scheduler);
}

void kernel_create_thread(kernel_thread_fn_t fn)
{
	kernel_thread_t* t = &g_threads[g_count];
	t->sp = (uint32_t)kernel_alloc_stack();
	t->epc = (uint32_t)fn;
	t->sleep = 0;
	++g_count;
}

void kernel_yield()
{
	__asm__ volatile ("ecall");
}

void kernel_sleep(uint32_t ms)
{
	interrupt_disable();
	g_threads[g_current].sleep = (ms + 99) / 100;
	interrupt_enable();
	kernel_yield();
}

void kernel_cs_lock(volatile kernel_cs_t* cs)
{
	for (;;)
	{
		while (cs->counter != 0)
			__asm__ volatile ("nop");
		interrupt_disable();
		if (cs->counter == 0)
		{
			cs->counter = 1;
			interrupt_enable();
			return;
		}
	}
}

void kernel_cs_unlock(volatile kernel_cs_t* cs)
{
	cs->counter = 0;
}
