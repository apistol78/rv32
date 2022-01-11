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

	// uint32_t sp, gp;
	// __asm__ volatile (
	// 	"mv %0, sp\n"
	// 	"mv %1, gp\n"
	// 	: "=r" (sp), "=r" (gp)
	// );

	// printf("SP: 0x%x\n", sp);
	// printf("GP: 0x%x\n", gp);

	// printf("** Initialize Video **\n");
	// if (video_init() != 0)
	// 	printf("Video init failed!\n");

	// printf("** Initialize SD card **\n");
	// if (sd_init() != 0)
	// 	printf("SD init failed!\n");

	// printf("** Initialize FS **\n");
	// if (file_init() != 0)
	// 	printf("FS init failed!\n");

	// printf("** Ready **\n");
    return 0;
}
