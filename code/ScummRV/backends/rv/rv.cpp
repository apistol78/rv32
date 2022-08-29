#include <stdio.h>
#include <stdlib.h>
#include "Runtime/Runtime.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/Video.h"

#include "rv.h"

void OSystem_RebelV::init_size(uint w, uint h)
{
	if (runtime_init() != 0)
		return;

	for (int i = 0; i < 256; ++i)
	{
		const uint8_t r = rand();
		const uint8_t g = rand();
		const uint8_t b = rand();
		video_set_palette(
			i,
			(r << 16) |
			(g << 8) |
			(b)
		);
	}
}

void OSystem_RebelV::set_palette(const byte *colors, uint start, uint num)
{
	for (uint i = 0; i < num; ++i)
	{
		const uint8_t r = colors[i * 3 + 0];
		const uint8_t g = colors[i * 3 + 1];
		const uint8_t b = colors[i * 3 + 2];
		video_set_palette(
			start + i,
			(r << 16) |
			(g << 8) |
			(b)
		);
	}
}

void OSystem_RebelV::copy_rect(const byte *buf, int pitch, int x, int y, int w, int h)
{
	printf("copy_rect\n");

	// uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();
	// memcpy(framebuffer, buf, h * 320);
}

void OSystem_RebelV::move_screen(int dx, int dy, int height)
{
	printf("move_screen\n");
}

void OSystem_RebelV::update_screen()
{
	//printf("update_screen\n");
	video_swap();
}

void OSystem_RebelV::set_shake_pos(int shake_pos)
{
	printf("set_shake_pos\n");
}

bool OSystem_RebelV::show_mouse(bool visible)
{
	printf("show_mouse\n");
	return true;
}

void OSystem_RebelV::warp_mouse(int x, int y)
{
	printf("warp_mouse\n");
}

void OSystem_RebelV::set_mouse_cursor(const byte *buf, uint w, uint h, int hotspot_x, int hotspot_y)
{
	//printf("set_mouse_cursor\n");
}

uint32 OSystem_RebelV::get_msecs()
{
	return timer_get_ms();
}

void OSystem_RebelV::delay_msecs(uint msecs)
{
	timer_wait_ms(msecs);
}

void OSystem_RebelV::set_timer(TimerProc callback, int timer)
{
	printf("set_timer\n");
}

bool OSystem_RebelV::poll_event(Event *event)
{
	//printf("poll_event\n");
	return false;
}

bool OSystem_RebelV::set_sound_proc(SoundProc proc, void *param, SoundFormat format)
{
	printf("set_sound_proc\n");
	return true;
}

void OSystem_RebelV::clear_sound_proc()
{
	printf("clear_sound_proc\n");
}

bool OSystem_RebelV::poll_cdrom()
{
	printf("poll_cdrom\n");
	return false;
}

void OSystem_RebelV::play_cdrom(int track, int num_loops, int start_frame, int duration)
{
	printf("play_cdrom\n");
}

void OSystem_RebelV::stop_cdrom()
{
	printf("stop_cdrom\n");
}

void OSystem_RebelV::update_cdrom()
{
	printf("update_cdrom\n");
}

OSystem::MutexRef OSystem_RebelV::create_mutex()
{
	// printf("create_mutex\n");
	return 0;
}

void OSystem_RebelV::lock_mutex(MutexRef mutex)
{
	// printf("lock_mutex\n");
}

void OSystem_RebelV::unlock_mutex(MutexRef mutex)
{
	// printf("unlock_mutex\n");
}

void OSystem_RebelV::delete_mutex(MutexRef mutex)
{
	// printf("delete_mutex\n");
}

void OSystem_RebelV::show_overlay()
{
	printf("show_overlay\n");
}

void OSystem_RebelV::hide_overlay()
{
	printf("hide_overlay\n");
}

void OSystem_RebelV::clear_overlay()
{
	printf("clear_overlay\n");
	video_clear(0);
}

void OSystem_RebelV::grab_overlay(NewGuiColor *buf, int pitch)
{
	printf("grab_overlay\n");
	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();
	memcpy(buf, framebuffer, 320 * 200);
}

void OSystem_RebelV::copy_rect_overlay(const NewGuiColor *buf, int pitch, int x, int y, int w, int h)
{
	if (x < 0) {
		w += x;
		buf -= x;
		x = 0;
	}

	if (y < 0) {
		h += y; buf -= y * pitch;
		y = 0;
	}

	if (w > 320 - x) {
		w = 320 - x;
	}

	if (h > 200 - y) {
		h = 200 - y;
	}

	if (w <= 0 || h <= 0)
		return;

	printf("copy_rect_overlay %d, %d, %d, %d, %d\n", pitch, x, y, w, h);

	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();

	uint8_t* dst = framebuffer + y * 320 + x;
	do {
		memcpy(dst, buf, w);
		dst += 320;
		buf += pitch;
	} while (--h);

}

uint32 OSystem_RebelV::property(int param, Property *value)
{
	if (param == PROP_GET_SAMPLE_RATE)
		return 44100;
	else
		return 0;
}

void OSystem_RebelV::quit()
{
	printf("quit\n");
	runtime_cold_restart();
}

OSystem *OSystem_RebelV_create()
{ 
	return new OSystem_RebelV();
}
