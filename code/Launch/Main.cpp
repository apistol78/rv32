#include <Core/Containers/StaticVector.h>
#include <Core/Io/AnsiEncoding.h>
#include <Core/Io/FileSystem.h>
#include <Core/Io/FileOutputStream.h>
#include <Core/Io/StringReader.h>
#include <Core/Io/Utf8Encoding.h>
#include <Core/Log/Log.h>
#include <Core/Misc/CommandLine.h>
#include <Core/Misc/String.h>
#include "Launch/Serial.h"

using namespace traktor;

template < typename T >
void write(Serial& serial, T value)
{
	serial.write(&value, sizeof(T));
}

template < typename T >
T read(Serial& serial)
{
	T value = 0;
	serial.read(&value, sizeof(T));
	return value;
}

bool uploadHEX(Serial& serial, const std::wstring& fileName)
{
	StaticVector< uint8_t, 16 > record;
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
			if (ln > 16)
			{
				log::error << L"Too long record." << Endl;
				return false;
			}

			log::info << L"TEXT " << str(L"%08x", (upper | addr) + segment) << L"..." << Endl;

			const uint32_t linear = (upper | addr) + segment;
			uint8_t cs = 0;

			// Add address to checksum.
			const uint8_t* p = (const uint8_t*)&linear;
			cs ^= p[0];
			cs ^= p[1];
			cs ^= p[2];
			cs ^= p[3];

			// Parse record and calculate checksum.
			record.resize(0);
			for (int32_t i = 8; i < 8 + ln * 2; i += 2)
			{
				const int32_t v = parseString< int32_t >(L"0x" + tmp.substr(i, 2));
				record.push_back((uint8_t)v);
				cs ^= (uint8_t)v;
			}

			write< uint8_t >(serial, 0x01);
			write< uint32_t >(serial, linear);
			write< uint8_t >(serial, (uint8_t)record.size());

			for (int32_t i = 0; i < record.size(); ++i)
				write< uint8_t >(serial, record[i]);

			write< uint8_t >(serial, cs);

			uint8_t reply = read< uint8_t >(serial);
			if (reply != 0x80)
			{
				log::error << L"Error reply, got " << str(L"%02x", reply) << Endl;
				return false;
			}

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
			log::info << L"JUMP " << str(L"%08x", linear) << L"..." << Endl;

			uint8_t cs = 0;

			// Add address to checksum.
			const uint8_t* p = (const uint8_t*)&linear;
			cs ^= p[0];
			cs ^= p[1];
			cs ^= p[2];
			cs ^= p[3];

			write< uint8_t >(serial, 0x02);
			write< uint32_t >(serial, linear);
			write< uint8_t >(serial, cs);

			uint8_t reply = read< uint8_t >(serial);
			if (reply != 0x80)
			{
				log::error << L"Error reply, got " << str(L"%02x", reply) << Endl;
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
	configuration.baudRate = 115200; //  9600;
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

	if (commandLine.hasOption('t', L"terminal"))
	{
		log::info << L"Terminal ready! Waiting for data..." << Endl;
		for (;;)
		{
			uint8_t ch = read< uint8_t >(serial);
			log::info << (wchar_t)ch << Endl;
		}
	}

	return 0;
}