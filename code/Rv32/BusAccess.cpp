#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/BusAccess.h"
#include "Rv32/DCache.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"BusAccess", BusAccess, Object)

BusAccess::BusAccess(DCache* dcache)
:	m_dcache(dcache)
{
}

BusAccess::~BusAccess()
{
	uint64_t wt = m_nw8 + m_nw16 + m_nw32;
	uint64_t rt = m_nr8 + m_nr16 + m_nr32;

	log::info << L"Bus access:" << Endl;
	log::info << str(L"%-12d", m_nw8)  << L" write 8-bit  (" << str(L"%.1f%%", (m_nw8 * 100.0) / wt)  << L")" << Endl;
	log::info << str(L"%-12d", m_nw16) << L" write 16-bit (" << str(L"%.1f%%", (m_nw16 * 100.0) / wt) << L")" << Endl;
	log::info << str(L"%-12d", m_nw32) << L" write 32-bit (" << str(L"%.1f%%", (m_nw32 * 100.0) / wt) << L")" << Endl;
	log::info << str(L"%-12d", m_nr8)  << L" read 8-bit   (" << str(L"%.1f%%", (m_nr8 * 100.0) / rt)  << L")" << Endl;
	log::info << str(L"%-12d", m_nr16) << L" read 16-bit  (" << str(L"%.1f%%", (m_nr16 * 100.0) / rt) << L")" << Endl;
	log::info << str(L"%-12d", m_nr32) << L" read 32-bit  (" << str(L"%.1f%%", (m_nr32 * 100.0) / rt) << L")" << Endl;
}

void BusAccess::writeU8(uint32_t address, uint8_t value)
{
	const uint32_t wa = address & ~3;
	const uint32_t wb = address & 3;
	uint32_t w = m_dcache->readU32(wa);
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
	m_dcache->writeU32(wa, w);
	++m_nw8;
}

void BusAccess::writeU16(uint32_t address, uint16_t value)
{
	const uint32_t wa = address & ~3;
	const uint32_t wb = address & 3;
	uint32_t w = m_dcache->readU32(wa);
	switch(wb)
	{
	case 0:
		w = (w & 0xffff0000) | value;
		break;
	case 2:
		w = (w & 0x0000ffff) | (value << 16);
		break;
	default:
		log::error << L"Unaligned 16-bit write to " << str(L"%08x", address) << Endl;
		break;
	}
	m_dcache->writeU32(wa, w);
	++m_nw16;
}

void BusAccess::writeU32(uint32_t address, uint32_t value)
{
	if ((address & 3) != 0)
		log::error << L"Unaligned 32-bit write to " << str(L"%08x", address) << Endl;
	m_dcache->writeU32(address, value);
	++m_nw32;
}

uint8_t BusAccess::readU8(uint32_t address) const
{
	const uint32_t ra = address & ~3;
	const uint32_t rb = address & 3;

	const uint32_t r = m_dcache->readU32(ra);
	++m_nr8;

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

uint16_t BusAccess::readU16(uint32_t address) const
{
	const uint32_t ra = address & ~3;
	const uint32_t rb = address & 3;

	const uint32_t r = m_dcache->readU32(ra);
	++m_nr16;

	switch(rb)
	{
	case 0:
		return r & 0xffff;
	case 2:
		return (r >> 16) & 0xffff;
	default:
		log::error << L"Unaligned 16-bit read from " << str(L"%08x", address) << Endl;
		break;
	}
	return 0;	
}

uint32_t BusAccess::readU32(uint32_t address) const
{
	if ((address & 3) != 0)
		log::error << L"Unaligned 32-bit read from " << str(L"%08x", address) << Endl;

	const uint32_t r = m_dcache->readU32(address);
	++m_nr32;

	return r;
}
