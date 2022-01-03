#include <stdlib.h>
#include "Print.h"
#include "SD.h"
#include "Video.h"

void main()
{
	printLn("Initialize Video...");
	//video_init();

	printLn("Initialize SD card...");
	//sd_init();

	printLn("Ready");


	static int star[100][3];
	for (int i = 0; i < 100; ++i)
	{
		star[i][0] = rand() % 320;
		star[i][1] = rand() % 240;
		star[i][2] = (rand() % 4) + 1;
	}

	uint8_t page = 0;

	volatile uint32_t* video = VIDEO_BASE;
	for (;;)
	{
		// clear
		for (uint32_t i = 0; i < 320 * 240; ++i)
			video[i] = 0;

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

/*
	int x1 = 50;
	for (;;)
	{
		int y1 = 50;
		int x2 = 200;
		int y2 = 150;

		draw_line(
			x1, y1,
			x2, y2
		);

		*VIDEO_CTRL = 1;

		if (++x1 > 300)
			x1 = 50;
	}
*/
}
