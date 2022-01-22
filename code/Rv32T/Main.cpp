#if defined(__LINUX__) || defined(__RPI__) || defined(__APPLE__)
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <signal.h>
#	include <stdio.h>
#	include <stdlib.h>
#	include <fcntl.h>
#	include <errno.h>
#	include <unistd.h>
#	include <syslog.h>
#	include <string.h>
#endif
#include <Core/Io/DynamicMemoryStream.h>
#include <Core/Io/FileSystem.h>
#include <Core/Io/StreamCopy.h>
#include <Core/Log/Log.h>
#include <Core/Misc/CommandLine.h>
#include <Core/Misc/SafeDestroy.h>
#include <Core/Misc/String.h>
#include <Ui/Application.h>
#include <Ui/Bitmap.h>
#include <Ui/Button.h>
#include <Ui/Image.h>
#include <Ui/Form.h>
#include <Ui/FloodLayout.h>
#include <Ui/TableLayout.h>
#if defined(_WIN32)
#	include <Ui/Win32/WidgetFactoryWin32.h>
#elif defined(__APPLE__)
#	include <Ui/Cocoa/WidgetFactoryCocoa.h>
#elif defined(__LINUX__) || defined(__RPI__)
#	include <Ui/X11/WidgetFactoryX11.h>
#endif
#include "Rv32T/ELF.h"
#include "Rv32T/HDMI.h"
#include "Rv32T/LEDR.h"
#include "Rv32T/SD.h"
#include "Rv32T/UART_TX.h"

// Verilated SoC
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "SoC/VSoC.h"

using namespace traktor;

bool g_going = true;

#if defined(__LINUX__) || defined(__RPI__) || defined(__APPLE__)
void abortHandler(int s)
{
	g_going = false;
}
#endif

bool loadELF(VSoC* soc, const std::wstring& fileName)
{
	AlignedVector< uint8_t > elf;
	uint32_t start = -1;

	// Read entire ELF into memory.
	{
		Ref< IStream > f = FileSystem::getInstance().open(fileName, File::FmRead);
		if (!f)
		{
			log::error << L"Unable to open ELF \"" << fileName << L"\"." << Endl;
			return false;
		}

		DynamicMemoryStream dms(elf, false, true);
		if (!StreamCopy(&dms, f).execute())
		{
			log::error << L"Unable to open ELF \"" << fileName << L"\"; failed to read file." << Endl;
			return false;
		}
	}

	auto hdr = (const ELF32_Header*)elf.c_ptr();

	if (hdr->e_machine != 0xF3)
	{
		log::error << L"Unable to parse ELF \"" << fileName << L"\"; incorrect machine type." << Endl;
		return false;		
	}

	auto shdr = (const ELF32_SectionHeader*)(elf.c_ptr() + hdr->e_shoff);
	for (uint32_t i = 0; i < hdr->e_shnum; ++i)
	{
		if (
			shdr[i].sh_type == 0x01 ||	// SHT_PROGBITS
			shdr[i].sh_type == 0x0e ||	// SHT_INIT_ARRAY
			shdr[i].sh_type == 0x0f		// SHT_FINI_ARRAY
		)
		{
			if ((shdr[i].sh_flags & 0x02) == 0x02)	// SHF_ALLOC
			{
				const auto pbits = (const uint8_t*)(elf.c_ptr() + shdr[i].sh_offset);
				const uint32_t addr = shdr[i].sh_addr;

				const uint32_t base = 0x20000000;

				uint8_t* dst = (uint8_t*)soc->SoC__DOT__sdram__DOT__data;
				for (uint32_t j = 0; j < shdr[i].sh_size; ++j)
					dst[addr - base + j] = pbits[j];
			}
		}
		else if (shdr[i].sh_type == 0x02)	// SHT_SYMTAB
		{
			const char* strings = (const char*)(elf.c_ptr() + shdr[shdr[i].sh_link].sh_offset);
			auto sym = (const ELF32_Sym*)(elf.c_ptr() + shdr[i].sh_offset);
			for (int32_t j = 0; j < shdr[i].sh_size / sizeof(ELF32_Sym); ++j)
			{
				const char* name = strings + sym[j].st_name;
				if (strcmp(name, "_start") == 0)
				{
					log::info << L"Entry found, " << str(L"0x%08x", sym[j].st_value) << Endl;
					start = sym[j].st_value;
					break;
				}
			}
		}
	}

	if (start != -1)
		soc->SoC__DOT__cpu__DOT__fetch__DOT__pc = start;

	return true;
}

