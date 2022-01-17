#include <Core/Io/DynamicMemoryStream.h>
#include <Core/Io/FileSystem.h>
#include <Core/Io/StreamCopy.h>

#include <Core/Log/Log.h>
#include <Core/Misc/SafeDestroy.h>
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
#include "Rv32T/ELF.h"
#include "Rv32T/HDMI.h"
#include "Rv32T/LEDR.h"
#include "Rv32T/UART_TX.h"

// Verilated SoC
#include "SoC/VSoC.h"

using namespace traktor;

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

	// if (start != -1)
	// {
	// 	soc->SoC__DOT__cpu__DOT__fetch_pc = start;
	// 	soc->SoC__DOT__cpu__DOT__writeback_pc_next = start;
	// }

	return true;
}

int main(int argc, char **argv)
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

	// Create user interface.
	Ref< ui::Form > form = new ui::Form();
	form->create(L"RV32", ui::dpi96(640), ui::dpi96(400), ui::Form::WsDefault, new ui::FloodLayout());

	Ref< ui::Bitmap > framebuffer = new ui::Bitmap(320, 200);
	
	Ref< ui::Image > image = new ui::Image();
	image->create(form, framebuffer, ui::Image::WsScale);

	form->update();
	form->show();

	// Create SoC simulation.
	VSoC* soc = new VSoC();

	//log::info << L"PC: " << str(L"%08x", soc->SoC__DOT__cpu__DOT__fetch_pc) << Endl;

	loadELF(soc, L"build/rv32/ReleaseStatic/Bare");

	soc->CPU_RESET_n = 1;
	soc->CLOCK_125_p = 0;
	soc->eval();
	soc->CLOCK_125_p = 1;
	soc->eval();

	//log::info << L"PC: " << str(L"%08x", soc->SoC__DOT__cpu__DOT__fetch_pc) << Endl;

	HDMI hdmi;
	LEDR ledr;
	UART_TX uart_tx;

	for (;;)
	{
		if (!ui::Application::getInstance()->process())
			break;

		for (int32_t i = 0; i < 50000; ++i)
		{
			//log::info << L"PC: " << str(L"%08x", soc->SoC__DOT__cpu__DOT__fetch_pc) << Endl;

			soc->CLOCK_125_p = 0;
			soc->eval();
			soc->CLOCK_125_p = 1;
			soc->eval();

			hdmi.eval(soc);
			ledr.eval(soc);
			uart_tx.eval(soc);
		}

		framebuffer->copyImage(hdmi.getImage());
		image->setImage(framebuffer);
	}

	safeDestroy(form);
	return 0;
}