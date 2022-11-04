#include <stdio.h>
#include "Runtime/CRT.h"
#include "Runtime/File.h"
#include "Runtime/Input.h"
#include "Runtime/Kernel.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/Audio.h"
#include "Runtime/HAL/Interrupt.h"
#include "Runtime/HAL/SD.h"
#include "Runtime/HAL/SystemRegisters.h"
#include "Runtime/HAL/Timer.h"
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

	// Ensure we boot from SD card by default, even
	// after reset (through UART command).
	sysreg_modify(SR_REG_FLAGS, 1, 0);

	// Log device name.
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
	if (sd_init(SD_MODE_HW) != 0)
		printf("SD init failed!\n");

	printf("** Initialize FS **\n");
	if (file_init() != 0)
		printf("FS init failed!\n");

	printf("** Initialize Input **\n");
	input_init();

	printf("** Initialize Kernel **\n");
	kernel_init();

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
		{
			sysreg_modify(SR_REG_FLAGS, 1, 1);	// Set boot mode into UART mode to prepare FM to download ELF.
			runtime_cold_restart();
		}
	}
}

void runtime_warm_restart()
{
	typedef void (*call_fn_t)();

	video_set_palette(0, 0x00000000);
	video_clear(0);
	video_present(0);

	const uint32_t sp = 0x20000000 + sysreg_read(SR_REG_RAM_SIZE);
	__asm__ volatile (
		"mv sp, %0	\n"
		:
		: "r" (sp)

	);

	const uint32_t addr = 0x00000000;
	((call_fn_t)addr)();

	for (;;);
}

void runtime_cold_restart()
{
	video_set_palette(0, 0x00000000);
	video_clear(0);
	video_present(0);

	sysreg_modify(SR_REG_FLAGS, 0b100, 0b100);

	for (;;);
}
