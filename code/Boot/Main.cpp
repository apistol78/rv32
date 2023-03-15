/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Runtime/Input.h"
#include "Runtime/File.h"
#include "Runtime/Kernel.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/SystemRegisters.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/UART.h"
#include "Runtime/HAL/Video.h"

#include "Console.h"
#include "ELF.h"

static void cmd_clear(const char* args);
static void cmd_reboot(const char* args);
static void cmd_list(const char* args);
static void cmd_remove(const char* filename);
static void cmd_run(const char* filename);
static void cmd_download(const char* filename);
static void cmd_sysinfo(const char* args);
static void cmd_iotest(const char* args);
static void cmd_memtest(const char* args);
static void cmd_rstvid(const char* args);
static void cmd_help(const char* args);

const struct cmdMap_t
{
	const char* name;
	void (*fn)(const char* args);
	const char* description;
}
c_cmds[] =
{
	{ "clear",		cmd_clear,		"Clear console"	},
	{ "reboot",		cmd_reboot,		"Cold reboot"	},
	{ "ls",			cmd_list,		"List files"	},
	{ "rm",			cmd_remove,		"Remove file"	},
	{ "run",		cmd_run,		"Run program"	},
	{ "dl",			cmd_download,	"Download file"	},
	{ "sysinfo",	cmd_sysinfo,	"System info"	},
	{ "iotest",		cmd_iotest,		"I/O test"		},
	{ "memtest",	cmd_memtest,	"Memory test"	},
	{ "rstvid",		cmd_rstvid,		"Reset video"	},
	{ "help",		cmd_help,		"Show help"		}
};

static void cmd_clear(const char* args)
{
	fb_clear();
}

static void cmd_reboot(const char* args)
{
	runtime_cold_restart();
}

static void cmd_list(const char* args)
{
	file_enumerate("", nullptr, [](void*, const char* filename, uint32_t size, uint8_t directory) {
		if (!directory)
			fb_printf("%-12s : %d\n", filename, size);
		else
			fb_printf("%-12s : <DIR>\n", filename);
	});
}

static void cmd_remove(const char* filename)
{
	file_remove(filename);
}

static void cmd_run(const char* filename)
{
	int32_t fp = file_open(filename, FILE_MODE_READ);
	if (fp <= 0)
	{
		fb_print("UNABLE TO LOAD PROGRAM.\n");
		return;
	}

	static char tmp[1025] = {};
	uint32_t jstart = 0;

	fb_print("Reading header...\n");

	static ELF32_Header hdr = {};
	file_read(fp, (uint8_t*)&hdr, sizeof(hdr));
	if (hdr.e_machine != 0xf3)
	{
		fb_printf("Incorrect machine type %02x.\n", hdr.e_machine);
		return;
	}

	fb_printf("Reading %d sections...\n", hdr.e_shnum);
	for (uint32_t i = 0; i < hdr.e_shnum; ++i)
	{
		static ELF32_SectionHeader shdr = {};
		file_seek(fp, hdr.e_shoff + i * sizeof(ELF32_SectionHeader), FILE_SEEK_SET);
		file_read(fp, (uint8_t*)&shdr, sizeof(shdr));

		if (
			shdr.sh_type == 0x01 ||	// SHT_PROGBITS
			shdr.sh_type == 0x0e ||	// SHT_INIT_ARRAY
			shdr.sh_type == 0x0f	// SHT_FINI_ARRAY
		)
		{
			if ((shdr.sh_flags & 0x02) == 0x02)	// SHF_ALLOC
			{
				fb_printf("0x%08x (%d bytes)...\n", shdr.sh_addr, shdr.sh_size);
				file_seek(fp, shdr.sh_offset, FILE_SEEK_SET);
				for (uint32_t i = 0; i < shdr.sh_size; i += 256)
				{
					uint32_t nb = shdr.sh_size - i;
					if (nb > 256)
						nb = 256;

					int32_t r = (int32_t)file_read(fp, (uint8_t*)(shdr.sh_addr + i), nb);
					if (r != nb)
					{
						fb_printf("READ ERROR (%d).\n", r);
						file_close(fp);
						return;
					}
				}
			}
		}
		else if (shdr.sh_type == 0x02)	// SHT_SYMTAB
		{
			if (jstart != 0)
				continue;

			static ELF32_SectionHeader shdr_link;
			file_seek(fp, hdr.e_shoff + shdr.sh_link * sizeof(ELF32_SectionHeader), FILE_SEEK_SET);
			file_read(fp, (uint8_t*)&shdr_link, sizeof(shdr_link));

			for (int32_t j = 0; j < shdr.sh_size; j += sizeof(ELF32_Sym))
			{
				ELF32_Sym sym = {};
				file_seek(fp, shdr.sh_offset + j, FILE_SEEK_SET);
				file_read(fp, (uint8_t*)&sym, sizeof(sym));

				if (sym.st_size >= sizeof(tmp) - 1)
					continue;

				file_seek(fp, shdr_link.sh_offset + sym.st_name, FILE_SEEK_SET);
				file_read(fp, (uint8_t*)tmp, sym.st_size);

				tmp[sym.st_size] = 0;

				if (strcmp(tmp, "_start") == 0)
				{
					jstart = sym.st_value;
					fb_printf("Start vector 0x%08x\n", jstart);
					break;
				}
			}
		}
	}

	file_close(fp);

	if (jstart != 0)
	{
		kernel_enter_critical();

		__asm__ volatile (
			"fence					\n"
		);

		const uint32_t sp = 0x20000000 + sysreg_read(SR_REG_RAM_SIZE); // - 0x8;
		fb_printf("Launching (SP @ 0x%08x)...\n", sp);

		__asm__ volatile (
			"mv		sp, %0			\n"
			"jr		%1				\n"
			:
			: "r" (sp), "r" (jstart)
		);
	}
}

