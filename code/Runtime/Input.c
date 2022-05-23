#include <stdio.h>
#include "Runtime/Input.h"
#include "Runtime/HAL/UART.h"

typedef struct
{
	uint8_t state;
	uint8_t keycode;
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

void input_init()
{
	for (int i = 0; i < 256; ++i)
		s_keystate[i] = 0;

	s_key_events_in = 0;
	s_key_events_out = 0;

	s_mouse_x = 0;
	s_mouse_y = 0;
	s_mouse_buttons = 0;

	s_mouse_events_in = 0;
	s_mouse_events_out = 0;
}

void input_update()
{
	while (!uart_rx_empty(1))
	{
		// Receive and decode packet.
		uint8_t id = uart_rx_u8(1);
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

				s_key_events_in = (s_key_events_in + 1) & 15;
				if (s_key_events_in == s_key_events_out)
					s_key_events_out = (s_key_events_out + 1) & 15;
			}
			break;

		case 'M':
			{
				const int8_t x = (int8_t)uart_rx_u8(1);
				const int8_t y = (int8_t)uart_rx_u8(1);
				const int8_t wheel = (int8_t)uart_rx_u8(1);
				const uint8_t buttons = uart_rx_u8(1);

				s_mouse_x += x;
				s_mouse_y += y;
				s_mouse_buttons = buttons;
				
				// Enqueue mouse event.
				mouse_event_t* e = &s_mouse_events[s_mouse_events_in];
				e->x = x;
				e->y = y;
				e->wheel = wheel;
				e->buttons = buttons;

				s_mouse_events_in = (s_mouse_events_in + 1) & 15;
				if (s_mouse_events_in == s_mouse_events_out)
					s_mouse_events_out = (s_mouse_events_out + 1) & 15;
			}
			break;
		}

		// Wait until end of packet.
		while (uart_rx_u8(1) != 'E');
	}
}

int32_t input_get_kb_state(uint8_t keycode)
{
	return s_keystate[keycode];
}

int32_t input_get_kb_event(uint8_t* keycode, uint8_t* pressed)
{
	if (s_key_events_in == s_key_events_out)
		return 0;

	// Read event from queue.
	*keycode = s_key_events[s_key_events_out].keycode;
	*pressed = (s_key_events[s_key_events_out].state == 'K') ? 1 : 0;

	s_key_events_out = (s_key_events_out + 1) & 15;
	return 1;
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
	const mouse_event_t* e = &s_mouse_events[s_mouse_events_out];
	*x = e->x;
	*y = e->y;
	*wheel = e->wheel;
	*buttons = e->buttons;

	s_mouse_events_out = (s_mouse_events_out + 1) & 15;
	return 1;
}
