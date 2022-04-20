#include <Core/Log/Log.h>
#include "Rv32/ICache.h"
#include "Rv32/Bus.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"ICache", ICache, Object)

ICache::ICache(Bus* bus)
:	m_bus(bus)
,   m_hits(0)
,   m_misses(0)
{
    for (int32_t i = 0; i < 64; ++i)
        m_valid[i] = false;
}

ICache::~ICache()
{
    log::info << L"ICache " << m_hits << L"/" << m_misses << L" (" << (m_hits * 100) / (m_hits + m_misses) << L"%)" << Endl;
}

uint32_t ICache::readU32(uint32_t address)
{
	uint32_t tag = (address >> 2) & 63;

    if (m_valid[tag] && m_data[tag][0] == address)
    {
        m_hits++;
        return m_data[tag][1];
    }

    uint32_t data = m_bus->readU32(address);

    m_data[tag][0] = address;
    m_data[tag][1] = data;
    m_valid[tag] = true;
    m_misses++;
    
	return data;
}
