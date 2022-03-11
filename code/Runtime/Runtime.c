#include <stdio.h>
#include "Runtime/File.h"
#include "Runtime/Kernel.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/Interrupt.h"
#include "Runtime/HAL/SD.h"
#include "Runtime/HAL/UART.h"
#include "Runtime/HAL/Video.h"

extern void crt_init();

int32_t __attribute__((used)) runtime_init()
{
	crt_init();

	// uint32_t sp, gp;
	// __asm__ volatile (
	// 	"mv %0, sp\n"
	// 	"mv %1, gp\n"
	// 	: "=r" (sp), "=r" (gp)
	// );

	// printf("SP: 0x%x\n", sp);
	// printf("GP: 0x%x\n", gp);

	//printf("** Initialize IRQ handler **\n");
	//interrupt_init();
	
	printf("** Initialize Video **\n");
	if (video_init() != 0)
		printf("Video init failed!\n");

	printf("** Initialize SD card **\n");
	if (sd_init() != 0)
		printf("SD init failed!\n");

	printf("** Initialize FS **\n");
	if (file_init() != 0)
		printf("FS init failed!\n");

	//printf("** Initialize Kernel **\n");
	//kernel_init();

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
