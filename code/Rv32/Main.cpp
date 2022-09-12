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
#include "Rv32/Audio.h"
#include "Rv32/Bus.h"
#include "Rv32/CPU.h"
#include "Rv32/Helpers.h"
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

const uint32_t c_memoryAvail = 0x2000000;

// Modifiers
#define RT_MODIFIER_CTRL    0x01
#define RT_MODIFIER_R_CTRL  0x10
#define RT_MODIFIER_SHIFT   0x02
#define RT_MODIFIER_R_SHIFT 0x20
#define RT_MODIFIER_ALT     0x04
#define RT_MODIFIER_R_ALT   0x40

// Keycodes
#define RT_KEY_A	        0x04
#define RT_KEY_B	        0x05
#define RT_KEY_C	        0x06
#define RT_KEY_D	        0x07
#define RT_KEY_E	        0x08
#define RT_KEY_F	        0x09
#define RT_KEY_G	        0x0a
#define RT_KEY_H	        0x0b
#define RT_KEY_I	        0x0c
#define RT_KEY_J	        0x0d
#define RT_KEY_K	        0x0e
#define RT_KEY_L	        0x0f
#define RT_KEY_M	        0x10
#define RT_KEY_N	        0x11
#define RT_KEY_O	        0x12
#define RT_KEY_P	        0x13
#define RT_KEY_Q	        0x14
#define RT_KEY_R	        0x15
#define RT_KEY_S	        0x16
#define RT_KEY_T	        0x17
#define RT_KEY_U	        0x18
#define RT_KEY_V	        0x19
#define RT_KEY_W	        0x1a
#define RT_KEY_X	        0x1b
#define RT_KEY_Y	        0x1c
#define RT_KEY_Z	        0x1d
#define RT_KEY_SPACE	    0x2c
#define RT_KEY_RETURN	    0x28
#define RT_KEY_ENTER	    0x58
#define RT_KEY_BACKSPACE	0x2a
#define RT_KEY_1	        0x1e
#define RT_KEY_2	        0x1f
#define RT_KEY_3	        0x20
#define RT_KEY_4	        0x21
#define RT_KEY_5	        0x22
#define RT_KEY_6	        0x23
#define RT_KEY_7	        0x24
#define RT_KEY_8	        0x25
#define RT_KEY_9	        0x26
#define RT_KEY_0	        0x27
#define RT_KEY_ESCAPE	    0x29
#define RT_KEY_TAB	        0x2b
#define RT_KEY_TILDE	    0x35
#define RT_KEY_F1	        0x3a
#define RT_KEY_F2	        0x3b
#define RT_KEY_F3	        0x3c
#define RT_KEY_F4	        0x3d
#define RT_KEY_F5	        0x3e
#define RT_KEY_F6	        0x3f
#define RT_KEY_F7	        0x40
#define RT_KEY_F8	        0x41
#define RT_KEY_F9	        0x42
#define RT_KEY_F10	        0x43
#define RT_KEY_F11	        0x44
#define RT_KEY_F12	        0x45
#define RT_KEY_DELETE	    0x4c
#define RT_KEY_INSERT	    0x49
#define RT_KEY_END	        0x4d
#define RT_KEY_HOME	        0x4a
#define RT_KEY_PGUP	        0x4b
#define RT_KEY_PGDOWN	    0x4e
#define RT_KEY_PRTSCR	    0x46
#define RT_KEY_SCRLK	    0x47
#define RT_KEY_PAUSE	    0x48
#define RT_KEY_LEFT		    0x50
#define RT_KEY_RIGHT	    0x4f
#define RT_KEY_UP		    0x52
#define RT_KEY_DOWN		    0x51
#define RT_KEY_COMMA	    0x36
#define RT_KEY_DOT		    0x37
#define RT_KEY_MINUS	    0x38
#define RT_KEY_PLUS		    0x2d
#define RT_KEY_DASH		    0x2e

const struct {
	ui::VirtualKey vkey;
	uint8_t kc;
} c_keyMapping[] = {
	{ ui::VkA, RT_KEY_A },
	{ ui::VkB, RT_KEY_B },
	{ ui::VkC, RT_KEY_C },
	{ ui::VkD, RT_KEY_D },
	{ ui::VkE, RT_KEY_E },
	{ ui::VkF, RT_KEY_F },
	{ ui::VkG, RT_KEY_G },
	{ ui::VkH, RT_KEY_H },
	{ ui::VkI, RT_KEY_I },
	{ ui::VkJ, RT_KEY_J },
	{ ui::VkK, RT_KEY_K },
	{ ui::VkL, RT_KEY_L },
	{ ui::VkM, RT_KEY_M },
	{ ui::VkN, RT_KEY_N },
	{ ui::VkO, RT_KEY_O },
	{ ui::VkP, RT_KEY_P },
	{ ui::VkQ, RT_KEY_Q },
	{ ui::VkR, RT_KEY_R },
	{ ui::VkS, RT_KEY_S },
	{ ui::VkT, RT_KEY_T },
	{ ui::VkU, RT_KEY_U },
	{ ui::VkV, RT_KEY_V },
	{ ui::VkW, RT_KEY_W },
	{ ui::VkX, RT_KEY_X },
	{ ui::VkY, RT_KEY_Y },
	{ ui::VkZ, RT_KEY_Z },

	{ ui::Vk1, RT_KEY_1 },
	{ ui::Vk2, RT_KEY_2 },
	{ ui::Vk3, RT_KEY_3 },
	{ ui::Vk4, RT_KEY_4 },
	{ ui::Vk5, RT_KEY_5 },
	{ ui::Vk6, RT_KEY_6 },
	{ ui::Vk7, RT_KEY_7 },
	{ ui::Vk8, RT_KEY_8 },
	{ ui::Vk9, RT_KEY_9 },
	{ ui::Vk0, RT_KEY_0 },

	{ ui::VkEscape, RT_KEY_ESCAPE },
	{ ui::VkSpace, RT_KEY_SPACE },
	{ ui::VkReturn, RT_KEY_RETURN },
	// { ui::VkEnter, RT_KEY_ENTER },
	{ ui::VkBackSpace, RT_KEY_BACKSPACE },

	{ ui::VkLeft, RT_KEY_LEFT },
	{ ui::VkRight, RT_KEY_RIGHT },
	{ ui::VkUp, RT_KEY_UP },
	{ ui::VkDown, RT_KEY_DOWN },

	{ ui::VkPeriod, RT_KEY_DOT },
	{ ui::VkComma, RT_KEY_COMMA },
};

