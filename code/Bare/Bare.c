

void main()
{
	unsigned char* video = (unsigned char*)0x10000000;
	unsigned char cnt = 1;
	for (;;)
	{
		*(unsigned char*)video = cnt;
		cnt++;
	}
}