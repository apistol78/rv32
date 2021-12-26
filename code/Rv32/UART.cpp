#include <Core/Containers/StaticVector.h>
#include <Core/Io/AnsiEncoding.h>
#include <Core/Io/IStream.h>
#include <Core/Io/StringReader.h>
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/UART.h"

using namespace traktor;

namespace
{

template < typename T >
void write(AlignedVector< uint8_t >& outStream, T value)
{
	const uint8_t* v = (const uint8_t*)&value;
	for (int i = 0; i < sizeof(T); ++i)
		outStream.push_back(*v++);
}

bool decodeHEX(IStream* stream, AlignedVector< uint8_t >& outStream)
{
	StaticVector< uint8_t, 16 > record;
	std::wstring tmp;

	uint32_t segment = 0x00000000;
	uint32_t upper = 0x00000000;
	uint32_t start = 0xffffffff;
	uint32_t end = 0x00000000;

	StringReader sr(stream, new AnsiEncoding());
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

			write< uint8_t >(outStream, 0x01);
			write< uint32_t >(outStream, linear);
			write< uint8_t >(outStream, (uint8_t)record.size());

			for (int32_t i = 0; i < record.size(); ++i)
				write< uint8_t >(outStream, record[i]);			

			write< uint8_t >(outStream, cs);

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
			uint8_t cs = 0;

			// Add address to checksum.
			const uint8_t* p = (const uint8_t*)&linear;
			cs ^= p[0];
			cs ^= p[1];
			cs ^= p[2];
			cs ^= p[3];			

			write< uint8_t >(outStream, 0x02);
			write< uint32_t >(outStream, linear);
			write< uint8_t >(outStream, cs);
		}
		else
			log::warning << L"Unhandled HEX record type " << type << L"." << Endl;
	}

	log::info << L"HEX uploaded, adress range " << str(L"0x%08x", start) << L" - " << str(L"0x%08x", end) << L"." << Endl;
	return true;
}

}

T_IMPLEMENT_RTTI_CLASS(L"UART", UART, Device)

UART::UART(IStream* stream)
{
	if (stream)
		decodeHEX(stream, m_stream);
}

bool UART::writeU8(uint32_t address, uint8_t value)
{
	log::info << wchar_t(value);
	return true;
}

bool UART::writeU16(uint32_t address, uint16_t value)
{
	log::info << wchar_t(value);
	return true;
}

bool UART::writeU32(uint32_t address, uint32_t value)
{
	log::info << wchar_t(value);
	return true;
}

uint8_t UART::readU8(uint32_t address) const
{
	if (m_position < m_stream.size())
	{
		m_position += 1;
		return *(const uint8_t*)(m_stream.c_ptr() + m_position - 1);
	}
	else
		return 0;
}

uint16_t UART::readU16(uint32_t address) const
{
	return readU8(address);
}

uint32_t UART::readU32(uint32_t address) const
{
	return readU8(address);
}
