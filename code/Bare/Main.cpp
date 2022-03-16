#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Runtime/Kernel.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/DMA.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/Video.h"

uint8_t* framebuffer = 0; // (uint8_t*)0x10010000;

//

class Vec3
{
public:
	float x, y, z;

	Vec3() = default;

	Vec3(float x_, float y_, float z_): x(x_), y(y_), z(z_) {}
};





//

class Vec2i
{
public:
	int32_t x, y;

	Vec2i() = default;

	Vec2i(int32_t x_, int32_t y_) : x(x_), y(y_) {}	
};

int32_t orient2d(const Vec2i& a, const Vec2i& b, const Vec2i& c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

int32_t min3(int32_t a, int32_t b, int32_t c)
{
	int32_t t = a < b ? a : b;
	return t < c ? t : c;
}

int32_t max3(int32_t a, int32_t b, int32_t c)
{
	int t = a > b ? a : b;
	return t > c ? t : c;
}

int32_t min(int32_t a, int32_t b)
{
	return a < b ? a : b;
}

int32_t max(int32_t a, int32_t b)
{
	return a > b ? a : b;
}

void triangle(const Vec2i& v0, const Vec2i& v1, const Vec2i& v2, uint8_t color)
{
	// Compute triangle bounding box
	int32_t minX = min3(v0.x, v1.x, v2.x);
	int32_t minY = min3(v0.y, v1.y, v2.y);
	int32_t maxX = max3(v0.x, v1.x, v2.x);
	int32_t maxY = max3(v0.y, v1.y, v2.y);

	// Clip against screen bounds
	minX = max(minX, 0);
	minY = max(minY, 0);
	maxX = min(maxX, 240 - 1); //320 - 1);
	maxY = min(maxY, 320 - 1); // 200 - 1);

	// Triangle setup
	int32_t A01 = v0.y - v1.y, B01 = v1.x - v0.x;
	int32_t A12 = v1.y - v2.y, B12 = v2.x - v1.x;
	int32_t A20 = v2.y - v0.y, B20 = v0.x - v2.x;

	// Barycentric coordinates at minX/minY corner
	Vec2i p = { minX, minY };
	int32_t w0_row = orient2d(v1, v2, p);
	int32_t w1_row = orient2d(v2, v0, p);
	int32_t w2_row = orient2d(v0, v1, p);

	// Rasterize
	uint32_t offset = minY * 240; // 320;
	for (p.y = minY; p.y <= maxY; p.y++)
	{
		// Barycentric coordinates at start of row
		int32_t w0 = w0_row;
		int32_t w1 = w1_row;
		int32_t w2 = w2_row;

		for (p.x = minX; p.x <= maxX; p.x++)
		{
			// If p is on or inside all edges, render pixel.
			if (w0 >= 0 && w1 >= 0 && w2 >= 0)
				framebuffer[p.x + offset] = color;

			// One step to the right
			w0 += A12;
			w1 += A20;
			w2 += A01;
		}

		// One row step
		w0_row += B12;
		w1_row += B20;
		w2_row += B01;

		offset += 240; // 320;
	}	
}

// -------------------

const Vec3 vertices[] =
{
	{ -1.0f, -1.0f,  1.0f },
	{  1.0f, -1.0f,  1.0f },
	{ -1.0f,  1.0f,  1.0f },
	{  1.0f,  1.0f,  1.0f },
	{ -1.0f, -1.0f, -1.0f },
	{  1.0f, -1.0f, -1.0f },
	{ -1.0f,  1.0f, -1.0f },
	{  1.0f,  1.0f, -1.0f }
};

const int32_t indices[] =
{
	// Top
	2, 7, 6,
	2, 3, 7,

	//Bottom
	0, 4, 5,
	0, 5, 1,

	//Left
	0, 2, 6,
	0, 6, 4,

	//Right
	1, 7, 3,
	1, 5, 7,

	//Front
	0, 3, 2,
	0, 1, 3,

	//Back
	4, 6, 7,
	4, 7, 5
};



kernel_cs_t lock = { 0 };

void test_thread_a()
{
	for (;;)
	{
		//kernel_cs_lock(&lock);
		//printf("Thread 1...\n");
		//kernel_cs_unlock(&lock);
		*LED_BASE = 0xaa;
		kernel_sleep(1000);
	}
}

void test_thread_b()
{
	for (;;)
	{
		//kernel_cs_lock(&lock);
		//printf("Thread 2...\n");
		//kernel_cs_unlock(&lock);
		*LED_BASE = 0xbb;
		kernel_sleep(1000);
	}
}


void audio_play_sound(const int16_t* samples, uint32_t nsamples)
{
	volatile int32_t* audio = (volatile int32_t*)0xd0000000;
	for (uint32_t i = 0; i < nsamples; ++i)
		*audio = samples[i];
}


#define CS_M	0x80
#define RD_M	0x40
#define RW_M	0x20
#define RS_M	0x10

#define CS		0x08
#define RD		0x04
#define RW		0x02
#define RS		0x01

void lcd_delay()
{
	for (uint32_t i = 0; i < 1000; ++i)
		__asm__ volatile ( "nop" );
}

void lcd_long_delay()
{
	for (uint32_t i = 0; i < 10000; ++i)
		__asm__ volatile ( "nop" );
}

void lcd_write_register(uint8_t index, uint8_t value)
{
	volatile uint32_t* vindex = VIDEO_DATA_BASE;
	volatile uint32_t* vvalue = VIDEO_DATA_BASE + 1;

	*vindex = index;	//
	*vvalue = value;
}


const uint8_t c_initialize[] =
{
	0x01, 0,
	0xcb, 5, 0x39, 0x2c, 0x00, 0x34, 0x02,
	0xcf, 3, 0x00, 0xc1, 0x30,
	0xe8, 3, 0x85, 0x00, 0x78,
	0xea, 2, 0x00, 0x00,
	0xed, 4, 0x64, 0x03, 0x12, 0x81,
	0xf7, 1, 0x20,
	0xc0, 1, 0x10,
	0xc1, 1, 0x10,
	0xc5, 2, 0x3e, 0x28,
	0xc7, 1, 0x86,
	0x36, 1, 0x48,
	0x3a, 1, 0x55,
	0xb1, 2, 0x00, 0x18,
	0xb6, 3, 0x08, 0x82, 0x27,
	0xf2, 1, 0x00,
	0x26, 1, 0x01,
	0xe0, 15, 0x0f, 0x31, 0x2b, 0x0c, 0x0e, 0x08, 0x4e, 0xf1, 0x37, 0x07, 0x10, 0x03, 0x0e, 0x09, 0x00,
	0xe1, 15, 0x00, 0x0e, 0x14, 0x03, 0x11, 0x07, 0x31, 0xc1, 0x48, 0x08, 0x0f, 0x0c, 0x31, 0x36, 0x0f,
	0x11, 0,
	0x29, 1, 0x2c,
	//
	0x00
};

#define V_LCD_CMD	(VIDEO_DATA_BASE)
#define V_LCD_DATA	(VIDEO_DATA_BASE + 1)
#define V_LCD_CTRL	(VIDEO_DATA_BASE + 2)

void lcd_init()
{
	printf("Initialize...\n");

	// chip select, keep selected forever.
	*V_LCD_CTRL = 0x00000000;

	// output initialization sequence.
	const uint8_t* ptr = c_initialize;
	while (*ptr != 0)
	{
		uint8_t cmd = *ptr++;
		uint8_t na = *ptr++;

		*V_LCD_CMD = cmd;
		lcd_delay();

		for (uint8_t i = 0; i < na; ++i)
		{
			*V_LCD_DATA = *ptr++;
			lcd_delay();
		}
	}

	printf("Done\n");
}

static void lcd_send_u32(volatile uint32_t* vbase, uint32_t v)
{
	*vbase = v >> 24;
	*vbase = v >> 16;
	*vbase = v >> 8;
	*vbase = v;
}

static void lcd_set_address(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2)
{
	*V_LCD_CMD = 0x2a;
	lcd_send_u32(V_LCD_DATA, (x1 << 16) | x2);
	*V_LCD_CMD = 0x2b;
	lcd_send_u32(V_LCD_DATA, (y1 << 16) | y2);
}

static void lcd_send_burst(uint16_t color, uint32_t len)
{
	uint16_t blocks;
	uint8_t i, high_bit = color >> 8, low_bit = color;

	*V_LCD_CMD = 0x2c;

	*V_LCD_DATA = high_bit;
	*V_LCD_DATA = low_bit;

	len--;
	blocks = (uint16_t)(len / 64); //64 pixels/block

	while (blocks--)
	{
		i = 16;
		do {
			*V_LCD_DATA = high_bit;
			*V_LCD_DATA = low_bit;
			*V_LCD_DATA = high_bit;
			*V_LCD_DATA = low_bit;
			*V_LCD_DATA = high_bit;
			*V_LCD_DATA = low_bit;
			*V_LCD_DATA = high_bit;
			*V_LCD_DATA = low_bit;		
		} while (--i);
	}
	for (i = (uint8_t)len & 63; i--;) {
		*V_LCD_DATA = high_bit;
		*V_LCD_DATA = low_bit;
	}

	*V_LCD_CTRL = 0x80000000;
	lcd_delay();
	*V_LCD_CTRL = 0x00000000;
}

void lcd_clear(uint16_t color)
{
	lcd_set_address(0, 0, 240 - 1, 320 - 1);
	lcd_send_burst(color, 320 * 240);
}

void lcd_blit(const uint8_t* buffer, const uint16_t* palette)
{
	lcd_set_address(0, 0, 240 - 1, 320 - 1);

	*V_LCD_CMD = 0x2c;

	for (uint32_t x = 0; x < 320 * 240; ++x)
	{
		uint16_t c = palette[buffer[x]];
		*V_LCD_DATA = c >> 8;
		*V_LCD_DATA = c;
	}

	*V_LCD_CTRL = 0x80000000;
	lcd_delay();
	*V_LCD_CTRL = 0x00000000;		
}


int main()
{

	// for (;;)
	// {
	// 	for (uint32_t lv = 0; lv < 256; ++lv)
	// 	{
	// 		for (uint32_t i = 0; i < 20; ++i)
	// 		{
	// 			for (uint32_t j = 0; j < 256; ++j)
	// 			{
	// 				*LED_BASE = (j < lv) ? 0xff : 0x00;
	// 				__asm__ volatile ( "nop" );
	// 			}
	// 		}
	// 	}
	// }

	runtime_init();

	printf("Hello world!\n");

	lcd_init();

	// for (;;)
	// {
	// 	//uint16_t deviceCode = lcd_read_register(0x00);
	// 	//printf("Device code %04x\n", deviceCode);

	// 	// lcd_write_begin();
	// 	// for (int32_t i = 0; i < 1000; ++i)
	// 	// 	lcd_write_data(rand());
	// 	// lcd_write_end();

	// 	lcd_clear(rand());
	// }


	// kernel_create_thread(test_thread_a);
	// kernel_create_thread(test_thread_b);


	// {

	// 	int16_t data[1024];
	// 	for (int32_t i = 0; i < 1024; ++i)
	// 	{
	// 		float f = (i / 1024.0f);
	// 		float v = sin(f * 6.28f * 10.0f);
	// 		data[i] = (int16_t)(v * 32767.0f + 32767.0f);
	// 	}

	// 	for (int32_t i = 0;; ++i)
	// 	{
	// 		//printf("Playing %d...\n", i);
	// 		audio_play_sound(data, 1024);
	// 	}
	// }


	// volatile uint32_t* palette = VIDEO_PALETTE_BASE;
	// volatile uint32_t* video = VIDEO_DATA_BASE;

	framebuffer = (uint8_t*)malloc(320 * 240);

	uint16_t palette[256];

	for (uint32_t i = 0; i < 256; ++i)
	{
		uint8_t r = rand();
		uint8_t g = rand();
		uint8_t b = rand();
		palette[i] = (r << 16) | (g << 8) | b;
	}

	palette[0] = 0x00000000;
	palette[1] = 0x00ffffff;

	float head = 0.0f;
	float pitch = 0.0f;
	float bank = 0.0f;
	Vec2i sv[8];

	for (;;)
	{
		*LED_BASE = 0xdd;

		static int count = 0;
		if (++count >= 60)
		{
			static uint32_t last_ms = 0;
			uint32_t ms = timer_get_ms();

			//kernel_cs_lock(&lock);
			printf("%d fps\n", (60 * 1000) / (ms - last_ms));
			//kernel_cs_unlock(&lock);

			last_ms = ms;
			count = 0;
		}

		const float ca = cos(head);
		const float sa = sin(head);
		const float cp = cos(pitch);
		const float sp = sin(pitch);
		const float cb = cos(bank);
		const float sb = sin(bank);

		for (int32_t i = 0; i < 8; ++i)
		{
			float xa = vertices[i].x * ca - vertices[i].z * sa;
			float ya = vertices[i].y;
			float za = vertices[i].x * sa + vertices[i].z * ca;

			float xb = xa;
			float yb = ya * cp - za * sp;
			float zb = ya * sp + za * cp;

			float x = xb * cb - yb * sb;
			float y = xb * sb + yb * cb;
			float z = zb;

			z += 5.0f;

			float w = z * 0.5f;
			float ndx = x / w;
			float ndy = y / w;

			sv[i].x = (int32_t)((ndx * 100) + 120); // 160);
			sv[i].y = (int32_t)((ndy * 100) + 160); // 100);
		}

		//dma_wait();

		memset(framebuffer, 0, 320 * 240);

		for (int32_t i = 0; i < (sizeof(indices) / sizeof(indices[0])) / 3; ++i)
		{
			int32_t i0 = indices[i * 3 + 0];
			int32_t i1 = indices[i * 3 + 1];
			int32_t i2 = indices[i * 3 + 2];

			triangle(
				sv[i0],
				sv[i2],
				sv[i1],
				i + 1
			);			
		}

		/*
		dma_copy(
			(void*)video,
			framebuffer,
			320 * 200 / 4
		);
		*/
		//lcd_blit(framebuffer, palette);
		

		head += 0.043f;
		pitch += 0.067f;
		bank += 0.034f;
	}

	return 0;
}
