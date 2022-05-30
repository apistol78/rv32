#include <stdio.h>
#include "Runtime/CRT.h"
#include "Runtime/File.h"
#include "Runtime/Input.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/Audio.h"
#include "Runtime/HAL/Interrupt.h"
#include "Runtime/HAL/SD.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/UART.h"
#include "Runtime/HAL/Video.h"

int32_t runtime_init()
{
	crt_init();

	switch (timer_get_device_id())
	{
		case 1: printf("** DEVICE: Rv32T **\n"); break;
		case 2: printf("** DEVICE: Terasic Cyclone V GX **\n"); break;
		case 3: printf("** DEVICE: QMTech Cyclone V **\n"); break;
		case 4: printf("** DEVICE: QMTech Kintex-7 **\n"); break;
		case 5: printf("** DEVICE: Rv32 **\n"); break;
		default: printf("** DEVICE: UNKNOWN **\n"); break;
	}

	printf("** FREQUENCY: %d MHz **\n", timer_get_frequency() / 1000000);

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
}
