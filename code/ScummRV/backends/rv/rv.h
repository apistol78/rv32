#pragma once

#include <common/stdafx.h>
#include <common/system.h>

class OSystem_RebelV : public OSystem {
public:
	OSystem_RebelV();
	
	virtual void init_size(uint w, uint h) override final;

	virtual int16 get_height() override final { return 320; }
	
	virtual int16 get_width() override final { return 200; }

	virtual void set_palette(const byte *colors, uint start, uint num) override final;

	virtual void copy_rect(const byte *buf, int pitch, int x, int y, int w, int h) override final;

	virtual void move_screen(int dx, int dy, int height) override final;

	virtual void update_screen() override final;

	virtual void set_shake_pos(int shake_pos) override final;

	virtual bool show_mouse(bool visible) override final;

	virtual void warp_mouse(int x, int y) override final;
	
	virtual void set_mouse_cursor(const byte *buf, uint w, uint h, int hotspot_x, int hotspot_y) override final;

	virtual uint32 get_msecs() override final;
	
	virtual void delay_msecs(uint msecs) override final;

	virtual void set_timer(TimerProc callback, int timer) override final;

	virtual bool poll_event(Event *event) override final;

	virtual bool set_sound_proc(SoundProc proc, void *param, SoundFormat format) override final;

	virtual void clear_sound_proc() override final;

	virtual bool poll_cdrom() override final;

	virtual void play_cdrom(int track, int num_loops, int start_frame, int duration) override final;

	virtual void stop_cdrom() override final;

	virtual void update_cdrom() override final;

	virtual MutexRef create_mutex() override final;

	virtual void lock_mutex(MutexRef mutex) override final;

	virtual void unlock_mutex(MutexRef mutex) override final;

	virtual void delete_mutex(MutexRef mutex) override final;

	virtual void show_overlay() override final;

	virtual void hide_overlay() override final;

	virtual void clear_overlay() override final;

	virtual void grab_overlay(NewGuiColor *buf, int pitch) override final;

	virtual void copy_rect_overlay(const NewGuiColor *buf, int pitch, int x, int y, int w, int h) override final;

	virtual uint32 property(int param, Property *value) override final;

	virtual void quit() override final;

// private:
	uint32_t m_lastTime = 0;

	TimerProc m_timerCallback = nullptr;
	int m_timerInterval = 0;
	int m_timerExpire = 0;

	SoundProc m_soundProc = nullptr;
	void* m_soundParam = nullptr;

	const byte* m_mouseBits = nullptr;
	int m_mouseW = 0;
	int m_mouseH = 0;
	int m_mouseHotX = 0;
	int m_mouseHotY = 0;
	int m_mouseX = 160;
	int m_mouseY = 100;
	bool m_mouseVisible = false;
	byte* m_mouseBackup = nullptr;
	int m_mouseBackupX = 0;
	int m_mouseBackupY = 0;
	int m_mouseBackupW = 0;
	int m_mouseBackupH = 0;
	bool m_mouseDrawn = false;

	uint8_t m_lastButtons = 0;

	void draw_mouse_cursor();

	void undraw_mouse_cursor();

	void update_timer();

	void update_sound();

	void update_frame();	
};
