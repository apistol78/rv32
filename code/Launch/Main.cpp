#include <Core/Io/AnsiEncoding.h>
#include <Core/Io/FileSystem.h>
#include <Core/Io/FileOutputStream.h>
#include <Core/Io/StringReader.h>
#include <Core/Io/Utf8Encoding.h>
#include <Core/Log/Log.h>
#include <Core/Misc/CommandLine.h>
#include <Core/Misc/String.h>
#include <Core/Thread/Thread.h>
#include <Core/Thread/ThreadManager.h>
#include "Launch/Serial.h"

using namespace traktor;

template < typename T >
void write(Serial& serial, T value)
{
	const uint8_t* v = (const uint8_t*)&value;
	for (int i = 0; i < sizeof(T); ++i)
	{
		serial.write(v++, 1);
		ThreadManager::getInstance().getCurrentThread()->sleep(30);
	}
}

template < typename T >
T read(Serial& serial)
{
	T value = 0;
	serial.read(&value, sizeof(T));
	return value;
}

bool writePoke(Serial& serial, uint32_t address, uint8_t data)
{
	write< uint8_t >(serial, 0x01);
	write< uint32_t >(serial, address);
	write< uint8_t >(serial, data);
	uint8_t echo = read< uint8_t >(serial);
	if (echo != data)
	{
		log::error << L"Invalid echo, sent " << str(L"%02x", data) << L", got back " << str(L"%02x", echo) << Endl;
		return false;
	}
	else
		return true;
}

uint8_t writePeek(Serial& serial, uint32_t address)
{
	write< uint8_t >(serial, 0x02);
	write< uint32_t >(serial, address);
	return read< uint8_t >(serial);
}

bool writeJump(Serial& serial, uint32_t address)
{
	write< uint8_t >(serial, 0x03);
	write< uint32_t >(serial, address);
	return read< uint8_t >(serial) == 0x80;
}

bool uploadHEX(Serial& serial, const std::wstring& fileName)
{
	std::wstring tmp;

	Ref< traktor::IStream > f = FileSystem::getInstance().open(fileName, File::FmRead);
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
			log::info << L"TEXT " << str(L"%08x", (upper | addr) + segment) << L"..." << Endl;

			for (int32_t i = 8; i < 8 + ln * 2; i += 2)
			{
				const int32_t v = parseString< int32_t >(L"0x" + tmp.substr(i, 2));
				const uint32_t linear = (upper | addr) + segment;

				if (!writePoke(serial, linear, v))
				{
					log::error << L"Failed to poke" << Endl;
					return false;
				}

				start = std::min< uint32_t >(start, linear);
				end = std::max< uint32_t >(end, linear);

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
			log::info << L"JUMP " << str(L"%08x", linear) << L"..." << Endl;
			if (!writeJump(serial, linear))
			{
				log::error << L"Failed to jump" << Endl;
				return false;
			}
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
	Serial serial;

	Serial::Configuration configuration;
	configuration.baudRate = 9600;
	configuration.stopBits = 1;
	configuration.parity = Serial::Parity::No;
	configuration.byteSize = 8;
	configuration.dtrControl = Serial::DtrControl::Disable;

	if (!serial.open(3, configuration))
	{
		log::error << L"Unable to open serial port." << Endl;
		return 1;
	}

	if (!uploadHEX(serial, commandLine.getString(0)))
		return 1;

	return 0;
}