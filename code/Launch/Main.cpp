#include <string.h>

#include <Core/Containers/StaticVector.h>
#include <Core/Io/AnsiEncoding.h>
#include <Core/Io/BufferedStream.h>
#include <Core/Io/DynamicMemoryStream.h>
#include <Core/Io/FileSystem.h>
#include <Core/Io/FileOutputStream.h>
#include <Core/Io/StreamCopy.h>
#include <Core/Io/StringReader.h>
#include <Core/Io/Utf8Encoding.h>
#include <Core/Log/Log.h>
#include <Core/Log/LogRedirectTarget.h>
#include <Core/Misc/CommandLine.h>
#include <Core/Misc/String.h>
#include <Core/Thread/ThreadManager.h>
#include <Core/Thread/Thread.h>

#include <Ui/Application.h>
#include <Ui/Button.h>
#include <Ui/Edit.h>
#include <Ui/FileDialog.h>
#include <Ui/FloodLayout.h>
#include <Ui/Form.h>
#include <Ui/StyleSheet.h>
#include <Ui/TableLayout.h>
#include <Ui/Itf/IWidget.h>
#include <Ui/LogList/LogList.h>
#if defined(_WIN32)
#	include <Ui/Win32/WidgetFactoryWin32.h>
#elif defined(__APPLE__)
#	include <Ui/Cocoa/WidgetFactoryCocoa.h>
#elif defined(__LINUX__) || defined(__RPI__)
#	include <Ui/X11/WidgetFactoryX11.h>
#endif

#include <Xml/XmlDeserializer.h>

#include "Launch/ELF.h"
#include "Launch/Serial.h"

using namespace traktor;

template < typename T >
void write(Serial& serial, T value)
{
	serial.write(&value, sizeof(T));
}

template < typename T >
void write(Serial& serial, const T* value, int32_t count)
{
	serial.write(value, count * sizeof(T));
}

template < typename T >
T read(Serial& serial)
{
	T value = 0;
	serial.read(&value, sizeof(T));
	return value;
}

template < typename T >
void read(Serial& serial, T* value, int32_t count)
{
	serial.read(value, count * sizeof(T));
}

bool sendLine(Serial& serial, uint32_t base, const uint8_t* line, uint32_t length)
{
	uint8_t cs = 0;

	// Add address to checksum.
	const uint8_t* p = (const uint8_t*)&base;
	cs ^= p[0];
	cs ^= p[1];
	cs ^= p[2];
	cs ^= p[3];

	// Parse record and calculate checksum.
	for (uint32_t i = 0; i < length; ++i)
		cs ^= line[i];

	write< uint8_t >(serial, 0x01);
	write< uint32_t >(serial, base);
	write< uint8_t >(serial, (uint8_t)length);
	write< uint8_t >(serial, line, length);
	write< uint8_t >(serial, cs);

	uint8_t reply = read< uint8_t >(serial);
	if (reply != 0x80)
	{
		log::error << L"Error reply, got " << str(L"%02x", reply) << Endl;
		return false;
	}

	return true;
}

bool sendJump(Serial& serial, uint32_t start, uint32_t sp)
{
	uint8_t cs = 0;

	// Add address to checksum.
	{
		const uint8_t* p = (const uint8_t*)&start;
		cs ^= p[0];
		cs ^= p[1];
		cs ^= p[2];
		cs ^= p[3];
	}

	// Add stack to checksum.
	{
		const uint8_t* p = (const uint8_t*)&sp;
		cs ^= p[0];
		cs ^= p[1];
		cs ^= p[2];
		cs ^= p[3];				
	}

	write< uint8_t >(serial, 0x03);
	write< uint32_t >(serial, start);
	write< uint32_t >(serial, sp);
	write< uint8_t >(serial, cs);

	uint8_t reply = read< uint8_t >(serial);
	if (reply != 0x80)
	{
		log::error << L"Error reply, got " << str(L"%02x", reply) << Endl;
		return false;
	}

	return true;
}

bool uploadImage(Serial& serial, const std::wstring& fileName, uint32_t offset)
{
	Ref< traktor::IStream > f = FileSystem::getInstance().open(fileName, File::FmRead);
	if (!f)
	{
		log::error << L"Unable to open image \"" << fileName << L"\"." << Endl;
		return false;
	}

	uint32_t linear = offset;
	uint8_t data[16];
	for (;;)
	{
		int64_t r = f->read(data, sizeof(data));
		if (r <= 0)
			break;

		log::info << L"DATA " << str(L"%08x", linear) << L"..." << Endl;

		if (!sendLine(serial, linear, data, 16))
			return false;

		linear += 16;
	}

	log::info << L"Image uploaded" << Endl;
	return true;
}

