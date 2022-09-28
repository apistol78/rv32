#include "Runtime/Input.h"
#include "Runtime/Kernel.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/UART.h"

typedef struct
{
	uint8_t state;
	uint8_t keycode;
	uint8_t modifier;
}
key_event_t;

typedef struct
{
	int8_t x;
	int8_t y;
	int8_t wheel;
	uint8_t buttons;
}
mouse_event_t;

// keycode : normal, shift, alt, shift + alt
const char keyboard_se[][5] =
{
	{ RT_KEY_A, 'a', 'A', 0, 0 },
	{ RT_KEY_B, 'b', 'B', 0, 0 },
	{ RT_KEY_C, 'c', 'C', 0, 0 },
	{ RT_KEY_D, 'd', 'D', 0, 0 },
	{ RT_KEY_E, 'e', 'E', 0, 0 },
	{ RT_KEY_F, 'f', 'F', 0, 0 },
	{ RT_KEY_G, 'g', 'G', 0, 0 },
	{ RT_KEY_H, 'h', 'H', 0, 0 },
	{ RT_KEY_I, 'i', 'I', 0, 0 },
	{ RT_KEY_J, 'j', 'J', 0, 0 },
	{ RT_KEY_K, 'k', 'K', 0, 0 },
	{ RT_KEY_L, 'l', 'L', 0, 0 },
	{ RT_KEY_M, 'm', 'M', 0, 0 },
	{ RT_KEY_N, 'n', 'N', 0, 0 },
	{ RT_KEY_O, 'o', 'O', 0, 0 },
	{ RT_KEY_O, 'o', 'O', 0, 0 },
	{ RT_KEY_P, 'p', 'P', 0, 0 },
	{ RT_KEY_Q, 'q', 'Q', 0, 0 },
	{ RT_KEY_R, 'r', 'R', 0, 0 },
	{ RT_KEY_S, 's', 'S', 0, 0 },
	{ RT_KEY_T, 't', 'T', 0, 0 },
	{ RT_KEY_U, 'u', 'U', 0, 0 },
	{ RT_KEY_V, 'v', 'V', 0, 0 },
	{ RT_KEY_W, 'w', 'W', 0, 0 },
	{ RT_KEY_X, 'x', 'X', 0, 0 },
	{ RT_KEY_Y, 'y', 'Y', 0, 0 },
	{ RT_KEY_Z, 'z', 'Z', 0, 0 },

	{ RT_KEY_SPACE,		' ',	' ',	0, 0 },
	{ RT_KEY_RETURN,	'\n',	'\n',	0, 0 },
	{ RT_KEY_ENTER,		'\n',	'\n',	0, 0 },
	{ RT_KEY_BACKSPACE,	'\b',	'\b',	0, 0 },

	{ RT_KEY_1, '1', '!',   0, 0 },
	{ RT_KEY_2, '2', '"', '@', 0 },
	{ RT_KEY_3, '3', '#',   0, 0 },
	{ RT_KEY_4, '4',   0, '$', 0 },
	{ RT_KEY_5, '5', '%',   0, 0 },
	{ RT_KEY_6, '6', '&',   0, 0 },
	{ RT_KEY_7, '7', '/', '{', 0 },
	{ RT_KEY_8, '8', '(', '[', 0 },
	{ RT_KEY_9, '9', ')', ']', 0 },
	{ RT_KEY_0, '0', '=', '}', 0 },

	{ RT_KEY_DOT,   '.', ':', 0, 0 },
	{ RT_KEY_COMMA, ',', ';', 0, 0 },

	{ 0, 0, 0, 0, 0 }
};

static kernel_cs_t s_lock;

static int32_t s_keystate[256];

static key_event_t s_key_events[16];
static int32_t s_key_events_in;
static int32_t s_key_events_out;

static int32_t s_mouse_x;
static int32_t s_mouse_y;
static uint8_t s_mouse_buttons;

static mouse_event_t s_mouse_events[16];
static int32_t s_mouse_events_in;
static int32_t s_mouse_events_out;

static int32_t clamp(int32_t v, int32_t mn, int32_t mx)
{
	return
		v < mn ? mn :
		v > mx ? mx :
		v;
}

void input_init()
{
	kernel_cs_init(&s_lock);

	for (int32_t i = 0; i < 256; ++i)
		s_keystate[i] = 0;

	s_key_events_in = 0;
	s_key_events_out = 0;

	s_mouse_x = 0;
	s_mouse_y = 0;
	s_mouse_buttons = 0;

	s_mouse_events_in = 0;
	s_mouse_events_out = 0;

	// Clear anything queued in uart.
	while (!uart_rx_empty(1))
		uart_rx_u8(1);
}

