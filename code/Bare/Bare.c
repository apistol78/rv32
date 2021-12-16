
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef void (*call_fn_t)();

void main()
{
	volatile uint8_t* video = (volatile uint8_t*)0x40000000;
	for (;;)
	{
		*video = 'H';
		*video = 'e';
		*video = 'l';
		*video = 'l';
		*video = 'o';
		*video = ' ';
		*video = 'W';
		*video = 'o';
		*video = 'r';
		*video = 'l';
		*video = 'd';
		*video = '\n';
	}
}