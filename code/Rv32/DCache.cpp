#include <Core/Log/Log.h>
#include "Rv32/DCache.h"
#include "Rv32/Bus.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"DCache", DCache, Object)

DCache::DCache(Bus* bus)
:	m_bus(bus)
,   m_hits(0)
,   m_misses(0)
{
    for (int32_t i = 0; i < 65536; ++i)
	{
        m_data[i].valid = false;
		m_data[i].dirty = false;
	}
}

DCache::~DCache()
{
    log::info << L"DCache " << m_hits << L"/" << m_misses << L" (" << (m_hits * 100) / (m_hits + m_misses) << L"%)" << Endl;
}

void DCache::writeU32(uint32_t address, uint32_t value)
{
	if (m_bus->cacheable(address))
	{
		const uint32_t tag = (address >> 2) & 65535;
		Line& line = m_data[tag];

		if (line.address != address && line.valid && line.dirty)
			m_bus->writeU32(line.address, line.word);

		line.address = address;
		line.word = value;
		line.valid = true;
		line.dirty = true;
	}
	else
		m_bus->writeU32(address, value);
}

uint32_t DCache::readU32(uint32_t address)
{
	if (m_bus->cacheable(address))
	{
		const uint32_t tag = (address >> 2) & 65535;
		Line& line = m_data[tag];

		if (line.valid && line.address == address)
		{
			m_hits++;
			return line.word;
		}

		if (line.valid && line.dirty)
			m_bus->writeU32(line.address, line.word);

		const uint32_t word = m_bus->readU32(address);

		line.address = address;
		line.word = word;
		line.valid = true;
		line.dirty = false;

		m_misses++;
	    return word;
	}
	else
		return m_bus->readU32(address);
}

void DCache::flush()
{
    for (int32_t i = 0; i < 65536; ++i)
	{
		Line& line = m_data[i];
		if (line.valid && line.dirty)
		{
			m_bus->writeU32(line.address, line.word);
			line.dirty = false;
		}
	}
}
