#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/ICache.h"
#include "Rv32/Bus.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"ICache", ICache, Object)

ICache::ICache(Bus* bus)
:	m_bus(bus)
,   m_hits(0)
,   m_misses(0)
{
	for (int32_t i = 0; i < 65536; ++i)
		m_data[i].valid = false;
}

ICache::~ICache()
{
	log::info << L"ICache " << m_hits << L"/" << m_misses << L" (" << str(L"%.2f%%", (m_hits * 100.0) / (m_hits + m_misses)) << L")" << Endl;
}

uint32_t ICache::readU32(uint32_t address)
{
	const uint32_t tag = (address >> 2) & 65535;
	Line& line = m_data[tag];

	if (line.valid && line.address == address)
	{
		m_hits++;
		return line.word;
	}

	const uint32_t word = m_bus->readU32(address);
	
	line.address = address;
	line.word = word;
	line.valid = true;

	m_misses++;
	return word;
}
