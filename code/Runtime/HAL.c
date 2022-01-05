#include "Runtime/HAL.h"
#include "Runtime/HAL/File.h"
#include "Runtime/HAL/Print.h"
#include "Runtime/HAL/SD.h"
#include "Runtime/HAL/Video.h"

int32_t hal_init()
{
	printLn("** Initialize Video **");
	video_init();

	printLn("** Initialize SD card **");
	sd_init();

	printLn("** Initialize FS **");
	file_init();

	printLn("** Ready **");
    return 0;
}
