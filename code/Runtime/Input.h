#pragma once

#include "Runtime/HAL/Common.h"

EXTERN_C void input_init();

EXTERN_C void input_update();

EXTERN_C int32_t input_get_kb_state(uint8_t keycode);

EXTERN_C int32_t input_get_kb_event(uint8_t* keycode, uint8_t* pressed);

// Keycodes
#define RT_KEY_A	        0x04
#define RT_KEY_B	        0x05
#define RT_KEY_C	        0x06
#define RT_KEY_D	        0x07
#define RT_KEY_E	        0x08
#define RT_KEY_F	        0x09
#define RT_KEY_G	        0x0a
#define RT_KEY_H	        0x0b
#define RT_KEY_I	        0x0c
#define RT_KEY_J	        0x0d
#define RT_KEY_K	        0x0e
#define RT_KEY_L	        0x0f
#define RT_KEY_M	        0x10
#define RT_KEY_N	        0x11
#define RT_KEY_O	        0x12
#define RT_KEY_P	        0x13
#define RT_KEY_Q	        0x14
#define RT_KEY_R	        0x15
#define RT_KEY_S	        0x16
#define RT_KEY_T	        0x17
#define RT_KEY_U	        0x18
#define RT_KEY_V	        0x19
#define RT_KEY_W	        0x1a
#define RT_KEY_X	        0x1b
#define RT_KEY_Y	        0x1c
#define RT_KEY_Z	        0x1d
#define RT_KEY_SPACE	    0x2c
#define RT_KEY_RETURN	    0x28
#define RT_KEY_ENTER	    0x58
#define RT_KEY_BACKSPACE	0x2a
#define RT_KEY_1	        0x1e
#define RT_KEY_2	        0x1f
#define RT_KEY_3	        0x20
#define RT_KEY_4	        0x21
#define RT_KEY_5	        0x22
#define RT_KEY_6	        0x23
#define RT_KEY_7	        0x24
#define RT_KEY_8	        0x25
#define RT_KEY_9	        0x26
#define RT_KEY_0	        0x27
#define RT_KEY_ESCAPE	    0x29
#define RT_KEY_TAB	        0x2b
#define RT_KEY_TILDE	    0x35
#define RT_KEY_F1	        0x3a
#define RT_KEY_F2	        0x3b
#define RT_KEY_F3	        0x3c
#define RT_KEY_F4	        0x3d
#define RT_KEY_F5	        0x3e
#define RT_KEY_F6	        0x3f
#define RT_KEY_F7	        0x40
#define RT_KEY_F8	        0x41
#define RT_KEY_F9	        0x42
#define RT_KEY_F10	        0x43
#define RT_KEY_F11	        0x44
#define RT_KEY_F12	        0x45
#define RT_KEY_DELETE	    0x4c
#define RT_KEY_INSERT	    0x49
#define RT_KEY_END	        0x4d
#define RT_KEY_HOME	        0x4a
#define RT_KEY_PGUP	        0x4b
#define RT_KEY_PGDOWN	    0x4e
#define RT_KEY_PRTSCR	    0x46
#define RT_KEY_SCRLK	    0x47
#define RT_KEY_PAUSE	    0x48
#define RT_KEY_LEFT		    0x50
#define RT_KEY_RIGHT	    0x4f
#define RT_KEY_UP		    0x52
#define RT_KEY_DOWN		    0x51
#define RT_KEY_COMMA	    0x36
#define RT_KEY_DOT		    0x37
#define RT_KEY_MINUS	    0x38
#define RT_KEY_PLUS		    0x2d
#define RT_KEY_DASH		    0x2e
