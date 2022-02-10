#include <stdio.h>
#include "Runtime/File.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/CSR.h"
#include "Runtime/HAL/SD.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/UART.h"
#include "Runtime/HAL/Video.h"

extern void crt_init();


// // ========


// // ========
// // Kernel

// #define RATE 100000

// typedef struct
// {
// 	uint32_t ra;
// 	uint32_t sp;
// 	uint32_t s0;
// 	uint32_t s1;
// 	uint32_t s2;
// 	uint32_t s3;
// 	uint32_t s4;
// 	uint32_t s5;
// 	uint32_t s6;
// 	uint32_t s7;
// 	uint32_t s8;
// 	uint32_t s9;
// 	uint32_t s10;
// 	uint32_t s11;
// }
// threadContext_t;

// threadContext_t s_contexts[8];
// uint32_t s_context_count;
// uint32_t s_context_current;

// #pragma GCC push_options
// #pragma GCC optimize ("align-functions=4")
// static void __attribute__ ((interrupt ("machine"))) kernel_irq_entry(void)
// {
// 	// Save current context

// 	if (++s_context_current >= s_context_count)
// 		s_context_current = 0;

// 	// Load next context

// 	timer_set_compare(RATE);
// }
// #pragma GCC pop_options

// void kernel_create_thread(void (*fn)())
// {
// 	threadContext_t* ctx = &s_contexts[s_context_count];

// 	memset(ctx, 0, sizeof(threadContext_t));

// 	ctx->pc = (uint32_t)fn;
// 	ctx->r[2] = 0x10007ff0;

// 	++s_context_count;
// }

// void kernel_init()
// {
// 	// Global interrupt disable
// 	csr_clr_bits_mstatus(MSTATUS_MIE_BIT_MASK);
// 	csr_write_mie(0);

// 	// Setup kernel timer.
// 	timer_set_compare(RATE);

// 	// Setup the IRQ handler entry point
// 	csr_write_mtvec((uint_xlen_t)kernel_irq_entry);

// 	// Enable MIE.MTI
// 	csr_set_bits_mie(MIE_MTI_BIT_MASK);

// 	// Global interrupt enable 
// 	csr_set_bits_mstatus(MSTATUS_MIE_BIT_MASK);	

// 	// Allocate space for main thread.
// 	memset(&s_contexts[0], 0, sizeof(s_contexts[0]));
// 	s_context_count = 1;
// 	s_context_current = 0;
// }

// // ========


// void runtime_thread()
// {
// 	for (;;)
// 	{
// 		printf("** runtime thread! **\n");
// 		timer_wait_ms(1000);
// 	}
// }


int32_t __attribute__((used)) runtime_init()
{
	crt_init();

	printf("** Initialize Video **\n");
	if (video_init() != 0)
		printf("Video init failed!\n");

	printf("** Initialize SD card **\n");
	if (sd_init() != 0)
		printf("SD init failed!\n");

	printf("** Initialize FS **\n");
	if (file_init() != 0)
		printf("FS init failed!\n");

	// printf("** Initialize kernel **\n");
	// kernel_init();
	// kernel_create_thread(runtime_thread);

	printf("** Ready **\n");
    return 0;
}

void runtime_update()
{
	if (uart_rx_empty(1))
		return;

	uint8_t cmd = uart_rx_u8(1);
	if (cmd == 0x01)	// key state
		;
	else if (cmd == 0x02)	// mouse move
		;
	else if (cmd == 0x03)	// gamepad digital state
		;
	else if (cmd == 0x04)	// gamepad analog move
		;
}
