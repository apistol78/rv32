#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Runtime/Input.h"
#include "Runtime/File.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/Audio.h"
#include "Runtime/HAL/DMA.h"
#include "Runtime/HAL/SystemRegisters.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/UART.h"
#include "Runtime/HAL/Video.h"

#define FW 320
#define FH 200

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

void triangle(const Vec2i& v0, const Vec2i& v1, const Vec2i& v2, uint8_t* framebuffer, uint8_t color)
{
	// Compute triangle bounding box
	int32_t minX = min3(v0.x, v1.x, v2.x);
	int32_t minY = min3(v0.y, v1.y, v2.y);
	int32_t maxX = max3(v0.x, v1.x, v2.x);
	int32_t maxY = max3(v0.y, v1.y, v2.y);

	// Clip against screen bounds
	minX = max(minX, 0);
	minY = max(minY, 0);
	maxX = min(maxX, FW - 1);
	maxY = min(maxY, FH - 1);

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
	uint32_t offset = minY * FW;
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

		offset += FW;
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



// void splash_screen()
// {
// 	for (uint32_t i = 0; i < 256; ++i)
// 	{
// 		const uint8_t r = rand();
// 		const uint8_t g = rand();
// 		const uint8_t b = rand();
// 		video_set_palette(i, (r << 16) | (g << 8) | b);
// 	}

// 	video_set_palette(0, 0x00000000);
// 	video_set_palette(1, 0x00ffffff);

// 	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();

// 	for (uint32_t i = 0; i < FW * FH; ++i)
// 		framebuffer[i] = rand();	

// 	video_swap();	
// }



#include "font8x8/font8x8_latin.h"

void draw_character(char font[][8], char ch, int32_t col, int32_t row, uint8_t* framebuffer)
{
	for (int32_t x = 0; x < 8; ++x) {
		for (int32_t y = 0; y < 8; ++y) {
			const bool set = font[ch][x] & (1 << y);
			if (set) {
				framebuffer[(y + col * 8) + (x + row * 8) * FW] = 0xff;
			}
		}
	}
}

void draw_string(char font[][8], const char* str, int32_t col, int32_t row, uint8_t* framebuffer)
{
	while (*str) {
		draw_character(font, *str++, col++, row, framebuffer);
	}
}



int main()
{
	runtime_init();


	//splash_screen();


	
	// int32_t fp = file_open("music.raw");
	// if (fp >= 0)
	// {
	// 	const int32_t size = file_size(fp);
	// 	const int32_t n = size / 2;

	// 	printf("%d samples...\n", n);

	// 	int16_t buf[256];
	// 	for (;;)
	// 	{
	// 		if (file_read(fp, (uint8_t*)buf, 256 * 2) < 256 * 2)
	// 		{
	// 			printf("loop\n");
	// 			file_seek(fp, 0, 0);
	// 			continue;
	// 		}
	// 		audio_play_mono(buf, 256);
	// 	}
	// }


	for (uint32_t i = 0; i < 256; ++i)
	{
		const uint8_t r = rand();
		const uint8_t g = rand();
		const uint8_t b = rand();
		video_set_palette(i, (r << 16) | (g << 8) | b);
	}

	video_set_palette(0, 0x00000000);
	video_set_palette(255, 0x00ffffff);

	float head = 0.0f;
	float pitch = 0.0f;
	float bank = 0.0f;
	Vec2i sv[8];

	for (;;)
	{
		runtime_update();

		static int count = 0;
		if (++count >= 60)
		{
			static uint32_t last_ms = 0;
			uint32_t ms = timer_get_ms();
			printf("%d fps\n", (60 * 1000) / (ms - last_ms));
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

			sv[i].x = (int32_t)((ndx * (FW/2)) + (FW/2));
			sv[i].y = (int32_t)((ndy * (FH/2)) + (FH/2));
		}

		uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();
		memset(framebuffer, 0, FW * FH);

		for (int32_t i = 0; i < (sizeof(indices) / sizeof(indices[0])) / 3; ++i)
		{
			int32_t i0 = indices[i * 3 + 0];
			int32_t i1 = indices[i * 3 + 1];
			int32_t i2 = indices[i * 3 + 2];

			int32_t f = i >> 1;

			triangle(
				sv[i0],
				sv[i2],
				sv[i1],
				framebuffer,
				f + 1
			);			
		}


		draw_string(font8x8_basic, " Rebel V" , 16, 7, framebuffer);
		draw_string(font8x8_basic, "=========", 16, 8, framebuffer);
		draw_string(font8x8_basic, "1. Demo"  , 16, 9, framebuffer);
		draw_string(font8x8_basic, "2. Doom"  , 16, 10, framebuffer);
		draw_string(font8x8_basic, "3. Quake" , 16, 11, framebuffer);
		draw_string(font8x8_basic, "=========", 16, 12, framebuffer);

		video_swap();

		head += 0.043f;
		pitch += 0.067f;
		bank += 0.034f;
	}

	return 0;
}
