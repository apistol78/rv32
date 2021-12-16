#include <Core/Io/AnsiEncoding.h>
#include <Core/Io/FileSystem.h>
#include <Core/Io/FileOutputStream.h>
#include <Core/Io/StringReader.h>
#include <Core/Log/Log.h>
#include <Core/Misc/CommandLine.h>
#include <Core/Misc/String.h>

using namespace traktor;

void writePoke(uint32_t address, uint8_t data)
{
	log::info <<
		L"0x01, " <<
		str(L"0x%02x", address & 0xff) << L", " <<
		str(L"0x%02x", (address >> 8) & 0xff) << L", " <<
		str(L"0x%02x", (address >> 16) & 0xff) << L", " <<
		str(L"0x%02x", (address >> 24) & 0xff) << L", " <<
		str(L"0x%02x", data) << L", " << Endl;
}

void writeJump(uint32_t address)
{
	log::info <<
		L"0x02, " <<
		str(L"0x%02x", address & 0xff) << L", " <<
		str(L"0x%02x", (address >> 8) & 0xff) << L", " <<
		str(L"0x%02x", (address >> 16) & 0xff) << L", " <<
		str(L"0x%02x", (address >> 24) & 0xff) << L", " << Endl;
}

bool uploadHEX(const std::wstring& fileName)
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

				writePoke(linear, v);

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
		{
			const uint32_t linear = parseString< uint32_t >(L"0x" + tmp.substr(8, 8));
			writeJump(linear);
		}
		else
			log::warning << L"Unhandled HEX record type " << type << L"." << Endl;
	}

	log::info << L"HEX uploaded, adress range " << str(L"0x%08x", start) << L" - " << str(L"0x%08x", end) << L"." << Endl;
	return true;
}

int main(int argc, const char** argv)
{
	CommandLine commandLine(argc, argv);

	if (!uploadHEX(commandLine.getString(0)))
		return 1;

	return 0;
}