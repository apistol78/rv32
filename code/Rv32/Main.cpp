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
#include <Core/Io/StringReader.h>
#include <Core/Io/AnsiEncoding.h>
#include <Core/Io/Utf8Encoding.h>
#include <Core/Log/Log.h>
#include <Core/Misc/CommandLine.h>
#include <Core/Misc/String.h>
#include <Ui/Application.h>
#include <Ui/Bitmap.h>
#include <Ui/Image.h>
#include <Ui/Form.h>
#include <Ui/TableLayout.h>
#if defined(_WIN32)
#	include <Ui/Win32/WidgetFactoryWin32.h>
#elif defined(__APPLE__)
#	include <Ui/Cocoa/WidgetFactoryCocoa.h>
#elif defined(__LINUX__) || defined(__RPI__)
#	include <Ui/X11/WidgetFactoryX11.h>
#endif
#include "Rv32/Bus.h"
#include "Rv32/CPU.h"
#include "Rv32/Memory.h"
#include "Rv32/UART.h"
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

	bool headless = true;

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

	Memory rom(0x00010000);
	Memory ram(0x00010000);
	Memory sram(0x00010000);
	Memory sdram(0x00010000);
	Video video;
	UART uart;

	Bus bus;
	bus.map(0x00000000, 0x00010000, &rom);
	bus.map(0x00010000, 0x00012000, &ram);
	bus.map(0x10000000, 0x20000000, &sram);
	bus.map(0x20000000, 0x40000000, &sdram);
	bus.map(0x40000000, 0x50000000, &video);
	bus.map(0x50000000, 0x50000010, &video);
	bus.map(0x50000010, 0x50000020, &uart);

	if (cmdLine.hasOption(L"image-file") && cmdLine.hasOption(L"image-base"))
	{
		std::wstring fileName = cmdLine.getOption(L"image-file").getString();
		int32_t offset = cmdLine.getOption(L"image-base").getInteger();

		Ref< IStream > f = FileSystem::getInstance().open(fileName, File::FmRead);
		if (!f)
		{
			log::error << L"Unable to open image \"" << fileName << L"\"." << Endl;
			return 1;
		}

		uint32_t start = 0xffffffff;
		uint32_t end = 0x00000000;

		uint8_t data[1024];
		for (;;)
		{
			int64_t r = f->read(data, sizeof(data));
			if (r <= 0)
				break;

			for (int64_t i = 0; i < (int32_t)r; ++i)
			{
				start = std::min< uint32_t >(start, offset);
				end = std::max< uint32_t >(end, offset);

				bus.writeU8(offset++, data[i]);
				if (bus.error())
					return 1;
			}
		}

		if (start <= end)
			log::info << L"Image loaded into " << str(L"0x%08x", start) << L" - " << str(L"0x%08x", end) << L"." << Endl;
	}

	if (cmdLine.hasOption(L'h', L"hex"))
	{
		std::wstring fileName = cmdLine.getOption(L'h', L"hex").getString();
		std::wstring tmp;

		Ref< IStream > f = FileSystem::getInstance().open(fileName, File::FmRead);
		if (!f)
		{
			log::error << L"Unable to open hex \"" << fileName << L"\"." << Endl;
			return 1;
		}

		uint32_t segment = 0x00000000;
		uint32_t upper = 0x00000000;

		uint32_t start = 0xffffffff;
		uint32_t end = 0x00000000;

		StringReader sr(f, new AnsiEncoding());
		while (sr.readLine(tmp) >= 0)
		{
			if (tmp.empty())
				continue;

			// Ensure start byte is correct.
			if (tmp[0] != L':')
				continue;
			tmp = tmp.substr(1);

			// Parse header.
			int32_t ln = parseString< int32_t >(L"0x" + tmp.substr(0, 2));
			int32_t addr = parseString< int32_t >(L"0x" + tmp.substr(2, 4));
			int32_t type = parseString< int32_t >(L"0x" + tmp.substr(6, 2));

			if (type == 0x00)
			{
				for (int32_t i = 8; i < 8 + ln * 2; i += 2)
				{
					int32_t v = parseString< int32_t >(L"0x" + tmp.substr(i, 2));
					bus.writeU8((upper | addr) + segment, (uint8_t)v);
					start = std::min(start, (upper | addr) + segment);
					end = std::max(end, (upper | addr) + segment);
					addr++;
				}
			}
			else if (type == 0x01)
				break;
			else if (type == 0x02)
			{
				segment = parseString< int32_t >(L"0x" + tmp.substr(8, 4));
				segment *= 16;
			}
			else if (type == 0x03)
				continue;
			else if (type == 0x04)
			{
				upper = parseString< int32_t >(L"0x" + tmp.substr(8, 4));
				upper <<= 16;
			}
			else
				log::warning << L"Unhandled HEX record type " << type << L"." << Endl;
		}

		if (start <= end)
			log::info << L"HEX loaded into " << str(L"0x%08x", start) << L" - " << str(L"0x%08x", end) << L"." << Endl;
	}

	// Lock ROM from being mutable.
	rom.setReadOnly(true);

	Ref< OutputStream > os = nullptr;	
	if (cmdLine.hasOption(L't', L"trace"))
	{
		Ref< IStream > f = FileSystem::getInstance().open(L"Trace.log", File::FmWrite);
		if (f)
			os = new FileOutputStream(f, new Utf8Encoding());
	}

	CPU cpu(&bus, os);

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

	CircularVector< std::pair< uint32_t, uint32_t >, 16 > dbg_pc;
	uint32_t dbg_sp = cpu.sp();

	Ref< ui::Form > form;
	Ref< ui::Bitmap > uiImage;
	Ref< ui::Image > image;
	
	if (!headless)
	{
		form = new ui::Form();
		form->create(L"RV32", ui::dpi96(640), ui::dpi96(400), ui::Form::WsDefault, new ui::TableLayout(L"100%", L"100%", 0, 0));

		uiImage = new ui::Bitmap(640, 400);
		
		image = new ui::Image();
		image->create(form, uiImage, ui::Image::WsScale);

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
					break;
				if (bus.error())
					break;
			}

			if (video.shouldPresent() && video.getImage())
			{
				uiImage->copyImage(video.getImage());
				image->setImage(uiImage);
			}
		}
		else
		{
			if (dbg_pc.full())
				dbg_pc.pop_front();
			dbg_pc.push_back({ cpu.pc(), cpu.sp() });

			if (!cpu.tick())
				break;
			if (bus.error())
				break;			
		}
	}

	if (form)
	{
		form->destroy();
		form = nullptr;
	}

	for (int i = 0; i < dbg_pc.size(); ++i)
	{
		log::info << i << L". PC " << str(L"%08x", dbg_pc[i].first) << L", SP " << str(L"%08x", dbg_pc[i].second) << Endl;
	}

	// for (uint32_t sp = cpu.sp(); sp <= dbg_sp; sp += 4)
	// 	log::info << str(L"%08x", sp) << L" : " << str(L"%08x", bus.readU32(sp)) << Endl;
}