bool uploadELF(Serial& serial, const std::wstring& fileName, uint32_t sp)
{
	AlignedVector< uint8_t > elf;
	uint32_t start = -1;
	uint32_t last = 0;

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

				for (uint32_t j = 0; j < shdr[i].sh_size; j += 128)
				{
					uint32_t cnt = std::min< uint32_t >(shdr[i].sh_size - j, 128);
					log::info << L"TEXT " << str(L"%08x", addr + j) << L" (" << cnt << L" bytes)..." << Endl;
					if (!sendLine(serial, addr + j, pbits + j, cnt))
						return false;
				}

				last = std::max(last, addr + shdr[i].sh_size);
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
					start = sym[j].st_value;
					break;
				}
			}
		}
	}

	if (start != -1)
	{
		if (sp)
			log::info << L"JUMP " << str(L"0x%08x", start) << L", SP " << str(L"0x%08x", sp) << Endl;
		else
			log::info << L"JUMP " << str(L"0x%08x", start) << Endl;

		if (!sendJump(serial, start, sp))
			return false;
	}

	return true;
}

bool uploadHEX(Serial& serial, const std::wstring& fileName, uint32_t sp)
{
	StaticVector< uint8_t, 16 > record;
	std::wstring tmp;

	Ref< traktor::IStream > ff = FileSystem::getInstance().open(fileName, File::FmRead);
	if (!ff)
	{
		log::error << L"Unable to open HEX \"" << fileName << L"\"." << Endl;
		return false;
	}

	Ref< traktor::IStream > f = new BufferedStream(ff);

	int64_t fileSize = f->available();

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

		int32_t percent = ((f->tell() * 100) / fileSize);

		// Parse header.
		int32_t ln = parseString< int32_t >(L"0x" + tmp.substr(0, 2));
		int32_t addr = parseString< int32_t >(L"0x" + tmp.substr(2, 4));
		int32_t type = parseString< int32_t >(L"0x" + tmp.substr(6, 2));

		if (type == 0x00)
		{
			if (ln > 16)
			{
				log::error << L"Too long record." << Endl;
				return false;
			}

			log::info << L"TEXT " << str(L"%08x", (upper | addr) + segment) << L" (" << percent << L"%)..." << Endl;

			const uint32_t linear = (upper | addr) + segment;

			// Parse record.
			record.resize(0);
			for (int32_t i = 8; i < 8 + ln * 2; i += 2)
			{
				const int32_t v = parseString< int32_t >(L"0x" + tmp.substr(i, 2));
				record.push_back((uint8_t)v);
			}

			if (!sendLine(serial, linear, record.c_ptr(), record.size()))
				return false;

			addr += record.size();

			start = std::min< uint32_t >(start, linear);
			end = std::max< uint32_t >(end, linear);
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
		else if (type == 0x05)
		{
			const uint32_t linear = parseString< uint32_t >(L"0x" + tmp.substr(8, 8));
			
			if (sp != 0)
				log::info << L"JUMP " << str(L"%08x", linear) << L" (SP: 0x" << str(L"%08x", sp) << L")..." << Endl;
			else
				log::info << L"JUMP " << str(L"%08x", linear) << L"..." << Endl;

			if (!sendJump(serial, linear, sp))
				return false;

			// Cannot load more since target is executing.
			break;
		}
		else
			log::warning << L"Unhandled HEX record type " << type << L"." << Endl;
	}

	log::info << L"HEX uploaded, adress range " << str(L"0x%08x", start) << L" - " << str(L"0x%08x", end) << L"." << Endl;
	return true;
}

