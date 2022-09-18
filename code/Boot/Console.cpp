#include <stdarg.h>
#include <string.h>
#include "Runtime/Kernel.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/Video.h"
#include "Console.h"

// C64 style font.
#include "font8x8_c64.h"

#define FW 320
#define FH 200

char fb[40 * 20];
int s_x = 0;
int s_y = 0;
int s_cursor = 1;

static void draw_character(const unsigned char* font, char ch, int32_t col, int32_t row, uint8_t* framebuffer)
{
	for (int32_t x = 0; x < 8; ++x) {
		for (int32_t y = 0; y < 8; ++y) {
			const bool set = font[(ch - ' ') * 8 + y] & (1 << x);
			if (set) {
				framebuffer[(y + col * 8) + (x + row * 8) * FW] = 0;
			}
		}
	}
}

static void draw_console()
{
	video_clear(0);

	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();
	for (int y = 23; y < 200 - 23; ++y) {
		memset(&framebuffer[23 + y * 320], 1, 320 - 23 -23);
	}

	for (int y = 0; y < 20; ++y) {
		for (int x = 0; x < 40; ++x) {
			char ch = fb[x + y * 40];
			if (ch >= ' ') {
				draw_character(font8x8_c64, ch, x + 3, y + 3, framebuffer);
			}
		}
	}

	if (s_cursor)
	{
		for (int y = 0; y < 8; ++y) {
			for (int x = 1; x < 7; ++x) {
				framebuffer[(s_x + 3) * 8 + x + ((s_y + 3) * 8 + y) * FW] = 0;
			}
		}
	}

	video_swap();
}

static void putchar(char c)
{
	if (c == '\n')
	{
		s_x = 0;
		if (s_y < 18)
			s_y++;
		else
		{
			for (int i = 0; i < 18; ++i)
				memmove(&fb[i * 40], &fb[(i + 1) * 40], 40);
			memset(&fb[18 * 40], 0, 40);
		}
	}
    else if (c == '\r')
    {
        s_x = 0;
    }
	else if (c == '\b')
	{
		if (s_x > 0)
		{
			s_x--;
			fb[s_x + s_y * 40] = 0;
		}
	}
	else
    {
		if (s_x < 40 && s_y < 20) {
			fb[s_x + s_y * 40] = c;
		}
		s_x++;
	}
}

// public

void fb_init()
{
  	video_set_palette(0, 0x887ecb);
	video_set_palette(1, 0x50459b);
	video_set_palette(255, 0x887ecb); 

	kernel_create_thread([]()
	{
		for (;;)
		{
			const uint32_t t0 = timer_get_ms();
			kernel_sleep(200);
			const uint32_t t1 = timer_get_ms();
			printf("%d\n", t1 - t0);

			s_cursor = 1 - s_cursor;
			draw_console();
		}
	});
}

void fb_clear()
{
	memset(fb, 0, 40*20);
	s_x = 0;
	s_y = 0;
	draw_console();
}

void fb_render()
{
	// draw_console();
}

void fb_putc(char c)
{
	putchar(c);
    draw_console();
}

void fb_print(const char* str)
{
	for (const char* c = str; *c != 0; ++c)
        putchar(*c);
	draw_console();
}

void fb_printf(const char* str, ...)
{
	char buf[128];
	va_list args;
	va_start(args, str);
	vsnprintf(buf, sizeof(buf), str, args);
	va_end(args);
	fb_print(buf);
}
