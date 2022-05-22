#include <stdio.h>
#include "Runtime/Input.h"
#include "Runtime/HAL/UART.h"

typedef struct
{
	uint8_t state;
	uint8_t keycode;
}
key_event_t;

static int32_t s_keystate[256];

static key_event_t s_keyevents[16];
static int32_t s_keyevents_in;
static int32_t s_keyevents_out;

void input_init()
{
	for (int i = 0; i < 256; ++i)
		s_keystate[i] = 0;

	s_keyevents_in = 0;
	s_keyevents_out = 0;
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
				s_keyevents[s_keyevents_in].state = id;
				s_keyevents[s_keyevents_in].keycode = keycode;

				s_keyevents_in = (s_keyevents_in + 1) & 15;
				if (s_keyevents_in == s_keyevents_out)
					s_keyevents_out = (s_keyevents_out + 1) & 15;
			}
			break;

		case 'M':
			{
				const int8_t x = (int8_t)uart_rx_u8(1);
				const int8_t y = (int8_t)uart_rx_u8(1);
				const int8_t wheel = (int8_t)uart_rx_u8(1);
				const uint8_t buttons = uart_rx_u8(1);
				//printf("Mouse %d * %d, wheel = %d, buttons = %d\n", x, y, wheel, buttons);
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
	if (s_keyevents_in == s_keyevents_out)
		return 0;

	// Read event from queue.
	*keycode = s_keyevents[s_keyevents_out].keycode;
	*pressed = (s_keyevents[s_keyevents_out].state == 'K') ? 1 : 0;

	s_keyevents_out = (s_keyevents_out + 1) & 15;
	return 1;
}
