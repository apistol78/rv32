#include <stdio.h>
#include <stdlib.h>
#include "Runtime/Runtime.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/Video.h"

#include "rv.h"

bool OSystem_RebelV::init()
{
	if (runtime_init() != 0)
		return false;

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

	return true;
}

void OSystem_RebelV::init_size(uint w, uint h)
{
	printf("init_size %d * %d\n", w, h);
}

void OSystem_RebelV::set_palette(const byte *colors, uint start, uint num)
{
	printf("set_palette\n");
}

void OSystem_RebelV::copy_rect(const byte *buf, int pitch, int x, int y, int w, int h)
{
	printf("copy_rect\n");
}

void OSystem_RebelV::copy_screen(const byte *buf, int y, int h, const uint32* mask, bool doublebuffer)
{
	// printf("copy_screen, %d, %d\n", y, h);
	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();
	memcpy(framebuffer, buf, h * 320);
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

NewGuiColor OSystem_RebelV::RGBToColor(uint8 r, uint8 g, uint8 b)
{
	return 0;
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

void OSystem_RebelV::set_mouse_cursor(uint32 id, const byte *buf, const byte* mask, uint w, uint h, int hotspot_x, int hotspot_y)
{
	printf("set_mouse_cursor\n");
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
}

void OSystem_RebelV::grab_overlay(byte *buf, int pitch)
{
	printf("grab_overlay\n");
	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();
	memcpy(buf, framebuffer, 320 * 200);
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
