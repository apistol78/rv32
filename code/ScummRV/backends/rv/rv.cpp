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

int32_t map_key(uint8_t kc, uint8_t ch)
{
	if (kc == RT_KEY_ESCAPE)
		return 27;
	else if (kc == RT_KEY_F5)
		return 319;		// F5
	else if (kc == RT_KEY_SPACE)
		return 32;		
	else
		return ch;
}

OSystem_RebelV* _this = nullptr;

}

OSystem_RebelV::OSystem_RebelV()
{
	uint32_t tid;

	runtime_init();

	_this = this;

	tid = kernel_create_thread([](){
		for (;;) {
			_this->update_timer();
		}
	});
	printf("timer thread %d\n", tid);

	tid = kernel_create_thread([](){
		for (;;) {
			_this->update_sound();
		}
	});
	printf("sound thread %d\n", tid);

	tid = kernel_create_thread([](){
		for (;;) {
			_this->update_frame();
		}
	});	
	printf("update thread %d\n", tid);
}

void OSystem_RebelV::init_size(uint w, uint h)
{
	for (int i = 0; i < 256; ++i)
	{
		const uint8_t r = rand();
		const uint8_t g = rand();
		const uint8_t b = rand();
		m_palette[i] = { r, g, b };
		video_set_palette(
			i,
			(r << 16) | (g << 8) | (b)
		);
	}

	m_overlayCopy = (byte*)malloc(320 * 200);
}

