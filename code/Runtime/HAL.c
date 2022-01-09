#include <stdio.h>
#include "Runtime/HAL.h"
#include "Runtime/HAL/File.h"
#include "Runtime/HAL/Print.h"
#include "Runtime/HAL/SD.h"
#include "Runtime/HAL/Video.h"

extern void crt_init();

int32_t NO_OPTIMIZE hal_init()
{
	crt_init();

	uint32_t sp, gp;
	__asm__ volatile (
		"mv %0, sp\n"
		"mv %1, gp\n"
		: "=r" (sp), "=r" (gp)
	);

	printf("SP: 0x%x\n", sp);
	printf("GP: 0x%x\n", gp);

	printf("** Initialize Video **\n");
	video_init();

	printf("** Initialize SD card **\n");
	sd_init();

	printf("** Initialize FS **\n");
	file_init();

	printf("** Ready **\n");
    return 0;
}
