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
#include <Core/Timer/Timer.h>
#include <Drawing/Image.h>
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
#include "Rv32/SD.h"
#include "Rv32/SystemRegisters.h"
#include "Rv32/Timer.h"
#include "Rv32/UART.h"
#include "Rv32/Unknown.h"
#include "Rv32/Video.h"

using namespace traktor;

// https://github.com/takahirox/riscv-rust/blob/master/src/cpu.rs
// https://mindchasers.com/dev/rv-getting-started

namespace
{

const wchar_t* c_registerNames[] =
{
	L"ZERO",
	L"RA",
	L"SP",
	L"GP",
	L"TP",
	L"T0",
	L"T1",
	L"T2",
	L"S0/FP",
	L"S1",
	L"A0",
	L"A1",
	L"A2",
	L"A3",
	L"A4",
	L"A5",
	L"A6",
	L"A7",
	L"S2",
	L"S3",
	L"S4",
	L"S5",
	L"S6",
	L"S7",
	L"S8",
	L"S9",
	L"S10",
	L"S11",
	L"T3",
	L"T4",
	L"T5",
	L"T6"
};

}

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

	Memory rom(0x00100000);
	Memory sdram(0x00800000);
	Video video;
	UART uart1;
	UART uart2;
	Unknown i2c(L"I2C", true);
	SD sd;
	Unknown dma(L"DMA", true);
	TimerD tmr;
	Unknown plic(L"PLIC", true);
	SystemRegisters sysreg;

	Bus bus;
	bus.map(0x00000000, 0x00100000, false, &rom);
	bus.map(0x20000000, 0x20800000, true, &sdram);
	bus.map(0x51000000, 0x51000100, false, &uart1);
	bus.map(0x52000000, 0x52000100, false, &uart2);
	bus.map(0x53000000, 0x53000100, false, &i2c);
	bus.map(0x54000000, 0x54000100, false, &sd);
	bus.map(0x55000000, 0x55000100, false, &tmr);
	bus.map(0x57000000, 0x57000100, false, &dma);
	bus.map(0x58000000, 0x58004000, false, &plic);
	bus.map(0x59000000, 0x59000100, false, &sysreg);
	bus.map(0x5a000000, 0x5b000000, false, &video);

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
	rom.setReadOnly(true);

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
	else
	{
		// Initialize stack at end of SDRAM, this is same as firmware does when launching applications.
		const uint32_t memoryAvail = sysreg.readU32(4 << 2);
		const uint32_t sp = 0x20000000 + memoryAvail - 0x10000;
		cpu.setSP(sp);
	}

	CircularVector< std::pair< uint32_t, uint32_t >, 32 > dbg_pc;
	uint32_t dbg_sp = cpu.sp();

	Ref< ui::Form > form;
	Ref< ui::Bitmap > uiImage;
	Ref< ui::Image > image;
	
	if (!headless)
	{
		form = new ui::Form();
		form->create(L"RV32", ui::dpi96(640), ui::dpi96(400), ui::Form::WsDefault, new ui::FloodLayout());

		uiImage = new ui::Bitmap(320, 200);
		
		image = new ui::Image();
		image->create(form, uiImage, ui::Image::WsScale);

		form->update();
		form->show();
	}

	Timer timer;
	while (g_going)
	{
		if (!headless)
		{
			if (!ui::Application::getInstance()->process())
				break;

			for (int32_t i = 0; i < 10000; ++i)
			{
				// if (dbg_pc.full())
				// 	dbg_pc.pop_front();
				// dbg_pc.push_back({ cpu.pc(), cpu.sp() });

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

			if (timer.getElapsedTime() > 1.0f/60.0f && video.getImage())
			{
				uiImage->copyImage(video.getImage());
				image->setImage(uiImage);
				timer.reset();
			}
		}
		else
		{
			// if (dbg_pc.full())
			// 	dbg_pc.pop_front();
			// dbg_pc.push_back({ cpu.pc(), cpu.sp() });

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

	video.getImage()->save(L"Rv32.png");

	if (form)
	{
		form->destroy();
		form = nullptr;
	}


	log::info << str(L"%-5S", L"PC") << L" : " << str(L"%08x", cpu.pc()) << Endl;
	log::info << L"---" << Endl;

	for (uint32_t i = 0; i < 32; ++i)
		log::info << str(L"%-5S", c_registerNames[i]) << L" : " << str(L"%08x", cpu.reg(i)) << Endl;

	// for (int i = 0; i < dbg_pc.size(); ++i)
	// 	log::info << i << L". PC " << str(L"%08x", dbg_pc[i].first) << L", SP " << str(L"%08x", dbg_pc[i].second) << Endl;

	return 0;
}
