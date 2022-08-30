#include <stdio.h>
#include <stdlib.h>
#include "Runtime/Input.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/Video.h"

#include "rv.h"

namespace
{

int32_t clamp(int32_t v, int32_t mn, int32_t mx)
{
	return
		v < mn ? mn :
		v > mx ? mx :
		v;
}

}

void OSystem_RebelV::init_size(uint w, uint h)
{
	if (runtime_init() != 0)
		return;

	for (int i = 0; i < 256; ++i)
	{
		const uint32_t r = rand();
		const uint32_t g = rand();
		const uint32_t b = rand();
		video_set_palette(
			i,
			(r << 16) | (g << 8) | (b)
		);
	}
}

void OSystem_RebelV::set_palette(const byte *colors, uint start, uint num)
{
	for (uint i = 0; i < num; ++i)
	{
		const uint32_t rr = colors[0];
		const uint32_t gg = colors[1];
		const uint32_t bb = colors[2];
		video_set_palette(
			start + i,
			(rr << 16) | (gg << 8) | (bb)
		);
		colors += 4;
	}
}

void OSystem_RebelV::copy_rect(const byte *src, int pitch, int x, int y, int w, int h)
{
	x = x & ~7;
	w = (w + 7) & ~7;

	if (x < 0) {
		w += x;
		src -= x;
		x = 0;
	}

	if (y < 0) {
		h += y;
		src -= (y * pitch) >> 1;
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

	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();

	byte *dst = framebuffer + y * 320 + x;
	do {
		for(int i = 0; i < w; ++i)
			dst[i] = src[i]; // & 0xf;
		dst += 320;
		src += pitch;
	} while (--h);
}

void OSystem_RebelV::move_screen(int dx, int dy, int height)
{
	printf("move_screen\n");
}

void OSystem_RebelV::update_screen()
{
	//printf("update_screen\n");
	runtime_update();
	draw_mouse_cursor();
	video_swap();
}

void OSystem_RebelV::set_shake_pos(int shake_pos)
{
	printf("set_shake_pos\n");
}

bool OSystem_RebelV::show_mouse(bool visible)
{
	// printf("show_mouse\n");
	return true;
}

void OSystem_RebelV::warp_mouse(int x, int y)
{
	m_mouseX = x;
	m_mouseY = y;
}

void OSystem_RebelV::set_mouse_cursor(const byte *buf, uint w, uint h, int hotspot_x, int hotspot_y)
{
	m_mouseBits = buf;
	m_mouseW = w;
	m_mouseH = h;
	m_mouseHotX = hotspot_x;
	m_mouseHotY = hotspot_y;
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

	uint8_t kc, m, p;
	if (input_get_kb_event(&kc, &m, &p) > 0)
	{
		if ((m & (RT_MODIFIER_SHIFT | RT_MODIFIER_R_SHIFT)) != 0)
			event->kbd.flags |= KBD_SHIFT;
		if ((m & (RT_MODIFIER_CTRL | RT_MODIFIER_R_CTRL)) != 0)
			event->kbd.flags |= KBD_CTRL;
		if ((m & (RT_MODIFIER_ALT | RT_MODIFIER_R_ALT)) != 0)
			event->kbd.flags |= KBD_ALT;

		event->event_code = p ? EVENT_KEYDOWN : EVENT_KEYUP;
		event->kbd.keycode = 0;
		event->kbd.ascii = 0;
		return true;
	}

	int8_t x, y, wheel;
	uint8_t buttons;
	if (input_get_mouse_event(&x, &y, &wheel, &buttons) > 0)
	{
		m_mouseX += x;
		m_mouseX = clamp(m_mouseX, 0, 319);
		m_mouseY += y;
		m_mouseY = clamp(m_mouseY, 0, 199);
		return true;
	}

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
	// printf("play_cdrom\n");
}

void OSystem_RebelV::stop_cdrom()
{
	// printf("stop_cdrom\n");
}

void OSystem_RebelV::update_cdrom()
{
	// printf("update_cdrom\n");
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

void OSystem_RebelV::draw_mouse_cursor()
{
	const byte *src = m_mouseBits;
	if (!src)
		return;

	int x = m_mouseX - m_mouseHotX;
	int y = m_mouseY - m_mouseHotY;
	int w = m_mouseW;
	int h = m_mouseH;

	if (x < 0) {
		w += x;
		src -= x;
		x = 0;
	}
	if (y < 0) {
		h += y;
		src -= y * m_mouseW;
		y = 0;
	}

	if (w > 320 - x)
		w = 320 - x;
	if (h > 200 - y)
		h = 200 - y;

	if (w <= 0 || h <= 0)
		return;

	// // Store the bounding box so that undraw mouse can restore the area the
	// // mouse currently covers to its original content.
	// _mouseOldState.x = x;
	// _mouseOldState.y = y;
	// _mouseOldState.w = w;
	// _mouseOldState.h = h;

	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();

	// byte *bak = _mouseBackup;		// Surface used to backup the area obscured by the mouse

	byte* dst = (byte *)framebuffer + y * 320 + x;
	while (h > 0)
	{
		int width = w;
		while (width > 0)
		{
			// *bak++ = *dst;
			byte color = *src++;
			if (color != 0xFF)
				*dst = color;
			dst++;
			width--;
		}
		src += m_mouseW - w;
		// bak += MAX_MOUSE_W - w;
		dst += 320 - w;
		h--;
	}
}

OSystem *OSystem_RebelV_create()
{ 
	return new OSystem_RebelV();
}
