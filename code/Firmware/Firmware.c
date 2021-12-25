
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

void fatal_error(uint8_t error)
{
	volatile uint32_t* leds = (volatile uint32_t*)0x50000000;
	for (;;)
	{
		*leds = 0x80 | error;
		for (uint32_t i = 0; i < 1000000; ++i)
			__asm__ volatile ("nop");
		*leds = 0x00 | error;
		for (uint32_t i = 0; i < 1000000; ++i)
			__asm__ volatile ("nop");
	}
//	__asm__ volatile ("ebreak");
}

void main()
{
	volatile uint32_t* leds = (volatile uint32_t*)0x50000000;
	*leds = 0x00000000;

	// verify sram
	{
		volatile uint32_t* sram = (volatile uint32_t*)0x10000000;
		
		for (uint32_t i = 0; i < 256; ++i)
			sram[i] = 0x1122ab00 + i;

		for (uint32_t i = 0; i < 256; ++i)
		{
			if (sram[i] != 0x1122ab00 + i)
				fatal_error(1);
		}
	}

	for (;;)
	{
		uint8_t cmd = uart_rx_u8();
		*leds = (uint32_t)cmd;

		// poke
		if (cmd == 0x01)
		{
			uint32_t addr = uart_rx_u32();
			uint8_t nb = uart_rx_u8();
			uint8_t cs = 0;

			if (nb == 0 || nb > 16)
			{
				uart_tx_u8(0x81);	// Invalid data.
				continue;
			}

			// Add address to checksum.
			const uint8_t* p = (const uint8_t*)&addr;
			cs ^= p[0];
			cs ^= p[1];
			cs ^= p[2];
			cs ^= p[3];

			// Receive 
			uint8_t r[16];
			for (uint8_t i = 0; i < nb; ++i)
			{
				uint8_t d = uart_rx_u8();
				r[i] = d;
				cs ^= d;
			}

			if (cs == uart_rx_u8())
			{
				for (uint8_t i = 0; i < nb; ++i)
					*(uint8_t*)addr++ = r[i];
				uart_tx_u8(0x80);	// Ok
			}
			else
				uart_tx_u8(0x82);	// Invalid checksum.
		}

		// jump to
		else if (cmd == 0x02)
		{
			uint32_t addr = uart_rx_u32();
			uint8_t cs = 0;

			// Add address to checksum.
			const uint8_t* p = (const uint8_t*)&addr;
			cs ^= p[0];
			cs ^= p[1];
			cs ^= p[2];
			cs ^= p[3];

			if (cs == uart_rx_u8())
			{
				uart_tx_u8(0x80);	// Ok
				((call_fn_t)addr)();
			}
			else
				uart_tx_u8(0x82);	// Invalid checksum.
		}
	}
}
