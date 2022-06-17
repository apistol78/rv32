#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "Firmware/ELF.h"
#include "Runtime/CRT.h"
#include "Runtime/File.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/SD.h"
#include "Runtime/HAL/SystemRegisters.h"
#include "Runtime/HAL/UART.h"

typedef void (*call_fn_t)();

static uint32_t uart_rx_u32(uint8_t port)
{
	uint8_t tmp[4];
	tmp[0] = uart_rx_u8(port);
	tmp[1] = uart_rx_u8(port);
	tmp[2] = uart_rx_u8(port);
	tmp[3] = uart_rx_u8(port);
	return *(uint32_t*)tmp;
}

static void uart_tx_print(const char* txt)
{
	for (const char* c = txt; *c; ++c)
		uart_tx_u8(0, *c);
}

static void uart_tx_printHex8(uint32_t v)
{
	static const char c_hex[] = { "0123456789abcdef" };
	for (int i = 1; i >= 0; --i)
		uart_tx_u8(0, c_hex[(v >> (i * 4)) & 15]);
}

static void uart_tx_printHex(uint32_t v)
{
	static const char c_hex[] = { "0123456789abcdef" };
	for (int i = 7; i >= 0; --i)
		uart_tx_u8(0, c_hex[(v >> (i * 4)) & 15]);
}

static void fatal_error(uint8_t error)
{
	for (;;)
	{
		sysreg_write(SR_REG_LEDS, 0x80 | error);
		for (uint32_t i = 0; i < 1000000; ++i)
			__asm__ volatile ("nop");
		sysreg_write(SR_REG_LEDS, 0x00 | error);
		for (uint32_t i = 0; i < 1000000; ++i)
			__asm__ volatile ("nop");
	}
}