bool memcheck(Serial& serial, uint32_t from, uint32_t to)
{
	uint8_t utd[16];
	uint8_t ind[16];

	uint8_t cnt = 0;
	uint32_t error = 0;

	const uint32_t nb = 16;

	for (uint32_t addr = from; addr < to; addr += nb)
	{
		for (int32_t i = 0; i < nb; ++i)
			utd[i] = cnt++;

		log::info << L"S " << str(L"%08x", addr) << L": ";
		for (int32_t i = 0; i < nb; ++i)
			log::info << str(L"%02x", utd[i]) << L" ";
		log::info << Endl;

		// Write random data.
		uint8_t cs = 0;

		const uint8_t* p = (const uint8_t*)&addr;
		cs ^= p[0];
		cs ^= p[1];
		cs ^= p[2];
		cs ^= p[3];

		for (int32_t i = 0; i < nb; ++i)
			cs ^= (uint8_t)utd[i];

		write< uint8_t >(serial, 0x01);
		write< uint32_t >(serial, addr);
		write< uint8_t >(serial, nb);
		write< uint8_t >(serial, utd, nb);
		write< uint8_t >(serial, cs);

		uint8_t reply = read< uint8_t >(serial);
		if (reply != 0x80)
		{
			log::error << L"Error reply, got " << str(L"%02x", reply) << Endl;
			return false;
		}

		// Read back data.
		write< uint8_t >(serial, 0x02);
		write< uint32_t >(serial, addr);
		write< uint8_t >(serial, nb);

		reply = read< uint8_t >(serial);
		if (reply != 0x80)
		{
			log::error << L"Error reply, got " << str(L"%02x", reply) << Endl;
			return false;
		}
		read< uint8_t >(serial, ind, nb);

		log::info << L"R " << str(L"%08x", addr) << L": ";
		for (int32_t i = 0; i < nb; ++i)
			log::info << str(L"%02x", ind[i]) << L" ";

		if (memcmp(ind, utd, nb) != 0)
		{
			log::info << L"MISMATCH!";
			error++;
		}

		log::info << Endl;

		if (error)
			break;
	}

	return error == 0;
}

uint8_t echoRnd()
{
	uint8_t v = 0;
	do { v = rand() & 255; } while(v == 0x01 || v == 0x02 || v == 0x03);
	return v;
}

class LogListTarget : public ILogTarget
{
public:
	LogListTarget(ui::LogList* logList)
	:	m_logList(logList)
	{
	}

	virtual void log(uint32_t threadId, int32_t level, const wchar_t* str) override final
	{
		m_logList->add(threadId, (ui::LogList::LogLevel)(1 << level), str);
	}

private:
	ui::LogList* m_logList;
};


Ref< ui::StyleSheet > loadStyleSheet(const Path& pathName)
{
	Ref< traktor::IStream > file = FileSystem::getInstance().open(pathName, traktor::File::FmRead);
	if (file)
	{
		Ref< ui::StyleSheet > styleSheet = xml::XmlDeserializer(file, pathName.getPathName()).readObject< ui::StyleSheet >();
		if (!styleSheet)
			return nullptr;

		auto includes = styleSheet->getInclude();
		for (const auto& include : includes)
		{
			Ref< ui::StyleSheet > includeStyleSheet = loadStyleSheet(include);
			if (!includeStyleSheet)
				return nullptr;

			styleSheet = includeStyleSheet->merge(styleSheet);
			if (!styleSheet)
				return nullptr;
		}

		return styleSheet;
	}
	else
		return nullptr;
}

