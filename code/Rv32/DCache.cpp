#include <Core/Log/Log.h>
#include "Rv32/DCache.h"
#include "Rv32/Bus.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"DCache", DCache, Object)

DCache::DCache(Bus* bus)
:	m_bus(bus)
{
}

void DCache::writeU8(uint32_t address, uint8_t value)
{
	const uint32_t wa = address & ~3;
	const uint32_t wb = address & 3;
	uint32_t w = m_bus->readU32(wa);
	switch(wb)
	{
	case 0:
		w = (w & 0xffffff00) | value;
		break;
	case 1:
		w = (w & 0xffff00ff) | (value << 8);
		break;
	case 2:
		w = (w & 0xff00ffff) | (value << 16);
		break;
	case 3:
		w = (w & 0x00ffffff) | (value << 24);
		break;
	}
	m_bus->writeU32(wa, w);
}

void DCache::writeU16(uint32_t address, uint16_t value)
{
	const uint32_t wa = address & ~3;
	const uint32_t wb = address & 3;
	uint32_t w = m_bus->readU32(wa);
	switch(wb)
	{
	case 0:
		w = (w & 0xffff0000) | value;
		break;
	case 2:
		w = (w & 0x0000ffff) | (value << 16);
		break;
	}
	m_bus->writeU32(wa, w);
}

void DCache::writeU32(uint32_t address, uint32_t value)
{
	m_bus->writeU32(address, value);
}

uint8_t DCache::readU8(uint32_t address) const
{
	const uint32_t ra = address & ~3;
	const uint32_t rb = address & 3;
	uint32_t r = m_bus->readU32(ra);
	switch(rb)
	{
	case 0:
		return r & 0xff;
	case 1:
		return (r >> 8) & 0xff;
	case 2:
		return (r >> 16) & 0xff;
	case 3:
		return (r >> 24) & 0xff;
	}
	return 0;
}

uint16_t DCache::readU16(uint32_t address) const
{
	const uint32_t ra = address & ~3;
	const uint32_t rb = address & 3;
	uint32_t r = m_bus->readU32(ra);
	switch(rb)
	{
	case 0:
		return r & 0xffff;
	case 2:
		return (r >> 16) & 0xffff;
	}
	return 0;	
//	return m_bus->readU16(address);
}

uint32_t DCache::readU32(uint32_t address) const
{
	return m_bus->readU32(address);
}
