
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef void (*call_fn_t)();

uint8_t uart_rx_u8()
{
	volatile uint8_t* uart = (volatile uint8_t*)0x50000010;
	return *uart;
}

uint32_t uart_rx_u32()
{
	volatile uint8_t* uart = (volatile uint8_t*)0x50000010;
	uint8_t tmp[4];
	tmp[0] = uart_rx_u8();
	tmp[1] = uart_rx_u8();
	tmp[2] = uart_rx_u8();
	tmp[3] = uart_rx_u8();
	return *(uint32_t*)tmp;
}

void uart_tx_u8(uint8_t data)
{
	volatile uint8_t* uart = (volatile uint8_t*)0x50000010;
	*uart = data;
}

void main()
{
	uint8_t cmd;
	uint32_t addr;
	uint8_t data;

	for (;;)
	{
		cmd = uart_rx_u8();

		// poke
		if (cmd == 0x01)
		{
			addr = uart_rx_u32();
			data = uart_rx_u8();
			*(uint8_t*)addr = data;
		}

		// jump to
		if (cmd == 0x02)
		{
			addr = uart_rx_u32();
			((call_fn_t)addr)();
		}
	}
}