static int32_t launch_elf(const char* filename)
{
	printf("open \"%s\"...\n", filename);

	int32_t fd = file_open(filename);
	if (fd <= 0)
	{
		printf("unable to open \"%s\"\n", filename);
		return 1;
	}

	char tmp[256] = {};
	const uint32_t sp = 0x20FFFFF0;
	uint32_t jstart = 0;

	printf("read header ...\n");

	ELF32_Header hdr = {};
	file_read(fd, (uint8_t*)&hdr, sizeof(hdr));
	if (hdr.e_machine != 0xf3)
	{
		printf("invalid ELF header\n");
		return 2;
	}

	printf("reading %d sections...\n", hdr.e_shnum);
	for (uint32_t i = 0; i < hdr.e_shnum; ++i)
	{
		ELF32_SectionHeader shdr = {};
		file_seek(fd, hdr.e_shoff + i * sizeof(ELF32_SectionHeader), 0);
		file_read(fd, (uint8_t*)&shdr, sizeof(shdr));

		if (
			shdr.sh_type == 0x01 ||	// SHT_PROGBITS
			shdr.sh_type == 0x0e ||	// SHT_INIT_ARRAY
			shdr.sh_type == 0x0f	// SHT_FINI_ARRAY
		)
		{
			if ((shdr.sh_flags & 0x02) == 0x02)	// SHF_ALLOC
			{
				printf("reading section at 0x%08x (%d bytes)...\n", shdr.sh_addr, shdr.sh_size);
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
			file_read(fd, (uint8_t*)&shdr_link, sizeof(shdr_link));

			for (int32_t j = 0; j < shdr.sh_size; j += sizeof(ELF32_Sym))
			{
				ELF32_Sym sym = {};
				file_seek(fd, shdr.sh_offset + j, 0);
				file_read(fd, (uint8_t*)&sym, sizeof(sym));

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
		printf("launching application...\n");
		__asm__ volatile (
			"fence					\n"
			"mv		sp, %0			\n"
			:
			: "r" (sp)
		);
		((call_fn_t)jstart)();
	}
	
	printf("no start address\n");
	return 4;
}

void main()
{
	// Initialize SP, since we hot restart and startup doesn't set SP.
	const uint32_t sp = 0x20110000;
	__asm__ volatile (
		"mv sp, %0	\n"
		:
		: "r" (sp)
	);

	// Initialize segments when running from ROM.
	{
		extern uint8_t INIT_DATA_VALUES;
		extern uint8_t INIT_DATA_START;
		extern uint8_t INIT_DATA_END;
		uint8_t* src = (uint8_t*)&INIT_DATA_VALUES;
		uint8_t* dest = (uint8_t*)&INIT_DATA_START;
		uint32_t len = (uint32_t)(&INIT_DATA_END - &INIT_DATA_START);
		while (len--)
			*dest++ = *src++;		
	}
	{
		extern uint8_t INIT_SDATA_VALUES;
		extern uint8_t INIT_SDATA_START;
		extern uint8_t INIT_SDATA_END;
		uint8_t* src = (uint8_t*)&INIT_SDATA_VALUES;
		uint8_t* dest = (uint8_t*)&INIT_SDATA_START;
		uint32_t len = (uint32_t)(&INIT_SDATA_END - &INIT_SDATA_START);
		while (len--)
			*dest++ = *src++;		
	}
	{
		extern uint8_t BSS_START;
		extern uint8_t BSS_END;
        uint8_t* dest = (uint8_t*)&BSS_START;
        uint32_t len = (uint32_t)(&BSS_END - &BSS_START);
        while (len--)
                *dest++=0;		
	}
	{
		extern uint8_t SBSS_START;
		extern uint8_t SBSS_END;
        uint8_t* dest = (uint8_t*)&SBSS_START;
        uint32_t len = (uint32_t)(&SBSS_END - &SBSS_START);
        while (len--)
                *dest++=0;		
	}

	crt_init();

	printf("===============================================================================\n");
	printf("                                 Rebel-V SoC                                   \n");
	printf("                           created by Anders Pistol                            \n");
	printf("                                    2022                                       \n");
	printf("-------------------------------------------------------------------------------\n");
	printf("                             firmware version 0.1                              \n");
	printf("===============================================================================\n");

	printf("initialize storage...\n");
	sd_init();

	printf("initialize file system...\n");
	file_init();

	if ((sysreg_read(SR_REG_BM0) & 0x01) == 0x00)
	{
		launch_elf("boot.elf");
		for (;;);
	}
	else
	{
		printf("waiting on UART...\n");
		for (;;)
		{
			uint8_t cmd = uart_rx_u8(0);
			sysreg_write(SR_REG_LEDS, cmd);

			// poke
			if (cmd == 0x01)
			{
				uint32_t addr = uart_rx_u32(0);
				uint8_t nb = uart_rx_u8(0);
				uint8_t cs = 0;

				if (nb == 0)
				{
					uart_tx_u8(0, 0x81);	// Invalid data.
					continue;
				}

				// Add address to checksum.
				const uint8_t* p = (const uint8_t*)&addr;
				cs ^= p[0];
				cs ^= p[1];
				cs ^= p[2];
				cs ^= p[3];

				// Receive 
				uint8_t r[256];
				for (uint8_t i = 0; i < nb; ++i)
				{
					uint8_t d = uart_rx_u8(0);
					r[i] = d;
					cs ^= d;
				}

				if (cs == uart_rx_u8(0))
				{
					// Write data to memory.
					for (uint8_t i = 0; i < nb; ++i)
						*(uint8_t*)(addr + i) = r[i];

					// Verify data written to memory.
					uint32_t result = 0x80;
					for (uint8_t i = 0; i < nb; ++i)
					{
						if (*(uint8_t*)(addr + i) != r[i])
						{
							result = 0x83;
							break;
						}
					}

					uart_tx_u8(0, result);
				}
				else
					uart_tx_u8(0, 0x82);	// Invalid checksum.
			}

			// peek
			else if (cmd == 0x02)
			{
				uint32_t addr = uart_rx_u32(0);
				uint8_t nb = uart_rx_u8(0);

				if (nb == 0)
				{
					uart_tx_u8(0, 0x81);	// Invalid data.
					continue;
				}

				// Ensure DCACHE is flushed.
				__asm__ volatile ("fence");

				uart_tx_u8(0, 0x80);	// Ok

				for (uint8_t i = 0; i < nb; ++i)
					uart_tx_u8(0, *(uint8_t*)addr++);
			}

			// jump to
			else if (cmd == 0x03)
			{
				uint32_t addr = uart_rx_u32(0);
				uint32_t sp = uart_rx_u32(0);
				uint8_t cs = 0;

				// Add address to checksum.
				{
					const uint8_t* p = (const uint8_t*)&addr;
					cs ^= p[0];
					cs ^= p[1];
					cs ^= p[2];
					cs ^= p[3];
				}

				// Add stack to checksum.
				{
					const uint8_t* p = (const uint8_t*)&sp;
					cs ^= p[0];
					cs ^= p[1];
					cs ^= p[2];
					cs ^= p[3];
				}

				if (cs == uart_rx_u8(0))
				{
					uart_tx_u8(0, 0x80);	// Ok

					// Ensure DCACHE is flushed.
					__asm__ volatile ("fence");
					
					if (sp != 0)
					{
						__asm__ volatile (
							"mv	sp, %0\n"
							:
							: "r" (sp)
						);
					}
					
					((call_fn_t)addr)();
				}
				else
					uart_tx_u8(0, 0x82);	// Invalid checksum.
			}

			// echo
			else
				uart_tx_u8(0, cmd);
		}
	}
}
