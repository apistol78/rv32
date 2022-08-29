#include "common/stdafx.h"
#include "common/scummsys.h"
#include "common/system.h"
#include "common/engine.h"
#include "common/util.h"
#include "common/dirty.h"
#include "common/gui/options.h"
#include "backends/intern.h"
#include "portdefs.h"

class OSystem_RebelV : public OSystem {
public:
	virtual bool init() override final;

	virtual void init_size(uint w, uint h) override final;

	virtual int16 get_height() override final { return SCREEN_HEIGHT; }
	
	virtual int16 get_width() override final { return SCREEN_WIDTH; }

	virtual void set_palette(const byte *colors, uint start, uint num) override final;

	virtual void copy_rect(const byte *buf, int pitch, int x, int y, int w, int h) override final;

	virtual void copy_screen(const byte *buf, int y, int h, const uint32* mask, bool doublebuffer) override final;

	virtual void move_screen(int dx, int dy, int height) override final;

	virtual void update_screen() override final;

	virtual void set_shake_pos(int shake_pos) override final;

	virtual NewGuiColor RGBToColor(uint8 r, uint8 g, uint8 b) override final;

	virtual bool show_mouse(bool visible) override final;

	virtual void warp_mouse(int x, int y) override final;
	
	virtual void set_mouse_cursor(uint32 id, const byte *buf, const byte* mask, uint w, uint h, int hotspot_x, int hotspot_y) override final;

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

	virtual void grab_overlay(byte *buf, int pitch) override final;

	virtual uint32 property(int param, Property *value) override final;

	virtual void quit() override final;
};
