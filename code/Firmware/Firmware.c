
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef void (*call_fn_t)();

uint8_t uart_rx_u8()
{
	volatile uint32_t* uart = (volatile uint32_t*)0x50000010;
	return (uint8_t)*uart;
}

uint32_t uart_rx_u32()
{
	uint8_t tmp[4];
	tmp[0] = uart_rx_u8();
	tmp[1] = uart_rx_u8();
	tmp[2] = uart_rx_u8();
	tmp[3] = uart_rx_u8();
	return *(uint32_t*)tmp;
}

void uart_tx_u8(uint8_t data)
{
	volatile uint32_t* uart = (volatile uint32_t*)0x50000010;
	*uart = (uint32_t)data;
}

void fatal_error()
{
	volatile uint32_t* leds = (volatile uint32_t*)0x50000000;
	*leds = 0xffffffff;
	__asm__ volatile ("ebreak");
	for (;;);
}

void main()
{
	volatile uint32_t* leds = (volatile uint32_t*)0x50000000;
	uint8_t cmd;
	uint32_t addr;
	uint8_t data;

	// verify sram
	{
		volatile uint32_t* sram = (volatile uint32_t*)0x10000000;
		
		for (uint32_t i = 0; i < 16; ++i)
			sram[i] = 0x1122ab00 + i;

		for (uint32_t i = 0; i < 16; ++i)
		{
			*leds = (i + 1);
			if (sram[i] != 0x1122ab00 + i)
				fatal_error();
		}
	}

	*leds = 0x00000000;

	for (;;)
	{
		cmd = uart_rx_u8();

		*leds = (uint32_t)cmd;

		// poke
		if (cmd == 0x01)
		{
			addr = uart_rx_u32();
			data = uart_rx_u8();
			if (addr >= 0x10000000 && addr <=  0x10001000)
			{
				*(uint8_t*)addr = data;
				uart_tx_u8(data);
			}
			else
				fatal_error();
		}

		// peek
		else if (cmd == 0x02)
		{
			addr = uart_rx_u32();
			uart_tx_u8(*(uint8_t*)addr);
		}

		// jump to
		else if (cmd == 0x03)
		{
			addr = uart_rx_u32();
			if (addr == 0x10000000)
			{
				uart_tx_u8(0x80);
				((call_fn_t)addr)();
			}
			else
				fatal_error();
		}

/*
		// unknown command
		else
			fatal_error();
*/
	}
}