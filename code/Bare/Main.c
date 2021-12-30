#include "Print.h"
#include "SD.h"
#include "Video.h"

void draw_line(int x0, int y0, int x1, int y1)
{
	volatile uint32_t* video = VIDEO_BASE;
	int dx, dy, p, x, y;
 
	dx = x1 - x0;
	dy = y1 - y0;
	 
	x = x0;
	y = y0;
	 
	p = 2 * dy - dx;
	 
	while (x < x1)
	{
		if (p >= 0)
		{
			video[x + y * 320] = 0x00ffffff;
			y = y + 1;
			p = p + 2 * dy - 2 * dx;
		}
		else
		{
			video[x + y * 320] = 0x00ffffff;
			p = p + 2 * dy;
		}
		x = x + 1;
	}
}

void main()
{
	printLn("Initialize Video...");
	video_init();

	printLn("Initialize SD card...");
	sd_init();

	printLn("Ready");


	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;


	volatile uint32_t* video = VIDEO_BASE;
	for (;;)
	{
		for (int y = 0; y < 240; ++y)
		{
			for (int x = 0; x < 320; ++x)
			{
				video[x + y * 320] = (r << 16) | (g << 8) | b;
				r++;
			}
			g++;
		}

		b++;

		*VIDEO_CTRL = 1;
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
