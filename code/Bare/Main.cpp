#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Runtime/Runtime.h"
#include "Runtime/HAL/DMA.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/Video.h"

uint8_t* framebuffer = (uint8_t*)0x10010000;

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
	maxX = min(maxX, 320 - 1);
	maxY = min(maxY, 200 - 1);

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
	uint32_t offset = minY * 320;
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

		offset += 320;
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


int main()
{
	runtime_init();

	volatile uint32_t* palette = VIDEO_PALETTE_BASE;
	volatile uint32_t* video = VIDEO_DATA_BASE;

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

			sv[i].x = (int32_t)((ndx * 100) + 160);
			sv[i].y = (int32_t)((ndy * 100) + 100);
		}

		dma_wait();

		memset(framebuffer, 0, 320 * 200);

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

		dma_copy(
			(void*)video,
			framebuffer,
			320 * 200 / 4
		);
		

		head += 0.043f;
		pitch += 0.067f;
		bank += 0.034f;
	}

	return 0;
}
