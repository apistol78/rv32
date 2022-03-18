#include <Core/Io/AnsiEncoding.h>
#include <Core/Io/FileSystem.h>
#include <Core/Io/FileOutputStream.h>
#include <Core/Io/StringReader.h>
#include <Core/Log/Log.h>
#include <Core/Misc/CommandLine.h>
#include <Core/Misc/String.h>

using namespace traktor;

bool loadHEX(const std::wstring& fileName, AlignedVector< uint8_t >& outMemory)
{
	std::wstring tmp;

	Ref< IStream > f = FileSystem::getInstance().open(fileName, File::FmRead);
	if (!f)
	{
		log::error << L"Unable to open HEX \"" << fileName << L"\"." << Endl;
		return false;
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
				const int32_t v = parseString< int32_t >(L"0x" + tmp.substr(i, 2));
				const uint32_t linear = (upper | addr) + segment;

				outMemory.resize(std::max< uint32_t >(linear + 1, outMemory.size()), 0x00);
				outMemory[linear] = v;

				start = std::min(start, linear);
				end = std::max(end, linear);

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
		else if (type == 0x05)
			continue;
		else
			log::warning << L"Unhandled HEX record type " << type << L"." << Endl;
	}

	log::info << L"HEX loaded, adress range " << str(L"0x%08x", start) << L" - " << str(L"0x%08x", end) << L"." << Endl;
	return true;
}

bool writeVerilog(const std::wstring& fileName, uint8_t word, const AlignedVector< uint8_t >& image)
{
	std::wstring tmp;

	Ref< IStream > f = FileSystem::getInstance().open(fileName, File::FmWrite);
	if (!f)
	{
		log::error << L"Unable to create \"" << fileName << L"\"." << Endl;
		return false;
	}

	FileOutputStream fos(f, new AnsiEncoding());
	switch (word)
	{
	default:
	case 8:
		{
			for (uint32_t i = 0; i < image.size(); ++i)
				fos << str(L"%02x", image[i]) << Endl;
			log::info << image.size() << L" 8-bit words written." << Endl;
		}
		break;

	case 16:
		{
			for (uint32_t i = 0; i < image.size(); i += 2)
				fos << str(L"%04x", *(uint16_t*)(image.c_ptr() + i)) << Endl;
			log::info << (image.size() / 2) << L" 16-bit words written." << Endl;
		}
		break;

	case 32:
		{
			for (uint32_t i = 0; i < image.size(); i += 4)
				fos << str(L"%08x", *(uint32_t*)(image.c_ptr() + i)) << Endl;
			log::info << (image.size() / 4) << L" 32-bit words written." << Endl;
		}
		break;
	}

	return true;
}

bool writeVerilogRange(const std::wstring& fileName, uint8_t word, const AlignedVector< uint8_t >& image)
{
	std::wstring tmp;

	Ref< IStream > f = FileSystem::getInstance().open(fileName, File::FmWrite);
	if (!f)
	{
		log::error << L"Unable to create \"" << fileName << L"\"." << Endl;
		return false;
	}

	FileOutputStream fos(f, new AnsiEncoding());
	fos << L"0:" << (image.size() / (word / 8)) - 1;

	return true;
}

int main(int argc, const char** argv)
{
	CommandLine commandLine(argc, argv);
	AlignedVector< uint8_t > image;

	if (!loadHEX(commandLine.getString(0), image))
		return 1;

	const int32_t word = commandLine.hasOption('w', L"word") ? commandLine.getOption('w', L"word").getInteger() : 32;

	if (commandLine.hasOption(L"vmem"))
	{
		if (!writeVerilog(commandLine.getOption(L"vmem").getString(), word, image))
			return 2;
	}
	if (commandLine.hasOption(L"vmem-range"))
	{
		if (!writeVerilogRange(commandLine.getOption(L"vmem-range").getString(), word, image))
			return 2;
	}

	log::info << L"Image converted succesfully." << Endl;
	return 0;
}