void input_update()
{
	uint8_t reset = 0;

	while (!uart_rx_empty(1))
	{
		// Receive and decode packet.
		const uint8_t id = uart_rx_u8(1);
		switch (id)
		{
		case 'K':
		case 'k':
			{
				const uint8_t keycode = uart_rx_u8(1);
				const uint8_t modifier = uart_rx_u8(1);

				s_keystate[keycode] = ((id == 'K') ? 1 : 0);

				// Enqueue key event.
				key_event_t* e = &s_key_events[s_key_events_in];
				e->state = id;
				e->keycode = keycode;
				e->modifier = modifier;

				kernel_cs_lock(&s_lock);
				s_key_events_in = (s_key_events_in + 1) & 15;
				if (s_key_events_in == s_key_events_out)
					s_key_events_out = (s_key_events_out + 1) & 15;
				kernel_cs_unlock(&s_lock);

				// Check for reset (CTRL+ALT+DEL) combination.
				if (keycode == RT_KEY_DELETE && (modifier & (RT_MODIFIER_CTRL | RT_MODIFIER_ALT)) == (RT_MODIFIER_CTRL | RT_MODIFIER_ALT))
					reset = 1;
			}
			break;

		case 'M':
			{
				const int8_t x = (int8_t)uart_rx_u8(1);
				const int8_t y = (int8_t)uart_rx_u8(1);
				const int8_t wheel = (int8_t)uart_rx_u8(1);
				const uint8_t buttons = uart_rx_u8(1);

				s_mouse_x = clamp(s_mouse_x + x, 0, 319);
				s_mouse_y = clamp(s_mouse_y + y, 0, 199);
				s_mouse_buttons = buttons;

				// Enqueue mouse event.
				mouse_event_t* e = &s_mouse_events[s_mouse_events_in];
				e->x = x;
				e->y = y;
				e->wheel = wheel;
				e->buttons = buttons;

				kernel_cs_lock(&s_lock);
				s_mouse_events_in = (s_mouse_events_in + 1) & 15;
				if (s_mouse_events_in == s_mouse_events_out)
					s_mouse_events_out = (s_mouse_events_out + 1) & 15;
				kernel_cs_unlock(&s_lock);
			}
			break;

		case 'E':
			break;
		}
	}

	if (reset)
		runtime_cold_restart();
}

int32_t input_get_kb_state(uint8_t keycode)
{
	return s_keystate[keycode];
}

int32_t input_get_kb_event(uint8_t* keycode, uint8_t* modifier, uint8_t* pressed)
{
	if (s_key_events_in == s_key_events_out)
		return 0;

	// Read event from queue.
	kernel_cs_lock(&s_lock);
	
	const key_event_t* e = &s_key_events[s_key_events_out];
	*keycode = e->keycode;
	*modifier = e->modifier;
	*pressed = (e->state == 'K') ? 1 : 0;
	s_key_events_out = (s_key_events_out + 1) & 15;

	kernel_cs_unlock(&s_lock);
	return 1;
}

int32_t input_translate_key(uint8_t keycode, uint8_t modifier, char* ch)
{
	int32_t idx = 0;
	if (modifier & (RT_MODIFIER_SHIFT | RT_MODIFIER_R_SHIFT)) {
		idx++;
	}
	if (modifier & (RT_MODIFIER_ALT | RT_MODIFIER_R_ALT)) {
		idx++;
	}
	for (int32_t i = 0; keyboard_se[i][0] != 0; ++i) {
		if (keyboard_se[i][0] == keycode) {
			*ch = keyboard_se[i][idx + 1];
			return 1;
		}
	}
	return 0;	
}

int32_t input_get_mouse_state(int32_t* x, int32_t* y, uint8_t* buttons)
{
	*x = s_mouse_x;
	*y = s_mouse_y;
	*buttons = s_mouse_buttons;
	return 1;
}

int32_t input_get_mouse_event(int8_t* x, int8_t* y, int8_t* wheel, uint8_t* buttons)
{
	if (s_mouse_events_in == s_mouse_events_out)
		return 0;

	// Read event from queue.
	kernel_cs_lock(&s_lock);

	const mouse_event_t* e = &s_mouse_events[s_mouse_events_out];
	*x = e->x;
	*y = e->y;
	*wheel = e->wheel;
	*buttons = e->buttons;
	s_mouse_events_out = (s_mouse_events_out + 1) & 15;
	
	kernel_cs_unlock(&s_lock);
	return 1;
}
