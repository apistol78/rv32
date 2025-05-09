/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
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
#include <Core/Misc/TString.h>
#include <Core/Timer/Timer.h>
#include <Net/Network.h>
#include <Net/SocketAddressIPv4.h>
#include <Net/SocketStream.h>
#include <Net/UdpSocket.h>
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
#include "Rv32T/AUDIO.h"
#include "Rv32T/ELF.h"
#include "Rv32T/HDMI.h"
#include "Rv32T/LEDR.h"
#include "Rv32T/Measure.h"
#include "Rv32T/ProfilerBUS.h"
#include "Rv32T/SD.h"
#include "Rv32T/TraceEXE.h"
#include "Rv32T/TracePC.h"
#include "Rv32T/TraceWB.h"
#include "Rv32T/UART_RX.h"
#include "Rv32T/UART_TX.h"

// Verilated SoC
#include "verilated.h"
#include "verilated_fst_c.h"
#include "SoC/VSoC.h"
#include "SoC/VSoC___024root.h"

using namespace traktor;

const uint32_t c_memoryAvail = 0x2000000;
bool g_going = true;

double sc_time_stamp() { return 0.0; }

class CustomLogTarget : public ILogTarget
{
public:
	virtual void log(uint32_t threadId, int32_t level, const wchar_t* str) override final
	{
		VL_PRINTF_MT("%s\n", wstombs(str).c_str());
	}	
};

#if defined(__LINUX__) || defined(__RPI__) || defined(__APPLE__)
void abortHandler(int s)
{
	g_going = false;
}
#endif

bool loadELF(VSoC* soc, const std::wstring& fileName, bool jumpTo)
{
	AlignedVector< uint8_t > elf;
	uint32_t start = -1;
	auto root = soc->rootp;

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

				uint8_t* dst = 0; // (uint8_t*)root->SoC__DOT__sdram__DOT__data;
				for (uint32_t j = 0; j < shdr[i].sh_size; ++j)
					dst[addr - base + j] = pbits[j];
			}
		}
		else if (shdr[i].sh_type == 0x02 && jumpTo)	// SHT_SYMTAB
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
		root->SoC__DOT__cpu__DOT__fetch__DOT__pc = start;

	return true;
}

