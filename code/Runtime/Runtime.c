#include <stdio.h>
#include "Runtime/File.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/SD.h"
#include "Runtime/HAL/Video.h"

extern void crt_init();

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

	printf("** Ready **\n");
    return 0;
}
