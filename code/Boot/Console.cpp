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

#define CC 40
#define CR 25

namespace
{

kernel_sig_t s_redraw;
char fb[CC * CR];
int s_x = 0;
int s_y = 0;
int s_cursor = 1;

}

static void draw_character(const unsigned char* font, char ch, int32_t col, int32_t row, uint8_t* framebuffer)
{
	for (int32_t x = 0; x < 8; ++x)
	{
		for (int32_t y = 0; y < 8; ++y)
		{
			const bool set = font[(ch - ' ') * 8 + y] & (1 << x);
			if (set)
				framebuffer[(y + col * 8) + (x + row * 8) * FW] = 1;
		}
	}
}

static void draw_console()
{
	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();

	video_clear(0);

	for (int y = 0; y < CR; ++y)
	{
		for (int x = 0; x < CC; ++x)
		{
			const char ch = fb[x + y * CC];
			if (ch >= ' ')
				draw_character(font8x8_c64, ch, x, y, framebuffer);
		}
	}

	if (s_cursor)
	{
		for (int y = 0; y < 8; ++y)
		{
			for (int x = 1; x < 7; ++x)
				framebuffer[s_x * 8 + x + (s_y * 8 + y) * FW] = 1;
		}
	}

	video_present(1);
}

static void putchar(char c)
{
	if (c == '\n')
	{
		s_x = 0;
		if (s_y < (CR - 1))
			s_y++;
		else
		{
			for (int i = 0; i < (CR - 1); ++i)
				memmove(&fb[i * CC], &fb[(i + 1) * CC], CC);
			memset(&fb[(CR - 1) * CC], 0, CC);
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
			fb[s_x + s_y * CC] = 0;
		}
	}
	else
    {
		if (s_x < CC && s_y < CR) {
			fb[s_x + s_y * CC] = c;
		}
		s_x++;
	}
}

// public

void fb_init()
{
	video_set_palette(0, 0xb11a79);
  	video_set_palette(1, 0xffffff);
	video_set_palette(255, 0x887ecb); 

	video_clear(0);

	kernel_sig_init(&s_redraw);

	kernel_create_thread([]()
	{
		for (;;)
		{
			kernel_sig_try_wait(&s_redraw, 200);
			s_cursor = 1 - s_cursor;
			draw_console();
		}
	});
}

void fb_clear()
{
	memset(fb, 0, CC * CR);
	s_x = 0;
	s_y = 0;
	kernel_sig_raise(&s_redraw);
}

void fb_putc(char c)
{
	putchar(c);
	kernel_sig_raise(&s_redraw);
}

void fb_print(const char* str)
{
	for (const char* c = str; *c != 0; ++c)
        putchar(*c);
	kernel_sig_raise(&s_redraw);
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