int main(int argc, const char **argv)
{
	CommandLine cmdLine(argc, argv);
	Timer timer;

	net::Network::initialize();

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

	Ref< CustomLogTarget > logTarget = new CustomLogTarget();
	log::info.setGlobalTarget(logTarget);
	log::warning.setGlobalTarget(logTarget);
	log::error.setGlobalTarget(logTarget);

	bool trace = false;
	bool reset = false;
	bool key1 = false;
	bool slow = false;
	bool mute = false;

	if (cmdLine.hasOption(L"slow"))
		slow = true;

	if (cmdLine.hasOption(L"mute"))
		mute = true;

	// Create user interface.
	Ref< ui::Form > form;
	Ref< ui::StatusBar > statusBar;
	Ref< ui::Bitmap > framebuffer;
	Ref< ui::Image > image;

	if (!cmdLine.hasOption(L'h', L"headless"))
	{
		form = new ui::Form();
		form->create(L"RV32", 640_ut, 400_ut, ui::Form::WsDefault, new ui::TableLayout(L"100%", L"*,100%,*", 4_ut, 4_ut));

		Ref< ui::Container > container = new ui::Container();
		container->create(form, ui::WsNone, new ui::TableLayout(L"*,*,*,*", L"*", 0_ut, 4_ut));

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

		framebuffer = new ui::Bitmap(640, 400);
		
		Ref< ui::Container > containerImage = new ui::Container();
		containerImage->create(form, ui::WsNone, new ui::FloodLayout());

		image = new ui::Image();
		image->create(containerImage, framebuffer, ui::Image::WsScale | ui::Image::WsNearestFilter);

		statusBar = new ui::StatusBar();
		statusBar->create(form);
		statusBar->addColumn(100);	// IPC
		statusBar->addColumn(200);	// BUS
		statusBar->addColumn(240);	// STALL
		statusBar->addColumn(200);	// PC
		statusBar->addColumn(200);	// FRAME COUNT

		form->update();
		form->show();
	}

	// Create SoC simulation.
	VSoC* soc = new VSoC();
	soc->rootp->CPU_RESET_n = 1;
	soc->rootp->CLOCK_125_p = 0;

	soc->eval();

	// VL_PRINTF_MT("*");

	// Randomize cache data.
	// for (int i = 0; i < sizeof_array(soc->SoC__DOT__cpu__DOT__fetch__DOT__genblk2__DOT__icache__DOT__cache__DOT__data); ++i)
	// 	soc->SoC__DOT__cpu__DOT__fetch__DOT__genblk2__DOT__icache__DOT__cache__DOT__data[i] = -1;
	// for (int i = 0; i < sizeof_array(soc->SoC__DOT__cpu__DOT__memory__DOT__genblk2__DOT__dcache__DOT__cache__DOT__data); ++i)
	// 	soc->SoC__DOT__cpu__DOT__memory__DOT__genblk2__DOT__dcache__DOT__cache__DOT__data[i] = -1;

	if (cmdLine.hasOption(L"elf-kernal"))
	{
		std::wstring fileName = cmdLine.getOption(L"elf-kernal").getString();
		if (!loadELF(soc, fileName, false))
			return 1;
	}	

	if (cmdLine.hasOption(L'e', L"elf"))
	{
		std::wstring fileName = cmdLine.getOption(L'e', L"elf").getString();
		if (!loadELF(soc, fileName, true))
			return 1;
	}

	if (cmdLine.hasOption('s', L"stack"))
	{
		const uint32_t sp = (uint32_t)cmdLine.getOption('s', L"stack").getInteger();
		soc->rootp->SoC__DOT__cpu__DOT__registers__DOT__r[2] = sp;
		log::info << L"Inital SP " << str(L"0x%08x", sp) << Endl;
	}
	else
	{
		// Initialize stack at end of SDRAM, this is same as firmware does when launching applications.
		const uint32_t sp = 0x20000000 + c_memoryAvail - 0x8;		
		soc->rootp->SoC__DOT__cpu__DOT__registers__DOT__r[2] = sp;
		log::info << L"Inital SP " << str(L"0x%08x", sp) << Endl;
	}

	// Create signal trace.
	VerilatedFstC* tfp = nullptr;
	if (cmdLine.hasOption(L't', L"trace"))
		trace = true;

	// Create devices.
	HDMI hdmi;
	AUDIO audio;

	RefArray< Device > devices;
	devices.push_back(&hdmi);
	devices.push_back(new LEDR());
	devices.push_back(new UART_TX(soc->rootp->UART_TX));
	devices.push_back(new UART_TX(soc->rootp->UART_1_TX));
	devices.push_back(new SD());

	// Create receiving UART.
	if (cmdLine.hasOption(L"udp"))
	{
		Ref< net::UdpSocket > socket = new net::UdpSocket();
		if (!socket->bind(net::SocketAddressIPv4(45123)))
		{
			log::error << L"Unable to bind socket to port." << Endl;
			return 1;
		}

		Ref< IStream > target = new net::SocketStream(socket, true, true, 1000);
		devices.push_back(new UART_RX(target, soc->rootp->UART_RX));
	}

	// Add audio device to record PWM audio.
	if (cmdLine.hasOption(L"record-audio"))
		devices.push_back(&audio);

	// Create trace devices.
	if (cmdLine.hasOption(L"trace-pc"))
	{
		Ref< IStream > f = FileSystem::getInstance().open(cmdLine.getOption(L"trace-pc").getString(), File::FmWrite);
		if (!f)
		{
			log::error << L"Unable to create PC trace." << Endl;
			return 1;
		}
		devices.push_back(new TracePC(
			new FileOutputStream(f, new Utf8Encoding())
		));
	}

	if (cmdLine.hasOption(L"trace-execute"))
	{
		Ref< IStream > f = FileSystem::getInstance().open(cmdLine.getOption(L"trace-execute").getString(), File::FmWrite);
		if (!f)
		{
			log::error << L"Unable to create EXECUTE trace." << Endl;
			return 1;
		}
		devices.push_back(new TraceEXE(
			new FileOutputStream(f, new Utf8Encoding())
		));
	}

	if (cmdLine.hasOption(L"trace-writeback"))
	{
		Ref< IStream > f = FileSystem::getInstance().open(cmdLine.getOption(L"trace-writeback").getString(), File::FmWrite);
		if (!f)
		{
			log::error << L"Unable to create WRITEBACK trace." << Endl;
			return 1;
		}
		devices.push_back(new TraceWB(
			new FileOutputStream(f, new Utf8Encoding())
		));
	}

	devices.push_back(new ProfilerBUS());

	uint64_t time = 0;
	uint64_t lastCycles = 0;
	uint64_t lastRetired = 0;
	uint64_t lastStarve = 0;
	Measure busActive;
	Measure busSDRAM;
	Measure stallExecute;
	Measure stallMemory;
	int32_t Tp = 0;

	timer.reset();
	while (g_going)
	{
		if (form)
		{
			if (!ui::Application::getInstance()->process())
				break;
		}

		const uint64_t Tc = (uint64_t)(timer.getElapsedTime() * 1000);
		const uint64_t count = slow ? std::min< uint64_t >(Tc - Tp, 1000) : 50000;
		Tp = Tc;

		bool shouldDraw = false;
		
		for (uint64_t i = 0; i < count; ++i)
		{
			if (trace)
			{
				Verilated::traceEverOn(true);
				tfp = new VerilatedFstC;
				soc->trace(tfp, 99);  // Trace 99 levels of hierarchy
				tfp->open("Rv32T.fst");
				trace = false;
				log::info << L"TRACE CREATED" << Endl;
			}

			const bool hdmiTxInt = hdmi.shouldDraw();
			shouldDraw |= hdmiTxInt;

			++time;
			soc->rootp->CLOCK_125_p = 0;
			soc->rootp->CPU_RESET_n = reset ? 0 : 1;
			soc->rootp->KEY |= key1 ? 2 : 0;
			soc->rootp->HDMI_TX_INT = hdmiTxInt;

			soc->eval();

			if (tfp)
				tfp->dump(time);

			++time;
			soc->rootp->CLOCK_125_p = 1;
			soc->rootp->CPU_RESET_n = reset ? 0 : 1;
			soc->rootp->KEY |= key1 ? 2 : 0;
			soc->rootp->HDMI_TX_INT = hdmiTxInt;

			soc->eval();

			if (tfp)
				tfp->dump(time);

			for (auto device : devices)
				device->eval(soc, time);

			// Count number of cycles bus is active.
			// if (soc->SoC__DOT__cpu_ibus_request || soc->SoC__DOT__cpu_dbus_request)
			// 	busActive++;
			// if (soc->SoC__DOT__sdram_request)
			//  	busSDRAM++;

			if (soc->rootp->o_execute_busy)
				stallExecute++;
			if (soc->rootp->o_memory_busy)
				stallMemory++;

			key1 = false;
			reset = false;

			if (soc->rootp->SoC__DOT__cpu__DOT__decode_fault)
			{
				log::warning << L"DECODE fault, terminating." << Endl;
				g_going = false;
				break;
			}
			if (soc->rootp->SoC__DOT__cpu__DOT__execute_fault)
			{
				log::warning << L"EXECUTE fault, terminating." << Endl;
				g_going = false;
				break;
			}
			if (soc->rootp->o_debug_bus_fault)
			{
				const wchar_t* faults[] = { L"NA", L"IBUS", L"DBUS", L"DMA" };
				log::warning << L"BUS fault, invalid address " << str(L"0x%08x", soc->rootp->o_debug_bus_fault_address) << L" (" << faults[soc->rootp->o_debug_bus_fault_type] << L"), terminating." << Endl;
				g_going = false;
				break;				
			}
		}

		if (form)
		{
			if (shouldDraw)
			{
				framebuffer->copyImage(hdmi.getImage());
				image->setImage(framebuffer);
			}

			if ((Tc % 30) == 0)
			{
				const uint32_t dc = soc->rootp->SoC__DOT__timer__DOT__cycles - lastCycles;
				const uint32_t dr = soc->rootp->SoC__DOT__cpu__DOT__writeback__DOT__retired - lastRetired;

				statusBar->setText(0, str(L"%.2f IPC", ((double)dr) / dc));
				
				statusBar->setText(1,
					str(
						L"%.2f%% (%.2f%%) BUS",
						((double)busActive.delta() * 100.0) / dc,
						((double)busSDRAM.delta() * 100.0) / dc
					)
				);

				statusBar->setText(2, 
					str(
						L"%.2f%% / %.2f%% STALL X/M",
						((double)stallExecute.delta() * 100.0) / dc,
						((double)stallMemory.delta() * 100.0) / dc
					)
				);

				statusBar->setText(
					3,
					str(
						L"%08x PC",
						soc->rootp->SoC__DOT__cpu__DOT__fetch__DOT__pc
					)
				);

				statusBar->setText(
					4,
					str(
						L"%d FRAMES",
						hdmi.frameCount()
					)
				);

				lastCycles = soc->rootp->SoC__DOT__timer__DOT__cycles;
				lastRetired = soc->rootp->SoC__DOT__cpu__DOT__writeback__DOT__retired;

				busActive.snapshot();
				busSDRAM.snapshot();
				stallExecute.snapshot();
				stallMemory.snapshot();					
			}
		}
		else if (!mute)
		{
			if ((Tc % 600) == 0)
			{
				const uint32_t dc = soc->rootp->SoC__DOT__timer__DOT__cycles - lastCycles;
				const uint32_t dr = soc->rootp->SoC__DOT__cpu__DOT__writeback__DOT__retired - lastRetired;
				const uint32_t ds = soc->rootp->SoC__DOT__cpu__DOT__fetch__DOT__starve - lastStarve;

				const uint32_t ich = soc->rootp->o_icache_hit;
				const uint32_t icm = soc->rootp->o_icache_miss;
				const double icr = (ich * 100.0) / (ich + icm);

				const uint32_t dch = soc->rootp->o_dcache_hit;
				const uint32_t dcm = soc->rootp->o_dcache_miss;
				const double dcr = (dch * 100.0) / (dch + dcm);

				log::info << L"### " <<
					str(L"%08x PC", soc->rootp->SoC__DOT__cpu__DOT__fetch__DOT__pc) << L", " <<
					str(L"%.2f IPC", ((double)dr) / dc) << L", " <<
					str(L"%.2f%% BUS", ((double)busActive.delta() * 100.0) / dc) << L", " <<
					str(L"%.2f%% STALL X", ((double)stallExecute.delta() * 100.0) / dc) << L", " <<
					str(L"%.2f%% STALL M", ((double)stallMemory.delta() * 100.0) / dc) << L", " <<
					str(L"%.2f%% STARVE", ((double)ds * 100.0) / dc) << L", " <<
					str(L"%d MPIE", soc->rootp->SoC__DOT__cpu__DOT__csr__DOT__mstatus_mpie ? 1 : 0) << L", " <<
					str(L"%d MIE", soc->rootp->SoC__DOT__cpu__DOT__csr__DOT__mstatus_mie ? 1 : 0) << L", " <<
					str(L"%d MEIE", soc->rootp->SoC__DOT__cpu__DOT__csr__DOT__mie_meie ? 1 : 0) << L", " <<
					str(L"%d MEIP", soc->rootp->SoC__DOT__cpu__DOT__csr__DOT__mip_meip ? 1 : 0) << L", " <<
					str(L"%d MTIE", soc->rootp->SoC__DOT__cpu__DOT__csr__DOT__mie_mtie ? 1 : 0) << L", " <<
					str(L"%d MTIP", soc->rootp->SoC__DOT__cpu__DOT__csr__DOT__mip_mtip ? 1 : 0) << L", " <<
					str(L"%08x MSCRATCH", soc->rootp->SoC__DOT__cpu__DOT__csr__DOT__mscratch) << L", " <<
					str(L"%d TIME", soc->rootp->SoC__DOT__cpu__DOT__csr__DOT__wtime) << L", " <<
					str(L"%.2f%% I$ HIT", icr) << L", " <<
					str(L"%.2f%% D$ HIT", dcr) << L", " <<
					Endl;

				lastCycles = soc->rootp->SoC__DOT__timer__DOT__cycles;
				lastRetired = soc->rootp->SoC__DOT__cpu__DOT__writeback__DOT__retired;
				lastStarve = soc->rootp->SoC__DOT__cpu__DOT__fetch__DOT__starve;
				
				busActive.snapshot();
				stallExecute.snapshot();
				stallMemory.snapshot();
			}	
		}
	}

	log::info << Endl;
	log::info << L"--- Terminated ---" << Endl;
	log::info << L"PC        : " << str(L"%08x", soc->rootp->SoC__DOT__cpu__DOT__fetch__DOT__pc) << Endl;
	log::info << L"CYCLES    : " << soc->rootp->SoC__DOT__timer__DOT__cycles << Endl;
	log::info << L"RETIRE    : " << soc->rootp->SoC__DOT__cpu__DOT__writeback__DOT__retired << Endl;
	log::info << L"BUS       : " << busActive.value() << L", " << (busActive.value() * 100) / soc->rootp->SoC__DOT__timer__DOT__cycles << L"%" << Endl;

	log::info << Endl;
	log::info << L"RA        : " << str(L"%08x", soc->rootp->SoC__DOT__cpu__DOT__registers__DOT__r[1]) << Endl;
	log::info << L"SP        : " << str(L"%08x", soc->rootp->SoC__DOT__cpu__DOT__registers__DOT__r[2]) << Endl;
	log::info << L"GP        : " << str(L"%08x", soc->rootp->SoC__DOT__cpu__DOT__registers__DOT__r[3]) << Endl;
	log::info << L"TP        : " << str(L"%08x", soc->rootp->SoC__DOT__cpu__DOT__registers__DOT__r[4]) << Endl;
	log::info << L"T0        : " << str(L"%08x", soc->rootp->SoC__DOT__cpu__DOT__registers__DOT__r[5]) << Endl;
	log::info << L"A0        : " << str(L"%08x", soc->rootp->SoC__DOT__cpu__DOT__registers__DOT__r[10]) << Endl;
	log::info << L"A1        : " << str(L"%08x", soc->rootp->SoC__DOT__cpu__DOT__registers__DOT__r[11]) << Endl;
	log::info << L"A2        : " << str(L"%08x", soc->rootp->SoC__DOT__cpu__DOT__registers__DOT__r[12]) << Endl;
	log::info << L"A3        : " << str(L"%08x", soc->rootp->SoC__DOT__cpu__DOT__registers__DOT__r[13]) << Endl;
	log::info << L"A4        : " << str(L"%08x", soc->rootp->SoC__DOT__cpu__DOT__registers__DOT__r[14]) << Endl;
	log::info << L"A5        : " << str(L"%08x", soc->rootp->SoC__DOT__cpu__DOT__registers__DOT__r[15]) << Endl;
	log::info << L"S2        : " << str(L"%08x", soc->rootp->SoC__DOT__cpu__DOT__registers__DOT__r[18]) << Endl;

	hdmi.getImage()->save(L"Rv32T.png");

	if (tfp)
	{
		tfp->close();
		delete tfp;
	}

	safeDestroy(form);
	return 0;
}