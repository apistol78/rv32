#include <Core/Io/FileOutputStream.h>
#include <Core/Io/FileSystem.h>
#include <Core/Io/IStream.h>
#include <Core/Io/StringReader.h>
#include <Core/Io/AnsiEncoding.h>
#include <Core/Io/Utf8Encoding.h>
#include <Core/Log/Log.h>
#include <Core/Misc/CommandLine.h>
#include <Core/Misc/String.h>
#include "Rv32/Bus.h"
#include "Rv32/CPU.h"
#include "Rv32/Memory.h"
#include "Rv32/Video.h"

using namespace traktor;

// https://github.com/takahirox/riscv-rust/blob/master/src/cpu.rs
// https://mindchasers.com/dev/rv-getting-started

int main(int argc, const char** argv)
{
	CommandLine cmdLine(argc, argv);

	Memory memory(0x00c00000);	// 12 MiB
	Video video;

	Bus bus;
	bus.map(0x00000000, 0x00bfffff, &memory);
	bus.map(0x10000000, 0x1000ffff, &video);

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

		uint8_t data[1024];
		for (;;)
		{
			int64_t r = f->read(data, sizeof(data));
			if (r <= 0)
				break;

			for (int64_t i = 0; i < (int32_t)r; ++i)
			{
				bus.writeU8(offset++, data[i]);
				if (bus.error())
					return 1;
			}
		}
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
	}

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

	for (;;)
	{
		if (!cpu.tick())
			break;
		if (bus.error())
			break;
	}
}
