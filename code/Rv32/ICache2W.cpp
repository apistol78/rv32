#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/ICache2W.h"
#include "Rv32/Bus.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"ICache2W", ICache2W, ICache)

ICache2W::ICache2W(Bus* bus)
:	m_bus(bus)
,   m_hits(0)
,   m_misses(0)
{
	for (int32_t i = 0; i < sizeof_array(m_data); ++i)
	{
		m_data[i].valid[0] = false;
		m_data[i].valid[1] = false;
		m_data[i].last = 0;
	}
}

ICache2W::~ICache2W()
{
	log::info << L"ICache2W " << m_hits << L"/" << m_misses << L" (" << str(L"%.2f%%", (m_hits * 100.0) / (m_hits + m_misses)) << L")" << Endl;
}

uint32_t ICache2W::readU32(uint32_t address)
{
	/*

33222222222211111111110000000000
10987654321098765432109876543210
tttttttttttttttttsssssssssssss..

	*/

	const uint32_t set = (address >> 2) & 0b1111111111111;
	const uint32_t tag = address >> 15;

	Line& line = m_data[set];

	if (line.valid[0] && line.tag[0] == tag)
	{
		m_hits++;
		return line.word[0];
	}
	if (line.valid[1] && line.tag[1] == tag)
	{
		m_hits++;
		return line.word[1];
	}

	const uint32_t word = m_bus->readU32(address);
	
	if (!line.valid[0])
	{
		line.tag[0] = tag;
		line.word[0] = word;
		line.valid[0] = true;
		line.last = 0;
	}
	else if (!line.valid[1])
	{
		line.tag[1] = tag;
		line.word[1] = word;
		line.valid[1] = true;
		line.last = 1;
	}
	else
	{
		// Replace oldest entry.
		if (line.last == 1)
		{
			line.tag[0] = tag;
			line.word[0] = word;
			line.last = 0;			
		}
		else
		{
			line.tag[1] = tag;
			line.word[1] = word;
			line.last = 1;			
		}
	}

	m_misses++;
	return word;
}
