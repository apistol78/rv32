#include <string.h>
#include <Core/Containers/StaticVector.h>
#include <Core/Io/AnsiEncoding.h>
#include <Core/Io/BufferedStream.h>
#include <Core/Io/FileSystem.h>
#include <Core/Io/FileOutputStream.h>
#include <Core/Io/StringReader.h>
#include <Core/Io/Utf8Encoding.h>
#include <Core/Log/Log.h>
#include <Core/Misc/CommandLine.h>
#include <Core/Misc/String.h>
#include <Core/Thread/ThreadManager.h>
#include <Core/Thread/Thread.h>
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

		uint8_t cs = 0;

		// Add address to checksum.
		const uint8_t* p = (const uint8_t*)&linear;
		cs ^= p[0];
		cs ^= p[1];
		cs ^= p[2];
		cs ^= p[3];

		// Add data to calculate checksum.
		for (int32_t i = 0; i < 16; ++i)
			cs ^= data[i];

		write< uint8_t >(serial, 0x01);
		write< uint32_t >(serial, linear);
		write< uint8_t >(serial, 16);
		write< uint8_t >(serial, data, 16);
		write< uint8_t >(serial, cs);

		uint8_t reply = read< uint8_t >(serial);
		if (reply != 0x80)
		{
			log::error << L"Error reply, got " << str(L"%02x", reply) << Endl;
			return false;
		}

		linear += 16;
	}

	log::info << L"Image uploaded" << Endl;
	return true;
}

bool uploadELF(Serial& serial, const std::wstring& fileName, uint32_t sp)
{
	Ref< traktor::IStream > f = FileSystem::getInstance().open(fileName, File::FmRead);
	if (!f)
	{
		log::error << L"Unable to open ELF \"" << fileName << L"\"." << Endl;
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
			write< uint8_t >(serial, record.c_ptr(), record.size());
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
			
			if (sp != 0)
				log::info << L"JUMP " << str(L"%08x", linear) << L" (SP: 0x" << str(L"%08x", sp) << L")..." << Endl;
			else
				log::info << L"JUMP " << str(L"%08x", linear) << L"..." << Endl;

			uint8_t cs = 0;

			// Add address to checksum.
			{
				const uint8_t* p = (const uint8_t*)&linear;
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
			write< uint32_t >(serial, linear);
			write< uint32_t >(serial, sp);
			write< uint8_t >(serial, cs);

			uint8_t reply = read< uint8_t >(serial);
			if (reply != 0x80)
			{
				log::error << L"Error reply, got " << str(L"%02x", reply) << Endl;
				return false;
			}

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

int main(int argc, const char** argv)
{
	CommandLine commandLine(argc, argv);
	Serial serial;

	int32_t port = 0;
	if (commandLine.hasOption('p', L"port"))
		port = commandLine.getOption('p', L"port").getInteger();

	Serial::Configuration configuration;
	configuration.baudRate = 115200; // 9600;
	configuration.stopBits = 1;
	configuration.parity = Serial::Parity::No;
	configuration.byteSize = 8;
	configuration.dtrControl = Serial::DtrControl::Disable;

	if (!serial.open(port, configuration))
	{
		log::error << L"Unable to open serial port " << port << L"." << Endl;
		return 1;
	}

	if (commandLine.hasOption('e', L"echocheck"))
	{
		for (;;)
		{
			uint8_t v[16];
			uint8_t r[16];

			for (int i = 0; i < 16; ++i)
				v[i] = echoRnd();

			write< uint8_t >(serial, v, 16);
			read< uint8_t >(serial, r, 16);

			if (memcmp(v, r, 16) == 0)
				log::info << L"OK" << Endl;
			else
				log::info << L"ERROR" << Endl;
		}
	}

	if (commandLine.hasOption('m', L"memcheck"))
	{
		bool result = true;

		// SRAM
		result &= memcheck(serial, 0x10000000, 0x10000100);

		// SDRAM
		result &= memcheck(serial, 0x20000000, 0x28000000);

		if (!result)
			return 1;
	}

	if (commandLine.hasOption(L"image-file") && commandLine.hasOption(L"image-base"))
	{
		std::wstring fileName = commandLine.getOption(L"image-file").getString();
		int32_t offset = commandLine.getOption(L"image-base").getInteger();
		if (!uploadImage(serial, fileName, offset))
			return 1;
	}

	if (commandLine.hasOption('u', L"upload"))
	{
		uint32_t sp = 0;

		if (commandLine.hasOption('s', L"stack"))
			sp = (uint32_t)commandLine.getOption('s', L"stack").getInteger();

		if (!uploadHEX(serial, commandLine.getOption('u', L"upload").getString(), sp))
			return 1;
	}

	if (commandLine.hasOption('t', L"terminal"))
	{
		log::info << L"Terminal ready! Waiting for data..." << Endl;
		for (;;)
		{
			uint8_t ch = read< uint8_t >(serial);
			log::info << (wchar_t)ch;
		}
	}

	return 0;
}