static void cmd_download(const char* filename)
{
	// Ensure UART is initially empty.
	while (!uart_rx_empty(0))
		uart_rx_u8(0);

	// Create file.
	int32_t fd = file_open(filename, FILE_MODE_WRITE);
	if (fd <= 0) {
		fb_print("UNABLE TO CREATE FILE.\n");
		return;
	}

	fb_print("Downloading...\n");

	uint8_t r[1024];
	int32_t total = 0;

	for (;;)
	{
		const int32_t nb = (int32_t)uart_rx_u32(0);
		if (nb == 0)
			break;

		if (nb > sizeof(r))
		{
			// Invalid packet size.
			fb_printf("ERROR: Invalid pkt size %d\n", nb);
			uart_tx_u8(0, 0x82);
			total = -1;
			break;
		}

		// Receive packet data.
		uint8_t cs = 0;
		for (int32_t i = 0; i < nb; ++i)
		{
			const uint8_t d = uart_rx_u8(0);
			r[i] = d;
			cs ^= d;
		}

		// Verify checksum.
		const uint8_t rxcs = uart_rx_u8(0);
		if (rxcs != cs)
		{
			// Checksum mismatch.
			fb_printf("WARN: Checksum error %02x != %02x\n", cs, rxcs);
			uart_tx_u8(0, 0x81);
			continue;
		}

		// Write data to file.
		if (file_write(fd, r, nb) == nb)
			uart_tx_u8(0, 0x80);
		else
		{
			// Failed to write data to file.
			uart_tx_u8(0, 0x83);
			total = -1;
			break;
		}

		total += nb;
	}

	file_close(fd);

	if (total >= 0)
		fb_printf("\rDownloaded %d bytes\n", total);
	else
	{
		file_remove(filename);
		fb_print("\rDownload failed!\n");
	}
}

static void cmd_sysinfo(const char* args)
{
	switch (sysreg_read(SR_REG_DEVICE_ID))
	{
		case SR_DEVICE_ID_RV32T:
			fb_print("DEVICE   : Rv32T\n");
			break;
		case SR_DEVICE_ID_T_CV_GX:
			fb_print("DEVICE   : Terasic CV GX\n");
			break;
		case SR_DEVICE_ID_Q_CV_2:
			fb_print("DEVICE   : QMTech CV2 - 5CEFA2F23\n");
			break;
		case SR_DEVICE_ID_Q_T7:
			fb_print("DEVICE   : QMTech K7\n");
			break;
		case SR_DEVICE_ID_RV32:
			fb_print("DEVICE   : Rv32\n");
			break;
		case SR_DEVICE_ID_Q_CV_5:
			fb_print("DEVICE   : QMTech CV5 - 5CEFA5F23\n");
			break;
		default:
			fb_print("DEVICE   : UNKNOWN\n");
			break;
	}

	fb_printf("FREQUENCY: %d MHz\n", sysreg_read(SR_REG_FREQUENCY) / 1000000);
	fb_printf("MEMORY   : %d KiB\n", sysreg_read(SR_REG_RAM_SIZE) / 1024);
}

