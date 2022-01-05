#include <stdlib.h>
#include "Runtime/File.h"
#include "Runtime/Print.h"
#include "Runtime/SD.h"
#include "Runtime/Video.h"

void main()
{
	printLn("** Initialize Video...");
	video_init();

	printLn("** Initialize SD card...");
	sd_init();

	printLn("** Initialize FS...");
	file_init();

	printLn("** Ready");

	static int star[100][3];
	for (int i = 0; i < 100; ++i)
	{
		star[i][0] = rand() % 320;
		star[i][1] = rand() % 240;
		star[i][2] = (rand() % 4) + 1;
	}

	uint8_t page = 0;
	uint32_t cc = 0;

	volatile uint32_t* video = VIDEO_BASE;
	for (;;)
	{
		// clear
		for (uint32_t i = 0; i < 320 * 240; ++i)
			video[i] = cc;
		++cc;

		// draw stars.
		for (int i = 0; i < 100; ++i)
		{
			int p = star[i][0] + star[i][1] * 320;

			video[p] = 0xfffffff;

			star[i][0] += star[i][2];

			if (star[i][0] >= 320)
				star[i][0] = 0;
		}

		page = 1 - page;
		*VIDEO_CTRL = (uint32_t)page;
	}
}