uint8_t virtualToKey(ui::VirtualKey vkey)
{
	for (int32_t i = 0; i < sizeof_array(c_keyMapping); ++i)
	{
		if (c_keyMapping[i].vkey == vkey)
			return c_keyMapping[i].kc;
	}
	return 0;
}

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
	Memory sdram(c_memoryAvail);
	Video video;
	UART uart1;
	UART uart2;
	Unknown i2c(L"I2C", true);
	SD sd;
	Unknown dma(L"DMA", true);
	TimerD tmr;
	Unknown plic(L"PLIC", true);
	SystemRegisters sysreg(c_memoryAvail);
	Audio audio;

	Bus bus;
	bus.map(0x00000000, 0x00100000, false, false, &rom);
	bus.map(0x20000000, 0x20000000 + c_memoryAvail, true, false, &sdram);
	bus.map(0x51000000, 0x51000100, false, false, &uart1);
	bus.map(0x52000000, 0x52000100, false, false, &uart2);
	bus.map(0x53000000, 0x53000100, false, false, &i2c);
	bus.map(0x54000000, 0x54000100, false, true, &sd);
	bus.map(0x55000000, 0x55000100, false, true, &tmr);
	bus.map(0x56000000, 0x56000100, false, true, &audio);
	bus.map(0x57000000, 0x57000100, false, false, &dma);
	bus.map(0x58000000, 0x58004000, false, false, &plic);
	bus.map(0x59000000, 0x59000100, false, false, &sysreg);
	bus.map(0x5a000000, 0x5b000000, false, false, &video);

	Ref< OutputStream > os = nullptr;	
	if (cmdLine.hasOption(L't', L"trace"))
	{
		Ref< IStream > f = FileSystem::getInstance().open(L"Rv32.trace", File::FmWrite);
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
		const uint32_t sp = 0x20000000 + c_memoryAvail;
		cpu.setSP(sp);
		log::info << L"Initial stack " << str(L"%08x", sp) << Endl;
	}

	// Push arguments.
	cpu.push(0);
	cpu.push(0);

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

		ui::Point mousePosition(0, 0);
		uint8_t mouseButton = 0;
		
		image = new ui::Image();
		image->create(form, uiImage, ui::Image::WsScale | ui::Image::WsNearestFilter);
		image->addEventHandler< ui::MouseButtonDownEvent >([&](ui::MouseButtonDownEvent* event){

			auto d = event->getPosition() - mousePosition;
			mousePosition = event->getPosition();

			if (event->getButton() == ui::MbtLeft)
				mouseButton |= 1;
			else
				mouseButton |= 2;

			uart2.enqueue('M');
			uart2.enqueue(d.cx);
			uart2.enqueue(d.cy);
			uart2.enqueue(0);
			uart2.enqueue(mouseButton);
			uart2.enqueue('E');
		});
		image->addEventHandler< ui::MouseButtonUpEvent >([&](ui::MouseButtonUpEvent* event){

			auto d = event->getPosition() - mousePosition;
			mousePosition = event->getPosition();

			if (event->getButton() == ui::MbtLeft)
				mouseButton &= ~1;
			else
				mouseButton &= ~2;

			uart2.enqueue('M');
			uart2.enqueue(d.cx);
			uart2.enqueue(d.cy);
			uart2.enqueue(0);
			uart2.enqueue(mouseButton);
			uart2.enqueue('E');
		});
		image->addEventHandler< ui::MouseMoveEvent >([&](ui::MouseMoveEvent* event){

			auto d = event->getPosition() - mousePosition;
			mousePosition = event->getPosition();

			uart2.enqueue('M');
			uart2.enqueue(d.cx);
			uart2.enqueue(d.cy);
			uart2.enqueue(0);
			uart2.enqueue(mouseButton);
			uart2.enqueue('E');			
		});

		image->addEventHandler< ui::KeyDownEvent >([&](ui::KeyDownEvent* event){
			const uint8_t kc = virtualToKey(event->getVirtualKey());
			if (kc != 0)
			{
				uart2.enqueue('K');
				uart2.enqueue(kc);
				uart2.enqueue(0);
				uart2.enqueue('E');
			}
		});
		image->addEventHandler< ui::KeyUpEvent >([&](ui::KeyUpEvent* event){
			if (event->isRepeat())
				return;
			const uint8_t kc = virtualToKey(event->getVirtualKey());
			if (kc != 0)
			{
				uart2.enqueue('k');
				uart2.enqueue(kc);
				uart2.enqueue(0);
				uart2.enqueue('E');
			}
		});

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
		log::info << str(L"%-5S", getRegisterName(i)) << L" : " << str(L"%08x", cpu.reg(i)) << Endl;

	// for (int i = 0; i < dbg_pc.size(); ++i)
	// 	log::info << i << L". PC " << str(L"%08x", dbg_pc[i].first) << L", SP " << str(L"%08x", dbg_pc[i].second) << Endl;

	return 0;
}
