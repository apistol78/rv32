#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Runtime/Input.h"
#include "Runtime/File.h"
#include "Runtime/Kernel.h"
#include "Runtime/Runtime.h"
#include "Runtime/HAL/Audio.h"
#include "Runtime/HAL/Interrupt.h"
#include "Runtime/HAL/SystemRegisters.h"
#include "Runtime/HAL/Timer.h"
#include "Runtime/HAL/UART.h"

#include "Console.h"
#include "ELF.h"

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
	FILE* fp = fopen(filename, "rb");
	if (!fp) {
		fb_print("UNABLE TO LOAD PROGRAM.\n");
		return;
	}

	static char tmp[1025] = {};
	uint32_t jstart = 0;

	fb_print("Reading header...\n");

	static ELF32_Header hdr = {};
	fread((uint8_t*)&hdr, sizeof(hdr), 1, fp);
	if (hdr.e_machine != 0xf3)
	{
		fb_printf("Incorrect machine type %02x.\n", hdr.e_machine);
		return;
	}

	fb_printf("Reading %d sections...\n", hdr.e_shnum);
	for (uint32_t i = 0; i < hdr.e_shnum; ++i)
	{
		static ELF32_SectionHeader shdr = {};
		fseek(fp, hdr.e_shoff + i * sizeof(ELF32_SectionHeader), SEEK_SET);
		fread((uint8_t*)&shdr, sizeof(shdr), 1, fp);

		if (
			shdr.sh_type == 0x01 ||	// SHT_PROGBITS
			shdr.sh_type == 0x0e ||	// SHT_INIT_ARRAY
			shdr.sh_type == 0x0f	// SHT_FINI_ARRAY
		)
		{
			if ((shdr.sh_flags & 0x02) == 0x02)	// SHF_ALLOC
			{
				fb_printf("0x%08x (%d bytes)...\n", shdr.sh_addr, shdr.sh_size);
				fseek(fp, shdr.sh_offset, SEEK_SET);
				for (uint32_t i = 0; i < shdr.sh_size; i += 256)
				{
					uint32_t nb = shdr.sh_size - i;
					if (nb > 256)
						nb = 256;

					int32_t r = (int32_t)fread((uint8_t*)(shdr.sh_addr + i), nb, 1, fp);
					if (r != 1)
					{
						fb_printf("READ ERROR (%d).\n", r);
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
			fseek(fp, hdr.e_shoff + shdr.sh_link * sizeof(ELF32_SectionHeader), SEEK_SET);
			fread((uint8_t*)&shdr_link, sizeof(shdr_link), 1, fp);

			for (int32_t j = 0; j < shdr.sh_size; j += sizeof(ELF32_Sym))
			{
				ELF32_Sym sym = {};
				fseek(fp, shdr.sh_offset + j, SEEK_SET);
				fread((uint8_t*)&sym, sizeof(sym), 1, fp);

				if (sym.st_size >= sizeof(tmp) - 1)
					continue;

				fseek(fp, shdr_link.sh_offset + sym.st_name, SEEK_SET);
				fread((uint8_t*)tmp, sym.st_size, 1, fp);

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

	fclose(fp);

	if (jstart != 0)
	{
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

	// Wait until first byte is received.
	fb_print("Waiting...\n");
	for (;;)
	{
		runtime_update();
		if (!uart_rx_empty(0))
			break;
		timer_wait_ms(10);
	}

	fb_print("Downloading...\n");

	uint8_t r[128];
	int32_t total = 0;

	for (;;)
	{
		const int32_t nb = (int32_t)uart_rx_u8(0);
		if (nb == 0)
			break;

		if (nb > 128)
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

volatile int g_leds = 0;
volatile int g_queue = 0;

static void cmd_play(const char* filename)
{
	int32_t fp = file_open(filename, FILE_MODE_READ);
	if (fp <= 0)
		return;

	const int32_t size = file_size(fp);
	const int32_t n = size / 2;

	fb_printf("Playing %d samples...\n", n);

	// Interrupt are issued when audio hw buffer gets below 50%.
	interrupt_set_handler(
		IRQ_SOURCE_PLIC_1,
		[](uint32_t source) {
			g_leds++;
			sysreg_write(SR_REG_LEDS, g_leds >> 4);
			g_queue = 1;
		}
	);

	int16_t buf[256];
	for (;;)
	{
		if (file_read(fp, (uint8_t*)buf, 256 * 2) < 256 * 2)
			break;

		g_queue = 0;
		audio_play_mono(buf, 256);

		while (g_queue == 0);

		runtime_update();

		uint8_t kc, m, p;
		if (input_get_kb_event(&kc, &m, &p) > 0)
		{
			if (p != 0 && kc == RT_KEY_ESCAPE)
				break;
		}
	}

	interrupt_set_handler(
		IRQ_SOURCE_PLIC_1,
		0
	);

	file_close(fp);
}

static void cmd_help(const char* args)
{
	fb_print("REBOOT  - Cold reboot\n");
	fb_print("LIST    - List files on SD\n");
	fb_print("REMOVE  - Remove file from SD\n");
	fb_print("RUN     - Run program\n");
	fb_print("DL      - Download file\n");
	fb_print("SYSINFO - System info\n");
	fb_print("PLAY    - Play sound file\n");
	fb_print("HELP    - Show help\n");
}

const struct cmdMap_t
{
	const char* name;
	void (*fn)(const char* args);
}
c_cmds[] =
{
	{ "reboot",		cmd_reboot		},
	{ "list",		cmd_list		},
	{ "remove",		cmd_remove		},
	{ "run",		cmd_run			},
	{ "dl",			cmd_download	},
	{ "sysinfo",	cmd_sysinfo		},
	{ "play",		cmd_play		},
	{ "help",		cmd_help		}
};

int main(int argc, const char** argv)
{
	char cmd[128];
	int32_t cnt = 0;

	runtime_init();

	fb_init();
	fb_clear();
	fb_print("   **** MOJO 5 ** SHELL V1 ****   \n");
	fb_printf(" %-2d MiB RAM, RISC-V CPU @ %-3d MHz\n", sysreg_read(SR_REG_RAM_SIZE) / (1024 * 1024), sysreg_read(SR_REG_FREQUENCY) / 1000000);
	fb_print("\n");
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
			char ch;
			if (input_translate_key(kc, m, &ch))
			{
				fb_putc(ch);
				if (ch != '\n')
				{
					if (cnt > 0 || ch != ' ')
						cmd[cnt++] = ch;
				}
				else if (ch == '\b')
				{
					if (cnt > 0)
						cmd[--cnt] = 0;
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

	return 0;
}