int main(int argc, const char **argv)
{
	CommandLine cmdLine(argc, argv);

#if defined(_WIN32)
	ui::Application::getInstance()->initialize(
		new ui::WidgetFactoryWin32(),
		nullptr
	);
#elif defined(__APPLE__)
	ui::Application::getInstance()->initialize(
		new ui::WidgetFactoryCocoa(),
		nullptr
	);
#elif defined(__LINUX__) || defined(__RPI__)
	ui::Application::getInstance()->initialize(
		new ui::WidgetFactoryX11(),
		nullptr
	);
#endif

#if defined(__LINUX__) || defined(__RPI__) || defined(__APPLE__)
	{
		struct sigaction sa = { SIG_IGN };
		sigaction(SIGPIPE, &sa, nullptr);
	}
	{
		struct sigaction sa;
		sa.sa_handler = abortHandler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sigaction(SIGINT, &sa, nullptr);
	}
#endif

	bool trace = false;

	// Create user interface.
	Ref< ui::Form > form = new ui::Form();
	form->create(L"RV32", ui::dpi96(640), ui::dpi96(400), ui::Form::WsDefault, new ui::TableLayout(L"100%", L"*,100%", 4, 4));

	Ref< ui::Container > container = new ui::Container();
	container->create(form, ui::WsNone, new ui::TableLayout(L"*", L"*", 0, 4));

	Ref< ui::Button > buttonTrace = new ui::Button();
	buttonTrace->create(container, L"Trace");
	buttonTrace->addEventHandler< ui::ButtonClickEvent >([&](ui::ButtonClickEvent* event) {
		buttonTrace->setEnable(false);
		trace = true;
	});

	Ref< ui::Bitmap > framebuffer = new ui::Bitmap(320, 200);
	
	Ref< ui::Image > image = new ui::Image();
	image->create(form, framebuffer, ui::Image::WsScale);

	form->update();
	form->show();

	// Create SoC simulation.
	VSoC* soc = new VSoC();
	soc->CPU_RESET_n = 1;
	soc->CLOCK_125_p = 0;
	soc->eval();

	if (cmdLine.hasOption(L'e', L"elf"))
	{
		std::wstring fileName = cmdLine.getOption(L'e', L"elf").getString();
		if (!loadELF(soc, fileName))
			return 1;
	}

	soc->SoC__DOT__cpu__DOT__registers__DOT__r[2] = 0x0001fff0;

	uint32_t intercept = 0xffffffff;
	if (cmdLine.hasOption(L"intercept"))
		intercept = cmdLine.getOption(L"intercept").getInteger();

	// Create signal trace.
	VerilatedVcdC* tfp = nullptr;
	if (cmdLine.hasOption(L't', L"trace"))
		trace = true;

	HDMI hdmi;
	LEDR ledr;
	UART_TX uart_tx;
	SD sd;

	int32_t time = 0;
	while (g_going)
	{
		if (!ui::Application::getInstance()->process())
			break;

		if (trace)
		{
			Verilated::traceEverOn(true);
			tfp = new VerilatedVcdC;
			soc->trace(tfp, 99);  // Trace 99 levels of hierarchy
			tfp->open("Rv32T.vcd");
			trace = false;	
		}

		for (int32_t i = 0; i < 50000; ++i)
		{
			++time;
			soc->CLOCK_125_p = 0;
			soc->eval();

			if (tfp)
				tfp->dump(time);

			++time;
			soc->CLOCK_125_p = 1;
			soc->eval();

			if (tfp)
				tfp->dump(time);

			hdmi.eval(soc);
			ledr.eval(soc);
			uart_tx.eval(soc);
			sd.eval(soc);

			// TODO
			// Check output of each CPU pipeline stage
			// are kept during entire output tag cycle.

			if (soc->SoC__DOT__cpu__DOT__fetch__DOT__pc == intercept)
			{
				log::error << L"Intercepted at 0x" << str(L"%08x", intercept) << Endl;
				g_going = false;
				break;
			}
		}

		framebuffer->copyImage(hdmi.getImage());
		image->setImage(framebuffer);
	}

	log::info << Endl;
	log::info << L"--- Terminated ---" << Endl;
	log::info << L"PC     : " << str(L"%08x", soc->SoC__DOT__cpu__DOT__fetch__DOT__pc) << Endl;
	log::info << L"CYCLES : " << soc->SoC__DOT__timer__DOT__cycles << Endl;
	log::info << L"RETIRE : " << soc->SoC__DOT__cpu_retire_count << Endl;
	log::info << L"MS     : " << soc->SoC__DOT__timer__DOT__ms << Endl;

	if (tfp)
	{
		tfp->close();
		delete tfp;
	}

	safeDestroy(form);
	return 0;
}