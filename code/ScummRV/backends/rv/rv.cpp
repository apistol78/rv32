#include <stdio.h>
#include <stdlib.h>
#include "Runtime/Input.h"
#include "Runtime/Kernel.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/Audio.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/Video.h"

#include "rv.h"

namespace
{

const int MAX_MOUSE_W = 80;
const int MAX_MOUSE_H = 80;
const int MAX_SCALING = 3;

int32_t clamp(int32_t v, int32_t mn, int32_t mx)
{
	return
		v < mn ? mn :
		v > mx ? mx :
		v;
}

int32_t map_key(uint8_t kc)
{
	if (kc >= RT_KEY_F1 && kc <= RT_KEY_F12) {
		return kc - RT_KEY_F1 + 315;
	// } else if (key >= SDLK_KP0 && key <= SDLK_KP9) {
	// 	return key - SDLK_KP0 + '0';
	// } else if (key >= SDLK_UP && key <= SDLK_PAGEDOWN) {
	// 	return key;
	// } else if (unicode) {
	// 	return unicode;
	// } else if (key >= 'a' && key <= 'z' && mod & KMOD_SHIFT) {
	// 	return key & ~0x20;
	// } else if (key >= SDLK_NUMLOCK && key <= SDLK_EURO) {
	// 	return 0;
	}
	return 0;
}

OSystem_RebelV* _this = nullptr;

// void updater_thread()
// {
// 	for (;;)
// 	{
// 		_this->update_timer();
// 		_this->update_sound();
// 		_this->update_frame();
// 		kernel_sleep(4);
// 	}
// }

}

OSystem_RebelV::OSystem_RebelV()
{
	runtime_init();
	kernel_init();

	_this = this;
	//kernel_create_thread(updater_thread);

	kernel_create_thread([](){
		for (;;) {
			_this->update_timer();
			kernel_sleep(10);
		}
	});
	kernel_create_thread([](){
		for (;;) {
			_this->update_sound();
			kernel_sleep(10);
		}
	});
	kernel_create_thread([](){
		for (;;) {
			_this->update_frame();
			kernel_sleep(20);
		}
	});	
}

void OSystem_RebelV::init_size(uint w, uint h)
{
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

	m_mouseBackup = (byte*)malloc(MAX_MOUSE_W * MAX_MOUSE_H * MAX_SCALING * 2);
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

	// undraw_mouse_cursor();

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
	// runtime_update();
	// draw_mouse_cursor();
	// video_swap();
}

void OSystem_RebelV::set_shake_pos(int shake_pos)
{
	printf("set_shake_pos\n");
}

bool OSystem_RebelV::show_mouse(bool visible)
{
	const bool last = m_mouseVisible;

	if ((m_mouseVisible = visible) == true)
		draw_mouse_cursor();
	else
		undraw_mouse_cursor();

	// video_swap();
	return last;
}

void OSystem_RebelV::warp_mouse(int x, int y)
{
	m_mouseX = x;
	m_mouseY = y;

	// if (m_mouseVisible)
	// {
	// 	draw_mouse_cursor();
	// 	// video_swap();
	// }
}

void OSystem_RebelV::set_mouse_cursor(const byte *buf, uint w, uint h, int hotspot_x, int hotspot_y)
{
	m_mouseBits = buf;
	m_mouseW = w;
	m_mouseH = h;
	m_mouseHotX = hotspot_x;
	m_mouseHotY = hotspot_y;

	// undraw_mouse_cursor();

	// if (m_mouseVisible)
	// {
	// 	draw_mouse_cursor();
	// 	// video_swap();
	// }
}

uint32 OSystem_RebelV::get_msecs()
{
	return timer_get_ms();
}

void OSystem_RebelV::delay_msecs(uint msecs)
{
	kernel_sleep(msecs);
}

void OSystem_RebelV::set_timer(TimerProc callback, int timer)
{
	m_timerInterval = timer;
	m_timerExpire = timer_get_ms() + timer;
	m_timerCallback = callback;
}