void OSystem_RebelV::set_palette(const byte *colors, uint start, uint num)
{
	for (uint i = 0; i < num; ++i)
	{
		const uint8_t r = colors[0];
		const uint8_t g = colors[1];
		const uint8_t b = colors[2];
		m_palette[start + i] = { r, g, b };
		video_set_palette(
			start + i,
			(r << 16) | (g << 8) | (b)
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
		memcpy(dst, src, w);
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
}

void OSystem_RebelV::set_shake_pos(int shake_pos)
{
	printf("set_shake_pos\n");
}

NewGuiColor OSystem_RebelV::RGBToColor(uint8 r, uint8 g, uint8 b)
{
	uint8_t nearest = 255;
	uint8_t check;
	uint8_t r2, g2, b2;

	uint8_t color = 255;

	for (int32_t i = 0; i < 256; i++)
	{
		r2 = m_palette[i].r;
		g2 = m_palette[i].g;
		b2 = m_palette[i].b;

		check = (abs(r2 - r) + abs(g2 - g) + abs(b2 - b)) / 3;

		if (check == 0)
			return i;
		else if (check < nearest)
		{
			color = i;
			nearest = check;
		}
	}

	return color;
}

void OSystem_RebelV::colorToRGB(NewGuiColor color, uint8 &r, uint8 &g, uint8 &b)
{
	const auto& c = m_palette[color];
	r = c.r;
	g = c.g;
	b = c.b;
}

bool OSystem_RebelV::show_mouse(bool visible)
{
	const bool last = m_mouseVisible;
	m_mouseVisible = visible;
	return last;
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
		event->kbd.flags = 0;
		if ((m & (RT_MODIFIER_SHIFT | RT_MODIFIER_R_SHIFT)) != 0)
			event->kbd.flags |= KBD_SHIFT;
		if ((m & (RT_MODIFIER_CTRL | RT_MODIFIER_R_CTRL)) != 0)
			event->kbd.flags |= KBD_CTRL;
		if ((m & (RT_MODIFIER_ALT | RT_MODIFIER_R_ALT)) != 0)
			event->kbd.flags |= KBD_ALT;

		event->event_code = p ? EVENT_KEYDOWN : EVENT_KEYUP;

		char ch = 0;
		if (input_translate_key(kc, m, &ch))
		{
			event->kbd.keycode = map_key(kc, ch);
			event->kbd.ascii = ch;
		}
		else
		{
			event->kbd.keycode = map_key(kc, kc);
			event->kbd.ascii = event->kbd.keycode;
		}
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
	kernel_cs_t* cs = new kernel_cs_t();
	kernel_cs_init(cs);
	return (OSystem::MutexRef)cs;
}

void OSystem_RebelV::lock_mutex(MutexRef mutex)
{
	kernel_cs_t* cs = (kernel_cs_t*)mutex;
	// kernel_cs_lock(cs);
}

void OSystem_RebelV::unlock_mutex(MutexRef mutex)
{
	kernel_cs_t* cs = (kernel_cs_t*)mutex;
	// kernel_cs_unlock(cs);
}

void OSystem_RebelV::delete_mutex(MutexRef mutex)
{
	kernel_cs_t* cs = (kernel_cs_t*)mutex;
	delete cs;
}

void OSystem_RebelV::show_overlay()
{
	if (m_overlayVisible)
		return;

	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();
	memcpy(m_overlayCopy, framebuffer, 320 * 200);

	m_overlayVisible = true;
}

void OSystem_RebelV::hide_overlay()
{
	if (!m_overlayVisible)
		return;

	clear_overlay();
	m_overlayVisible = false;
}

void OSystem_RebelV::clear_overlay()
{
	if (!m_overlayVisible)
		return;

	uint8_t* framebuffer = (uint8_t*)video_get_secondary_target();
	memcpy(m_overlayCopy, framebuffer, 320 * 200);
}

void OSystem_RebelV::grab_overlay(NewGuiColor *buf, int pitch)
{
	const uint8_t* src = m_overlayCopy;
	for (int32_t i = 0; i < 200; ++i)
	{
		memcpy(buf, src, 320);
		src += 320;
		buf += pitch;
	}
}

void OSystem_RebelV::copy_rect_overlay(const NewGuiColor *buf, int pitch, int x, int y, int w, int h)
{
	if (x < 0)
	{
		w += x;
		buf -= x;
		x = 0;
	}

	if (y < 0)
	{
		h += y;
		buf -= y * pitch;
		y = 0;
	}

	if (w > 320 - x)
		w = 320 - x;

	if (h > 200 - y)
		h = 200 - y;

	if (w <= 0 || h <= 0)
		return;

	uint8_t* dst = m_overlayCopy + y * 320 + x;
	do
	{
		memcpy(dst, buf, w);
		dst += 320;
		buf += pitch;
	}
	while (--h);
}

uint32 OSystem_RebelV::property(int param, Property *value)
{
	switch (param)
	{
	case PROP_SET_WINDOW_CAPTION:
		return 1;
	case PROP_OPEN_CD:
		break;
	case PROP_TOGGLE_ASPECT_RATIO:
		break;
	case PROP_GET_SAMPLE_RATE:
		return 11025;
	case PROP_HAS_SCALER:
		return 0;
	}
	return 0;
}

void OSystem_RebelV::quit()
{
	runtime_cold_restart();
}

void OSystem_RebelV::draw_mouse_cursor()
{
	const byte *src = m_mouseBits;

	int32_t x = m_mouseX - m_mouseHotX;
	int32_t y = m_mouseY - m_mouseHotY;
	int32_t w = m_mouseW;
	int32_t h = m_mouseH;

	if (x < 0)
	{
		w += x;
		src -= x;
		x = 0;
	}
	if (y < 0)
	{
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

	byte* dst = (uint8_t*)video_get_primary_target();
	dst += y * 320 + x;

	while (h > 0)
	{
		int width = w;
		while (width > 0)
		{
			byte color = *src++;
			if (color != 0xFF)
				*dst = color;
			dst++;
			width--;
		}
		src += m_mouseW - w;
		dst += 320 - w;
		h--;
	}
}

void OSystem_RebelV::update_sound()
{
	const int32_t chunkSize = 512;
	static int16_t buf[chunkSize * 2];
	if (m_soundProc)
	{
		for (int32_t i = 0; i < 16; ++i)
		{
			const int32_t queued = audio_get_queued();
			if (queued <= 0)
				printf("WARN: audio underrun\n");

			const int32_t free = 4096 - queued;
			if (free < chunkSize)
				break;

			m_soundProc(m_soundParam, (byte*)buf, sizeof(buf));
			audio_play_stereo(buf, chunkSize * 2);
		}
	}
	kernel_sleep(10);
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
	kernel_sleep(30);
}

void OSystem_RebelV::update_frame()
{
	uint8_t buttons;

	runtime_update();
	input_get_mouse_state(&m_mouseX, &m_mouseY, &buttons);

	if (m_overlayVisible)
		video_blit(1, m_overlayCopy);
	else
	 	video_swap(2);

	// Mouse is drawn directly onto primary target, thus
	// no need to preserve background etc.
	if (m_mouseVisible)
		draw_mouse_cursor();
}

OSystem *OSystem_RebelV_create()
{ 
	return new OSystem_RebelV();
}