int main(int argc, const char** argv)
{
	CommandLine commandLine(argc, argv);
	Serial serial;

	int32_t port = 0;
	if (commandLine.hasOption('p', L"port"))
		port = commandLine.getOption('p', L"port").getInteger();

	Serial::Configuration configuration;
	configuration.baudRate = 115200;
	configuration.stopBits = 1;
	configuration.parity = Serial::Parity::No;
	configuration.byteSize = 8;
	configuration.dtrControl = Serial::DtrControl::Disable;

	if (!serial.open(port, configuration))
	{
		log::error << L"Unable to open serial port " << port << L"." << Endl;
		return 1;
	}

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

	// Load default stylesheet.
	std::wstring styleSheetName = L"$(TRAKTOR_HOME)/resources/runtime/themes/Light/StyleSheet.xss";
	Ref< ui::StyleSheet > styleSheet = loadStyleSheet(styleSheetName);
	if (!styleSheet)
	{
		log::error << L"Unable to load stylesheet " << styleSheetName << Endl;
		return false;
	}
	ui::Application::getInstance()->setStyleSheet(styleSheet);

	Ref< ui::Form > form = new ui::Form();
	form->create(L"Launcher", ui::dpi96(640), ui::dpi96(480), ui::Form::WsDefault, new ui::TableLayout(L"100%", L"*,100%", 4, 4));

	Ref< ui::Container > top = new ui::Container();
	top->create(form, ui::WsNone, new ui::TableLayout(L"100%,*", L"*", 0, 4));

	std::wstring elf = L"";
	if (commandLine.hasOption('e', L"elf"))
		elf = commandLine.getOption('e', L"elf").getString();

	Ref< ui::Edit > editFile = new ui::Edit();
	editFile->create(top, elf);

	Ref< ui::Button > buttonBrowse = new ui::Button();
	buttonBrowse->create(top, L"...");
	buttonBrowse->addEventHandler< ui::ButtonClickEvent >([&](ui::ButtonClickEvent* event) {
		ui::FileDialog fileDialog;
		fileDialog.create(form, L"File", L"Select file to upload", L"All files (*.*);*.*");

		Path path;
		if (fileDialog.showModal(path))
		{
			editFile->setText(path.getPathNameNoVolume());
			editFile->update();
		}

		fileDialog.destroy();
	});

	uint32_t sp = 0;
	if (commandLine.hasOption('s', L"stack"))
		sp = (uint32_t)commandLine.getOption('s', L"stack").getInteger();

	Ref< ui::Edit > editStack = new ui::Edit();
	editStack->create(top, str(L"0x%08x", sp));

	Thread* threadUpload = nullptr;
	bool running = false;

	Ref< ui::Button > buttonUpload = new ui::Button();
	buttonUpload->create(top, L"Run");
	buttonUpload->addEventHandler< ui::ButtonClickEvent >([&](ui::ButtonClickEvent* event) {

		if (threadUpload)
		{
			log::info << L"Upload already in progress." << Endl;
			return;
		}

		running = false;

		uint32_t sp = parseString< uint32_t >(editStack->getText());

		threadUpload = ThreadManager::getInstance().create([&]() {
			Path fileName(editFile->getText());
			if (compareIgnoreCase(fileName.getExtension(), L"hex") == 0)
			{
				if (uploadHEX(serial, editFile->getText(), sp))
					running = true;
			}
			else
			{
				if (uploadELF(serial, editFile->getText(), sp))
					running = true;
			}
		});
		threadUpload->start();
		
	});

	Ref< ui::LogList > logList = new ui::LogList();
	logList->create(form, ui::WsAccelerated, nullptr);

	Ref< ILogTarget > defaultInfoLog = log::info.getGlobalTarget();
	Ref< LogListTarget > logTarget = new LogListTarget(logList);
	log::info.setGlobalTarget(new LogRedirectTarget(defaultInfoLog, logTarget));

	form->update();
	form->show();

	bool keyStates[9] = {};

	ui::Application::getInstance()->addEventHandler< ui::KeyDownEvent >([&](ui::KeyDownEvent* event) {

		if (!running)
			return;

		uint8_t keyCode = 0;
		switch (event->getVirtualKey())
		{
		case ui::VkA:
		case ui::VkLeft:
			keyCode = 1;
			break;
		
		case ui::VkD:
		case ui::VkRight:
			keyCode = 2;
			break;

		case ui::VkW:
		case ui::VkUp:
			keyCode = 3;
			break;

		case ui::VkS:
		case ui::VkDown:
			keyCode = 4;
			break;
			
		case ui::VkShift:
			keyCode = 5;
			break;

		case ui::VkControl:
			keyCode = 6;
			break;

		case ui::VkEscape:
			keyCode = 7;
			break;

		case ui::VkReturn:
			keyCode = 8;
			break;
			
		default:
			break;
		}

		if (keyCode != 0 && keyStates[keyCode] == false)
		{
			write< uint8_t >(serial, keyCode | 0x80);
			keyStates[keyCode] = true;
		}			
	});
	ui::Application::getInstance()->addEventHandler< ui::KeyUpEvent >([&](ui::KeyUpEvent* event){

		if (!running)
			return;
		if (event->isRepeat())
			return;

		uint8_t keyCode = 0;
		switch (event->getVirtualKey())
		{
		case ui::VkA:
		case ui::VkLeft:
			keyCode = 1;
			break;
		
		case ui::VkD:
		case ui::VkRight:
			keyCode = 2;
			break;

		case ui::VkW:
		case ui::VkUp:
			keyCode = 3;
			break;

		case ui::VkS:
		case ui::VkDown:
			keyCode = 4;
			break;
			
		case ui::VkShift:
			keyCode = 5;
			break;

		case ui::VkControl:
			keyCode = 6;
			break;

		case ui::VkEscape:
			keyCode = 7;
			break;

		case ui::VkReturn:
			keyCode = 8;
			break;

		default:
			break;
		}

		if (keyCode != 0 && keyStates[keyCode] == true)
		{
			write< uint8_t >(serial, keyCode);
			keyStates[keyCode] = false;
		}	
	});

	while (ui::Application::getInstance()->process())
	{
		if (threadUpload != nullptr && threadUpload->wait(0))
		{
			ThreadManager::getInstance().destroy(threadUpload);
			threadUpload = nullptr;
		}

		if (running)
		{
			if (serial.available() > 0)
			{
				uint8_t ch = read< uint8_t >(serial);
				if (!iscntrl(ch))
					log::info << wchar_t(ch);
				else if (ch == '\n')
					log::info << Endl;
			}
		}
	}

	form->destroy();

	ui::Application::getInstance()->finalize();
	return 0;
}