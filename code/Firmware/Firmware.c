#include <stddef.h>
#include <string.h>
#include "Firmware/HAL/SD.h"
#include "Firmware/HAL/UART.h"
#include "Firmware/ELF.h"
#include "Firmware/File.h"

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
	const char c_hex[] = { "0123456789abcdef" };
	for (int i = 1; i >= 0; --i)
		uart_tx_u8(0, c_hex[(v >> (i * 4)) & 15]);
}

static void uart_tx_printHex(uint32_t v)
{
	const char c_hex[] = { "0123456789abcdef" };
	for (int i = 7; i >= 0; --i)
		uart_tx_u8(0, c_hex[(v >> (i * 4)) & 15]);
}

static void fatal_error(uint8_t error)
{
	for (;;)
	{
		*LED_BASE = 0x80 | error;
		for (uint32_t i = 0; i < 1000000; ++i)
			__asm__ volatile ("nop");
		*LED_BASE = 0x00 | error;
		for (uint32_t i = 0; i < 1000000; ++i)
			__asm__ volatile ("nop");
	}
}

void main()
{
	// Initialize data.
	{
		extern uint32_t _data_rom;
		extern uint32_t _data_ram;
		extern uint32_t _data_ram_end;
		memcpy((void*)_data_ram, (const void*)_data_rom, _data_ram_end - _data_ram);
	}

	uart_tx_print("Booting...\n");

	// Initialize SD card reader.
	sd_init();

	// Initialize file system.
	file_init();

	// Read kernal.elf into memory.
	int32_t fd = file_open("KERNAL");
	if (fd > 0)
	{
		char tmp[256] = {};
		const uint32_t sp = 0x20FFFFF0;
		uint32_t jstart = 0;

		uart_tx_print("Loading KERNAL...\n");

		ELF32_Header hdr = {};
		file_read(fd, &hdr, sizeof(hdr));
		if (hdr.e_machine != 0xf3)
			fatal_error(1);

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
					for (uint32_t i = 0; i < shdr.sh_size; i += 16)
					{
						uint32_t nb = shdr.sh_size - i;
						if (nb > 16)
							nb = 16;
						if (file_read(fd, (void*)(shdr.sh_addr + i), nb) != nb)
							fatal_error(2);
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
						jstart = sym.st_value;
				}
			}
		}

		file_close(fd);

		if (jstart != 0)
		{
			uart_tx_print("Starting KERNAL...\n");
			__asm__ volatile (
				"fence					\n"
				"mv		sp, %0			\n"
				:
				: "r" (sp)
			);

			((call_fn_t)jstart)();
		}
		
		// If we've reach here then we cannot continue.
		uart_tx_print("Unknown error\n");
		fatal_error(3);
	}
	else	// No kernal.elf found, fall back to UART.
	{
		uart_tx_print("No KERNAL found, waiting for UART...\n");

		for (;;)
		{
			uint8_t cmd = uart_rx_u8(0);
			*LED_BASE = (uint32_t)cmd;

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

					// Ensure all data is written to SDRAM before calling user program.
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