static void cmd_iotest(const char* args)
{
	const int32_t blks[] = { 16, 32, 64, 128, 256, 512, 1024 };
	uint8_t buf[1024];
	{
		fb_printf("Running HAL I/O...\n");
		for (int32_t i = 0; i < sizeof(blks) / sizeof(blks[0]); ++i)
		{
			const int32_t fp = file_open("doom1.wad", FILE_MODE_READ);
			if (fp > 0)
			{
				const uint32_t ms0 = timer_get_ms();
				uint32_t nbytes = 0;
				while (nbytes < 16 * 1024)
				{
					int32_t r = file_read(fp, buf, blks[i]);
					if (r <= 0)
						break;

					nbytes += (uint32_t)r;
				}
				const uint32_t ms1 = timer_get_ms();
				file_close(fp);
				fb_printf("%4d: %d bytes/s\n", blks[i], (nbytes * 1000) / (ms1 - ms0));
			}
			else
			{
				fb_printf("File error; unable to open\n");
				return;
			}
		}
	}
	{
		fb_printf("Running CRT I/O...\n");
		for (int32_t i = 0; i < sizeof(blks) / sizeof(blks[0]); ++i)
		{
			FILE* fp = fopen("doom1.wad", "rb");
			if (fp)
			{
				const uint32_t ms0 = timer_get_ms();
				uint32_t nbytes = 0;
				while (nbytes < 16 * 1024)
				{
					int32_t r = fread(buf, 1, blks[i], fp);
					if (r <= 0)
						break;

					nbytes += (uint32_t)r;
				}
				const uint32_t ms1 = timer_get_ms();
				fclose(fp);
				fb_printf("%4d: %d bytes/s\n", blks[i], (nbytes * 1000) / (ms1 - ms0));
			}
			else
			{
				fb_printf("File error; unable to open\n");
				return;
			}
		}
	}
	{
		fb_printf("Running CRT file...\n");

		int32_t fp = file_open("dummy.txt", FILE_MODE_WRITE);
		if (fp)
		{
			const char buf[] = { "Hello world!\n" };
			int32_t r = file_write(fp, (const uint8_t*)buf, sizeof(buf));
			file_close(fp);
			fb_printf("File written, result %d\n", r);
		}
		else
		{
			fb_printf("File error; unable to create\n");
			return;
		}

		fp = file_open("dummy.txt", FILE_MODE_READ);
		if (fp)
		{
			char buf[64];
			int32_t r = file_read(fp, (uint8_t*)buf, sizeof(buf));
			file_close(fp);
			fb_printf("File read, result %d\n", r);
			fb_printf("%s\n", buf);
		}
		else
		{
			fb_printf("File error; unable to open\n");
			return;
		}		
	}
}

static void cmd_memtest(const char* args)
{
	volatile uint32_t* mb = (uint32_t*)malloc(2 * 1024 * 1024);
	if (!mb)
	{
		fb_printf("Unable to allocate memory\n");
		return;
	}

	kernel_enter_critical();

	const uint32_t t0 = timer_get_ms();

	for (int n = 0; n < 100; ++n)
	{
		volatile uint32_t* ptr = mb;
		for (int i = 0; i < 2 * 1024 * 1024 / 4; i += 8)
		{
			*ptr++ = 0xcafebabe;
			*ptr++ = 0xcafebabe;
			*ptr++ = 0xcafebabe;
			*ptr++ = 0xcafebabe;
			*ptr++ = 0xcafebabe;
			*ptr++ = 0xcafebabe;
			*ptr++ = 0xcafebabe;
			*ptr++ = 0xcafebabe;
		}
	}

	const uint32_t t1 = timer_get_ms();

	kernel_leave_critical();

	free((void*)mb);

	fb_printf("%d KiB written in %d ms\n", 2 * 1024 * 100, t1 - t0);
}

static void cmd_rstvid(const char* args)
{
	video_init();
}

static void cmd_help(const char* args)
{
	for (int i = 0; i < sizeof(c_cmds) / sizeof(c_cmds[0]); ++i)
	{
		fb_printf(
			"%7s - %s\n",
			c_cmds[i].name,
			c_cmds[i].description
		);
	}
}

// void beforeMain (void) __attribute__((constructor));
// void beforeMain (void)
// {
// 	runtime_init();
// }

int main(int argc, const char** argv)
{
	char cmd[128];
	int32_t cnt = 0;

	runtime_init();

	fb_init();
	fb_clear();
	fb_print("BurkenOS 1.0\n");
	fb_print("READY.\n");

	sysreg_write(SR_REG_LEDS, 0);

	for(;;)
	{
		runtime_update();
		
		uint8_t kc, m, p;
		if (input_get_kb_event(&kc, &m, &p) <= 0)
			continue;

		if (p)
		{
			if (kc == RT_KEY_ESCAPE)
			{
				while (cnt > 0)
				{
					fb_putc('\b');
					cnt--;
				}
				cmd[0] = 0;
			}
			else
			{
				char ch;
				if (input_translate_key(kc, m, &ch))
				{
					fb_putc(ch);
					if (ch == '\b')
					{
						if (cnt > 0)
							cmd[--cnt] = 0;
					}
					else if (ch != '\n')
					{
						if (cnt < sizeof(cmd) - 1 && (cnt > 0 || ch != ' '))
							cmd[cnt++] = ch;
					}
					else
					{
						if (cnt > 0)
						{
							cmd[cnt] = 0;

							char* args = cmd;
							while (*args != 0)
							{
								if (*args == ' ')
								{
									*args++ = 0;
									break;
								}
								++args;
							}
							while (*args == ' ')
								++args;

							const cmdMap_t* cm = nullptr;
							for (int i = 0; i < sizeof(c_cmds) / sizeof(c_cmds[0]); ++i)
							{
								if (strcasecmp(c_cmds[i].name, cmd) == 0)
								{
									cm = &c_cmds[i];
									break;
								}
							}

							if (cm)
								cm->fn(args);
							else
								fb_print("SYNTAX ERROR.\n");

							cnt = 0;
						}
					}
				}
			}
		}
	}

	return 0;
}
