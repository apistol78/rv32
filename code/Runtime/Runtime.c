#include <stdio.h>
#include "Runtime/CRT.h"
#include "Runtime/File.h"
#include "Runtime/Input.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/Audio.h"
#include "Runtime/HAL/Interrupt.h"
#include "Runtime/HAL/SD.h"
#include "Runtime/HAL/SystemRegisters.h"
#include "Runtime/HAL/UART.h"
#include "Runtime/HAL/Video.h"

// Needed by custom printf implementation.
void _putchar(char character)
{
	uart_tx_u8(0, character);
}

int32_t runtime_init()
{
	crt_init();

	switch (sysreg_read(SR_REG_DEVICE_ID))
	{
		case SR_DEVICE_ID_RV32T:
			printf("** DEVICE   : Rv32T **\n");
			break;
		case SR_DEVICE_ID_T_CV_GX:
			printf("** DEVICE   : Terasic Cyclone V GX **\n");
			break;
		case SR_DEVICE_ID_Q_CV_2:
			printf("** DEVICE   : QMTech Cyclone V 2 - 5CEFA2F23 **\n");
			break;
		case SR_DEVICE_ID_Q_T7:
			printf("** DEVICE   : QMTech Kintex-7 **\n");
			break;
		case SR_DEVICE_ID_RV32:
			printf("** DEVICE   : Rv32 **\n");
			break;
		case SR_DEVICE_ID_Q_CV_5:
			printf("** DEVICE   : QMTech Cyclone V 5 - 5CEFA5F23 **\n");
			break;
		default:
			printf("** DEVICE   : UNKNOWN **\n");
			break;
	}

	printf("** FREQUENCY: %d MHz **\n", sysreg_read(SR_REG_FREQUENCY) / 1000000);
	printf("** MEMORY   : %d KiB **\n", sysreg_read(SR_REG_RAM_SIZE) / 1024);

	printf("** Initialize IRQ handler **\n");
	interrupt_init();
	
	printf("** Initialize Video **\n");
	if (video_init() != 0)
		printf("Video init failed!\n");

	printf("** Initialize Audio **\n");
	audio_init();

	printf("** Initialize SD card **\n");
	if (sd_init() != 0)
		printf("SD init failed!\n");

	printf("** Initialize FS **\n");
	if (file_init() != 0)
		printf("FS init failed!\n");

	printf("** Initialize Input **\n");
	input_init();

	printf("** Ready **\n");
    return 0;
}

void runtime_update()
{
	input_update();

	// Check for reset signal on UART.
	if (!uart_rx_empty(0))
	{
		uint8_t cmd = uart_rx_u8(0);
		if (cmd == 0xff)
			runtime_cold_restart();
	}
}

void runtime_warm_restart()
{
	typedef void (*call_fn_t)();

	const uint32_t sp = 0x20000000 + sysreg_read(SR_REG_RAM_SIZE) - 0x10000;
	__asm__ volatile (
		"mv sp, %0	\n"
		:
		: "r" (sp)

	);

	const uint32_t addr = 0x00000000;
	((call_fn_t)addr)();
}

void runtime_cold_restart()
{
	sysreg_write(SR_REG_COLD_RESET, 1);
}
