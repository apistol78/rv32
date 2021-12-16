

void main()
{
	unsigned char* video = (unsigned char*)0x10000000;
	unsigned char* sram = (unsigned char*)0x00030000;
	unsigned char cnt = 1;
	unsigned char tmp;

	for (;;)
	{
		*(unsigned char*)sram = cnt;
		tmp = *(unsigned char*)sram;
		*(unsigned char*)video = tmp;
		cnt++;
	}
}