bool OSystem_RebelV::poll_event(Event *event)
{
	// Get keyboard events.
	uint8_t kc, m, p;
	if (input_get_kb_event(&kc, &m, &p) > 0)
	{
		char ch = 0;
		input_translate_key(kc, m, &ch);

		if ((m & (RT_MODIFIER_SHIFT | RT_MODIFIER_R_SHIFT)) != 0)
			event->kbd.flags |= KBD_SHIFT;
		if ((m & (RT_MODIFIER_CTRL | RT_MODIFIER_R_CTRL)) != 0)
			event->kbd.flags |= KBD_CTRL;
		if ((m & (RT_MODIFIER_ALT | RT_MODIFIER_R_ALT)) != 0)
			event->kbd.flags |= KBD_ALT;

		event->event_code = p ? EVENT_KEYDOWN : EVENT_KEYUP;
		event->kbd.keycode = map_key(kc);
		event->kbd.ascii = ch;
		return true;
	}

	// Get mouse motion events.
	int8_t x, y, wheel;
	uint8_t buttons;
	if (input_get_mouse_event(&x, &y, &wheel, &buttons) > 0)
	{
		if (x != 0 || y != 0)
		{
			int32_t mx, my;
			input_get_mouse_state(&mx, &my, &buttons);			

			event->event_code = EVENT_MOUSEMOVE;
			event->mouse.x = mx;
			event->mouse.y = my;
			return true;
		}
	}

	// Get mouse button events.
	int32_t mx, my;
	input_get_mouse_state(&mx, &my, &buttons);
	if (buttons != m_lastButtons)
	{
		if (buttons & 1)
			event->event_code = EVENT_LBUTTONDOWN;
		else if (m_lastButtons & 1)
			event->event_code = EVENT_LBUTTONUP;
		else if (buttons & 2)
			event->event_code = EVENT_RBUTTONDOWN;
		else if (m_lastButtons & 2)
			event->event_code = EVENT_RBUTTONUP;

		event->mouse.x = m_mouseX;
		event->mouse.y = m_mouseY;

		m_lastButtons = buttons;
		return true;
	}

	return false;
}

bool OSystem_RebelV::set_sound_proc(SoundProc proc, void *param, SoundFormat format)
{
	m_soundParam = param;
	m_soundProc = proc;
	return true;
}

void OSystem_RebelV::clear_sound_proc()
{
	m_soundProc = nullptr;
	m_soundParam = nullptr;
}

bool OSystem_RebelV::poll_cdrom()
{
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
		return 11025; // 22050; //44100; //SAMPLES_PER_SEC;
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
	if (m_mouseDrawn)
		return;

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

	// Store the bounding box so that undraw mouse can restore the area the
	// mouse currently covers to its original content.
	m_mouseBackupX = x;
	m_mouseBackupY = y;
	m_mouseBackupW = w;
	m_mouseBackupH = h;

	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();

	byte* bak = m_mouseBackup;
	byte* dst = (byte *)framebuffer + y * 320 + x;
	while (h > 0)
	{
		int width = w;
		while (width > 0)
		{
			*bak++ = *dst;
			byte color = *src++;
			if (color != 0xFF)
				*dst = color;
			dst++;
			width--;
		}
		src += m_mouseW - w;
		bak += MAX_MOUSE_W - w;
		dst += 320 - w;
		h--;
	}

	m_mouseDrawn = true;
}

void OSystem_RebelV::undraw_mouse_cursor()
{
	if (!m_mouseDrawn)
		return;

	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();

	byte *dst, *bak = m_mouseBackup;

	// No need to do clipping here, since draw_mouse() did that already
	dst = (byte *)framebuffer + m_mouseBackupY * 320 + m_mouseBackupX;
	for (int y = 0; y < m_mouseBackupH; ++y, bak += MAX_MOUSE_W, dst += 320) {
		for (int x = 0; x < m_mouseBackupW; ++x) {
			dst[x] = bak[x];
		}
	}

	//add_dirty_rect(m_mouseBackupX, m_mouseBackupY, m_mouseBackupW, m_mouseBackupH);
	m_mouseDrawn = false;
}

void OSystem_RebelV::update_sound()
{
	const int32_t chunkSize = 256;
	static int16_t buf[chunkSize * 2];
	if (m_soundProc)
	{
		int32_t qn = 0;
		for (int32_t i = 0; i < 4; ++i)
		{
			int32_t free = 1024 - audio_get_queued();
			if (free < chunkSize)
				break;

			m_soundProc(m_soundParam, (byte*)buf, sizeof(buf));
			audio_play_stereo(buf, chunkSize * 2);

			qn += chunkSize;
		}
	}
}

void OSystem_RebelV::update_timer()
{
	const uint32_t ms = timer_get_ms();
	if (ms >= m_timerExpire || ms < m_lastTime)
	{
		if (m_timerCallback)
			m_timerCallback(m_timerInterval);
		m_timerExpire = ms + m_timerInterval;
	}
	m_lastTime = ms;
}

void OSystem_RebelV::update_frame()
{
	uint8_t buttons;

	runtime_update();
	input_get_mouse_state(&m_mouseX, &m_mouseY, &buttons);

	undraw_mouse_cursor();
	if (m_mouseVisible)
		draw_mouse_cursor();

	video_swap();
}

OSystem *OSystem_RebelV_create()
{ 
	return new OSystem_RebelV();
}