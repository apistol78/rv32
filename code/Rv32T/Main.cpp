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
#include <Core/Io/FileOutputStream.h>
#include <Core/Io/StreamCopy.h>
#include <Core/Io/Utf8Encoding.h>
#include <Core/Log/Log.h>
#include <Core/Misc/CommandLine.h>
#include <Core/Misc/SafeDestroy.h>
#include <Core/Misc/String.h>
#include <Core/Timer/Timer.h>
#include <Ui/Application.h>
#include <Ui/Bitmap.h>
#include <Ui/Button.h>
#include <Ui/Image.h>
#include <Ui/Form.h>
#include <Ui/FloodLayout.h>
#include <Ui/TableLayout.h>
#include <Ui/StatusBar/StatusBar.h>
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
#include "Rv32T/Measure.h"
#include "Rv32T/SD.h"
#include "Rv32T/UART_TX.h"

// Verilated SoC
#include "verilated.h"
#include "verilated_fst_c.h"
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
	{
		soc->SoC__DOT__cpu__DOT__fetch__DOT__pc = start;
//		soc->SoC__DOT__cpu__DOT__fetch__DOT__next_pc = start;
	}

	return true;
}

int main(int argc, const char **argv)
{
	CommandLine cmdLine(argc, argv);
	Timer timer;

	if (!cmdLine.hasOption(L'h', L"headless"))
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
	bool reset = false;
	bool key1 = false;
	bool slow = false;

	if (cmdLine.hasOption(L"slow"))
		slow = true;

	// Create user interface.
	Ref< ui::Form > form;
	Ref< ui::StatusBar > statusBar;
	Ref< ui::Bitmap > framebuffer;
	Ref< ui::Image > image;

	if (!cmdLine.hasOption(L'h', L"headless"))
	{
		form = new ui::Form();
		form->create(L"RV32", ui::dpi96(640), ui::dpi96(400), ui::Form::WsDefault, new ui::TableLayout(L"100%", L"*,100%,*", 4, 4));

		Ref< ui::Container > container = new ui::Container();
		container->create(form, ui::WsNone, new ui::TableLayout(L"*,*,*,*", L"*", 0, 4));

		Ref< ui::Button > buttonTrace = new ui::Button();
		buttonTrace->create(container, L"Trace");
		buttonTrace->addEventHandler< ui::ButtonClickEvent >([&](ui::ButtonClickEvent* event) {
			buttonTrace->setEnable(false);
			trace = true;
		});

		Ref< ui::Button > buttonReset = new ui::Button();
		buttonReset->create(container, L"Reset");
		buttonReset->addEventHandler< ui::ButtonClickEvent >([&](ui::ButtonClickEvent* event) {
			reset = true;
		});	

		Ref< ui::Button > buttonKey1 = new ui::Button();
		buttonKey1->create(container, L"KEY1");
		buttonKey1->addEventHandler< ui::ButtonClickEvent >([&](ui::ButtonClickEvent* event) {
			key1 = true;
		});

		Ref< ui::Button > buttonSpeed = new ui::Button();
		buttonSpeed->create(container, L"Fast/Slow");
		buttonSpeed->addEventHandler< ui::ButtonClickEvent >([&](ui::ButtonClickEvent* event) {
			slow = !slow;
		});

		framebuffer = new ui::Bitmap(640, 480);
		
		Ref< ui::Container > containerImage = new ui::Container();
		containerImage->create(form, ui::WsNone, new ui::FloodLayout());

		image = new ui::Image();
		image->create(containerImage, framebuffer, ui::Image::WsScale);

		statusBar = new ui::StatusBar();
		statusBar->create(form);
		statusBar->addColumn(ui::dpi96(100));	// IPC
		statusBar->addColumn(ui::dpi96(200));	// BUS
		statusBar->addColumn(ui::dpi96(240));	// STALL
		statusBar->addColumn(ui::dpi96(200));	// PC

		form->update();
		form->show();
	}

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

	if (cmdLine.hasOption('s', L"stack"))
	{
		uint32_t sp = (uint32_t)cmdLine.getOption('s', L"stack").getInteger();
		soc->SoC__DOT__cpu__DOT__registers__DOT__r[2] = sp;
		log::info << L"Inital SP " << str(L"0x%08x", sp) << Endl;
	}

	// Create signal trace.
	VerilatedFstC* tfp = nullptr;
	if (cmdLine.hasOption(L't', L"trace"))
		trace = true;

	Ref< FileOutputStream > fos;
	if (cmdLine.hasOption(L"trace-pc"))
	{
		Ref< IStream > f = FileSystem::getInstance().open(L"Rv32T.pc", File::FmWrite);
		if (!f)
		{
			log::error << L"Unable to create PC trace." << Endl;
			return 1;
		}
		fos = new FileOutputStream(f, new Utf8Encoding());
	}

	Ref< FileOutputStream > bus_os;
	if (cmdLine.hasOption(L"trace-bus"))
	{
		Ref< IStream > f = FileSystem::getInstance().open(L"Rv32T.bus", File::FmWrite);
		if (!f)
		{
			log::error << L"Unable to create BUS trace." << Endl;
			return 1;
		}
		bus_os = new FileOutputStream(f, new Utf8Encoding());
	}

	// Create from PC.
	uint32_t traceFromPC = -1;
	if (cmdLine.hasOption(L"trace-from"))
		traceFromPC = cmdLine.getOption(L"trace-from").getInteger();

	HDMI hdmi;
	LEDR ledr;
	UART_TX uart_tx;
	SD sd;

	int32_t time = 0;
	uint32_t lastCycles = 0;
	uint32_t lastRetired = 0;
	uint32_t lastStarve = 0;
	Measure busActive;
	Measure busCPUandDMA;
	Measure busSDRAM;
	Measure stallExecute;
	Measure stallMemory;

	uint32_t lastTracePC = -1;
	int32_t Tp = 0;

	timer.reset();
	while (g_going)
	{
		if (form)
		{
			if (!ui::Application::getInstance()->process())
				break;
		}

		int32_t Tc = (int32_t)(timer.getElapsedTime() * 1000);
		uint32_t count = slow ? std::min< int32_t >(Tc - Tp, 1000) : 50000;
		Tp = Tc;
		
		for (int32_t i = 0; i < count; ++i)
		{
			// if (soc->SoC__DOT__cpu__DOT__fetch__DOT__pc == traceFromPC)
			// {
			// 	log::info << L"TRACE FROM BEGUN" << Endl;
			// 	trace = true;
			// 	traceFromPC = -1;
			// }

			if (trace)
			{
				Verilated::traceEverOn(true);
				tfp = new VerilatedFstC;
				soc->trace(tfp, 99);  // Trace 99 levels of hierarchy
				tfp->open("Rv32T.fst");
				trace = false;
				log::info << L"TRACE CREATED" << Endl;
			}

			++time;
			soc->CLOCK_125_p = 0;
			soc->KEY |= key1 ? 2 : 0;
			soc->CPU_RESET_n = reset ? 0 : 1;

			soc->eval();

			if (tfp)
				tfp->dump(time);

			++time;
			soc->CLOCK_125_p = 1;
			soc->KEY |= key1 ? 2 : 0;
			soc->CPU_RESET_n = reset ? 0 : 1;

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

			// Count number of cycles bus is active.
			if (soc->SoC__DOT__cpu_ibus_request /* || soc->SoC__DOT__cpu_dbus_request*/)
				busActive++;
			// if (soc->SoC__DOT__cpu_dbus_request && soc->SoC__DOT__dma_bus_request)
			// 	busCPUandDMA++;
			if (soc->SoC__DOT__l2cache_bus_request)
				busSDRAM++;

			if (soc->SoC__DOT__cpu__DOT__execute_busy)
				stallExecute++;
			if (soc->SoC__DOT__cpu__DOT__memory__DOT__busy)
				stallMemory++;

			if (fos)
			{
				if (soc->SoC__DOT__cpu__DOT__fetch__DOT__pc != lastTracePC)
				{
					*fos << str(L"%8d", time/2) << L", " << str(L"%08x", soc->SoC__DOT__cpu__DOT__fetch__DOT__pc);

					for (uint32_t i = 0; i < 32; ++i)
						*fos << str(L", %08x", soc->SoC__DOT__cpu__DOT__registers__DOT__r[i]);
					
					*fos << Endl;
					lastTracePC = soc->SoC__DOT__cpu__DOT__fetch__DOT__pc;
				}
			}

			// if (bus_os)
			// {
			// 	if (soc->SoC__DOT__cpu_dbus_request && soc->SoC__DOT__cpu_dbus_ready)
			// 	{
			// 		*bus_os << str(L"%8d", time/2) << L", " << str(L"%08x", soc->SoC__DOT__cpu__DOT__fetch__DOT__pc) << L", ";
			// 		*bus_os << (soc->SoC__DOT__cpu_dbus_rw ? L"W" : L"R") << L", ";
			// 		*bus_os << str(L"%08x", soc->SoC__DOT__cpu_dbus_address) << L", ";
			// 		if (soc->SoC__DOT__cpu_dbus_rw)
			// 			*bus_os << str(L"%08x", soc->SoC__DOT__cpu_dbus_wdata);
			// 		else
			// 			*bus_os << str(L"%08x", soc->SoC__DOT__cpu_dbus_rdata);
			// 		*bus_os << Endl;
			// 	}
			// }

			key1 = false;
			reset = false;

			// if (time/2 >= (957224-1000) && !tfp)
			// {
			// 	trace = true;
			// 	slow = true;
			// 	log::info << L"TRACING!!" << Endl;
			// }

			if (soc->SoC__DOT__cpu__DOT__decode_fault)
			{
				log::warning << L"DECODE fault, terminating." << Endl;
				g_going = false;
				break;
			}
			if (soc->SoC__DOT__cpu__DOT__execute_fault)
			{
				log::warning << L"EXECUTE fault, terminating." << Endl;
				g_going = false;
				break;
			}
			if (soc->SoC__DOT__bus_fault)
			{
				log::warning << L"BUS fault, invalid address " << str(L"0x%08x", soc->SoC__DOT__bus_address) << L", terminating." << Endl;
				g_going = false;
				break;				
			}
		}

		if (form)
		{
			if (hdmi.shouldDraw())
			{
				framebuffer->copyImage(hdmi.getImage());
				image->setImage(framebuffer);
			}

			if ((Tc % 30) == 0)
			{
				uint32_t dc = soc->SoC__DOT__timer__DOT__cycles - lastCycles;
				uint32_t dr = soc->SoC__DOT__cpu__DOT__writeback__DOT__retired - lastRetired;

				statusBar->setText(0, str(L"%.2f IPC", ((double)dr) / dc));
				
				statusBar->setText(1,
					str(
						L"%.2f%% (%.2f%%) BUS",
						((double)busActive.delta() * 100.0) / dc,
						((double)busSDRAM.delta() * 100.0) / dc
					)
				);

				//statusBar->setText(2, str(L"%.2f%% BUS & DMA", ((double)busCPUandDMA.delta() * 100.0) / dc));

				statusBar->setText(2, 
					str(
						L"%.2f%% / %.2f%% STALL X/M",
						((double)stallExecute.delta() * 100.0) / dc,
						((double)stallMemory.delta() * 100.0) / dc
					)
				);

				statusBar->setText(3, str(L"%08x PC", soc->SoC__DOT__cpu__DOT__fetch__DOT__pc));

				lastCycles = soc->SoC__DOT__timer__DOT__cycles;
				lastRetired = soc->SoC__DOT__cpu__DOT__writeback__DOT__retired;

				busActive.snapshot();
				busCPUandDMA.snapshot();
				busSDRAM.snapshot();
				stallExecute.snapshot();
				stallMemory.snapshot();					
			}
		}
		else
		{
			if ((Tc % 600) == 0)
			{
				uint32_t dc = soc->SoC__DOT__timer__DOT__cycles - lastCycles;
				uint32_t dr = soc->SoC__DOT__cpu__DOT__writeback__DOT__retired - lastRetired;
				uint32_t ds = soc->SoC__DOT__cpu__DOT__fetch__DOT__starve - lastStarve;

				// double l2 = ((double)soc->SoC__DOT__l2cache__DOT__hit * 100.0) / (soc->SoC__DOT__l2cache__DOT__hit + soc->SoC__DOT__l2cache__DOT__miss);

				log::info << L"### " <<
					str(L"%.2f IPC", ((double)dr) / dc) << L", " <<
					str(L"%.2f%% BUS", ((double)busActive.delta() * 100.0) / dc) << L", " <<
					str(L"%.2f%% STALL X", ((double)stallExecute.delta() * 100.0) / dc) << L", " <<
					str(L"%.2f%% STALL M", ((double)stallMemory.delta() * 100.0) / dc) << L", " <<
					// str(L"%.2f%% L2", l2) << L", " <<
					// str(L"%d", soc->SoC__DOT__l2cache__DOT__hit) << L", " <<
					// str(L"%d", soc->SoC__DOT__l2cache__DOT__miss) << L", " <<
					str(L"%.2f%% STARVE", ((double)ds * 100.0) / dc) << L", " <<
					Endl;

				lastCycles = soc->SoC__DOT__timer__DOT__cycles;
				lastRetired = soc->SoC__DOT__cpu__DOT__writeback__DOT__retired;
				lastStarve = soc->SoC__DOT__cpu__DOT__fetch__DOT__starve;
				
				busActive.snapshot();
				busCPUandDMA.snapshot();
				stallExecute.snapshot();
				stallMemory.snapshot();
			}	
		}
	}

	log::info << Endl;
	log::info << L"--- Terminated ---" << Endl;
	log::info << L"PC     : " << str(L"%08x", soc->SoC__DOT__cpu__DOT__fetch__DOT__pc) << Endl;
	log::info << L"CYCLES : " << soc->SoC__DOT__timer__DOT__cycles << Endl;
	log::info << L"RETIRE : " << soc->SoC__DOT__cpu__DOT__writeback__DOT__retired << Endl;
	log::info << L"BUS    : " << busActive.value() << L", " << (busActive.value() * 100) / soc->SoC__DOT__timer__DOT__cycles << L"%" << Endl;
	log::info << L"MS     : " << soc->SoC__DOT__timer__DOT__ms << Endl;

	hdmi.getImage()->save(L"Rv32T.png");

	if (tfp)
	{
		tfp->close();
		delete tfp;
	}

	safeDestroy(form);
	return 0;
}