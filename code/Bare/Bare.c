
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

void main()
{
	volatile uint32_t* leds = (volatile uint32_t*)0x50000000;
	uint8_t cnt = 1;
	int i;

	for (;;)
	{
		*leds = (uint32_t)cnt;

		for (i = 0; i < 1000; ++i)
			;

		++cnt;
	}
}