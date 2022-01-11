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
#include <Core/Containers/CircularVector.h>
#include <Core/Io/FileOutputStream.h>
#include <Core/Io/FileSystem.h>
#include <Core/Io/IStream.h>
#include <Core/Io/Utf8Encoding.h>
#include <Core/Log/Log.h>
#include <Core/Misc/CommandLine.h>
#include <Core/Misc/String.h>
#include <Ui/Application.h>
#include <Ui/Bitmap.h>
#include <Ui/Image.h>
#include <Ui/Form.h>
#include <Ui/FloodLayout.h>
#if defined(_WIN32)
#	include <Ui/Win32/WidgetFactoryWin32.h>
#elif defined(__APPLE__)
#	include <Ui/Cocoa/WidgetFactoryCocoa.h>
#elif defined(__LINUX__) || defined(__RPI__)
#	include <Ui/X11/WidgetFactoryX11.h>
#endif
#include "Rv32/Bus.h"
#include "Rv32/CPU.h"
#include "Rv32/LoadELF.h"
#include "Rv32/LoadHEX.h"
#include "Rv32/Memory.h"
#include "Rv32/UART.h"
#include "Rv32/Unknown.h"
#include "Rv32/Video.h"

using namespace traktor;

// https://github.com/takahirox/riscv-rust/blob/master/src/cpu.rs
// https://mindchasers.com/dev/rv-getting-started

bool g_going = true;

#if defined(__LINUX__) || defined(__RPI__) || defined(__APPLE__)
void abortHandler(int s)
{
	g_going = false;
}
#endif

int main(int argc, const char** argv)
{
	CommandLine cmdLine(argc, argv);

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

	bool headless = cmdLine.hasOption(L"headless");

	if (!headless)
	{
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
	}

	Ref< IStream > uartStream;
	if (cmdLine.hasOption(L'u', L"uart"))
	{
		uartStream = FileSystem::getInstance().open(
			cmdLine.getOption(L'u', L"uart").getString(),
			File::FmRead
		);
		if (!uartStream)
		{
			log::error << L"Unable to open UART stream." << Endl;
			return 1;
		}
	}

	//Memory rom(0x00010000);
	Memory ram(0x10000000);
	Memory sram(0x00400000);
	Memory sdram(0x20000000);
	Video video;
	Unknown led(L"LED");
	UART uart(uartStream);
	Unknown gpio(L"GPIO");
	Unknown i2c(L"I2C");
	Unknown sd(L"SD");

	Bus bus;
	// bus.map(0x00000000, 0x00010000, &rom);
	// bus.map(0x00010000, 0x00010400, &ram);
	bus.map(0x00000000, 0x10000000, &ram);
	bus.map(0x10000000, 0x10400000, &sram);
	bus.map(0x20000000, 0x40000000, &sdram);
	bus.map(0x40000000, 0x50000000, &video);
	bus.map(0x50000000, 0x50000010, &led);
	bus.map(0x50000010, 0x50000020, &uart);
	bus.map(0x50000020, 0x50000030, &gpio);
	bus.map(0x50000030, 0x50000040, &i2c);
	bus.map(0x50000040, 0x50000050, &sd);

	Ref< OutputStream > os = nullptr;	
	if (cmdLine.hasOption(L't', L"trace"))
	{
		Ref< IStream > f = FileSystem::getInstance().open(L"Trace.log", File::FmWrite);
		if (f)
			os = new FileOutputStream(f, new Utf8Encoding());
	}

	CPU cpu(&bus, os);

	if (cmdLine.hasOption(L'e', L"elf"))
	{
		std::wstring fileName = cmdLine.getOption(L'e', L"elf").getString();
		if (!loadELF(fileName, cpu, bus))
			return 1;
	}

	if (cmdLine.hasOption(L'h', L"hex"))
	{
		std::wstring fileName = cmdLine.getOption(L'h', L"hex").getString();
		if (!loadHEX(fileName, cpu, bus))
			return 1;
	}

	// Lock ROM from being mutable.
	//rom.setReadOnly(true);

	if (cmdLine.hasOption(L's', L"start"))
	{
		int32_t start = cmdLine.getOption(L's', L"start").getInteger();
		if (start < 0)
		{
			log::error << L"Invalid start address." << Endl;
			return 1;
		}
		cpu.jump((uint32_t)start);
	}

	if (cmdLine.hasOption(L"stack"))
	{
		int32_t sp = cmdLine.getOption(L"stack").getInteger();
		if (sp < 0)
		{
			log::error << L"Invalid stack address." << Endl;
			return 1;
		}
		cpu.setSP((uint32_t)sp);
	}

	CircularVector< std::pair< uint32_t, uint32_t >, 32 > dbg_pc;
	uint32_t dbg_sp = cpu.sp();

	Ref< ui::Form > form;
	Ref< ui::Bitmap > uiImage;
	Ref< ui::Image > image;
	
	if (!headless)
	{
		form = new ui::Form();
		form->create(L"RV32", ui::dpi96(640), ui::dpi96(480), ui::Form::WsDefault, new ui::FloodLayout());

		uiImage = new ui::Bitmap(320, 240);
		
		image = new ui::Image();
		image->create(form, uiImage, ui::Image::WsScale);

		form->update();
		form->show();
	}

	while (g_going)
	{
		if (!headless)
		{
			if (!ui::Application::getInstance()->process())
				break;

			for (int32_t i = 0; i < 10000; ++i)
			{
				if (dbg_pc.full())
					dbg_pc.pop_front();
				dbg_pc.push_back({ cpu.pc(), cpu.sp() });

				if (!cpu.tick())
				{
					g_going = false;
					break;
				}
				if (bus.error())
				{
					g_going = false;
					break;
				}

				if (video.shouldPresent())
					break;
			}

			if (video.shouldPresent() && video.getImage())
			{
				uiImage->copyImage(video.getImage());
				image->setImage(uiImage);
				video.clearPresent();
			}
		}
		else
		{
			if (dbg_pc.full())
				dbg_pc.pop_front();
			dbg_pc.push_back({ cpu.pc(), cpu.sp() });

			if (!cpu.tick())
			{
				g_going = false;
				break;
			}
			if (bus.error())
			{
				g_going = false;
				break;
			}		
		}
	}

	if (form)
	{
		form->destroy();
		form = nullptr;
	}

	for (int i = 0; i < dbg_pc.size(); ++i)
		log::info << i << L". PC " << str(L"%08x", dbg_pc[i].first) << L", SP " << str(L"%08x", dbg_pc[i].second) << Endl;
}
