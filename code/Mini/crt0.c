

/*
Copy from data segments ROM to RAM
*/
/*
extern unsigned int _data_ram;
extern unsigned int _data_ram_end;
extern unsigned int _data_rom;

void __init_data()
{
    unsigned int* src;
    unsigned int* dest;
	unsigned int i;

	src = &_data_rom;
	dest = &_data_ram;
	while (dest < &_data_ram_end)
		*(dest++) = *(src++);
}
*/
