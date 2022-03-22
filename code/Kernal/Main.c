#include <stdio.h>
#include <string.h>
#include "Kernal/ELF.h"
#include "Kernal/File.h"
#include "Kernal/Kernal.h"
#include "Kernal/HAL/DMA.h"
#include "Kernal/HAL/SD.h"
#include "Kernal/HAL/Timer.h"
#include "Kernal/HAL/UART.h"
#include "Kernal/HAL/Video.h"

typedef void (*call_fn_t)();

kernal_t krn __attribute__ ((section (".kernal"))) =
{
	0,
	1,

	// file i/o
	&file_open,
	&file_close,
	&file_size,
	&file_seek,
	&file_write,
	&file_read,

	// uart
	&uart_tx_u8,
	&uart_rx_full,
	&uart_rx_empty,
	&uart_rx_u8,

	// timer
	&timer_get_ms,
	&timer_wait_ms,
	&timer_get_cycles,
	&timer_set_compare,

	// dma
	&dma_copy,
	&dma_wait
};

extern void crt_init();

int32_t launch_elf(const char* filename)
{
	printf("Open file ...\n");

	int32_t fd = file_open(filename);
	if (fd <= 0)
	{
		printf("Unable to open \"%s\"\n", filename);
		return 1;
	}

	char tmp[256] = {};
	const uint32_t sp = 0x20FFFFF0;
	uint32_t jstart = 0;

	printf("Read header ...\n");

	ELF32_Header hdr = {};
	file_read(fd, &hdr, sizeof(hdr));
	if (hdr.e_machine != 0xf3)
	{
		printf("Invalid ELF header\n");
		return 2;
	}

	printf("Reading %d sections...\n", hdr.e_shnum);
	for (uint32_t i = 0; i < hdr.e_shnum; ++i)
	{
		ELF32_SectionHeader shdr = {};
		file_seek(fd, hdr.e_shoff + i * sizeof(ELF32_SectionHeader), 0);
		file_read(fd, &shdr, sizeof(shdr));

		if (
			shdr.sh_type == 0x01 ||	// SHT_PROGBITS
			shdr.sh_type == 0x0e ||	// SHT_INIT_ARRAY
			shdr.sh_type == 0x0f	// SHT_FINI_ARRAY
		)
		{
			if ((shdr.sh_flags & 0x02) == 0x02)	// SHF_ALLOC
			{
				file_seek(fd, shdr.sh_offset, 0);
				for (uint32_t i = 0; i < shdr.sh_size; i += 512)
				{
					uint32_t nb = shdr.sh_size - i;
					if (nb > 512)
						nb = 512;
					if (file_read(fd, (void*)(shdr.sh_addr + i), nb) != nb)
						return 3;
				}
			}
		}
		else if (shdr.sh_type == 0x02)	// SHT_SYMTAB
		{
			ELF32_SectionHeader shdr_link;
			file_seek(fd, hdr.e_shoff + shdr.sh_link * sizeof(ELF32_SectionHeader), 0);
			file_read(fd, &shdr_link, sizeof(shdr_link));

			for (int32_t j = 0; j < shdr.sh_size; j += sizeof(ELF32_Sym))
			{
				ELF32_Sym sym = {};
				file_seek(fd, shdr.sh_offset + j, 0);
				file_read(fd, &sym, sizeof(sym));

				file_seek(fd, shdr_link.sh_offset + sym.st_name, 0);
				file_read(fd, tmp, sym.st_size);

				tmp[sym.st_size] = 0;

				if (strcmp(tmp, "_start") == 0)
				{
					jstart = sym.st_value;
					break;
				}
			}
		}
	}

	file_close(fd);

	if (jstart != 0)
	{
		printf("Launching application...\n");
		__asm__ volatile (
			"fence					\n"
			"mv		sp, %0			\n"
			:
			: "r" (sp)
		);
		((call_fn_t)jstart)();
	}
	
	printf("No start address\n");
	return 4;
}

int main()
{
	crt_init();

	printf("KERNAL v%d.%d\n", krn.major, krn.minor);
	printf("-----------\n");

	printf("** Initialize Video **\n");
	if (video_init() != 0)
		printf("Video init failed!\n");

	printf("** Initialize SD card **\n");
	if (sd_init() != 0)
		printf("SD init failed!\n");

	printf("** Initialize FS **\n");
	if (file_init() != 0)
		printf("FS init failed!\n");

	printf("Ready\n");

	launch_elf("bare");
	for (;;)
		;

	return 0;
}
