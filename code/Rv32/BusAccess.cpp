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
	const uint64_t wt = m_nw8 + m_nw16 + m_nw32;
	const uint64_t rt = m_nr8 + m_nr16 + m_nr32;

	log::info << L"Bus access:" << Endl;
	if (wt > 0)
	{
		log::info << str(L"%-12d", m_nw8)  << L" write 8-bit  (" << str(L"%.1f%%", (m_nw8 * 100.0) / wt)  << L")" << Endl;
		log::info << str(L"%-12d", m_nw16) << L" write 16-bit (" << str(L"%.1f%%", (m_nw16 * 100.0) / wt) << L")" << Endl;
		log::info << str(L"%-12d", m_nw32) << L" write 32-bit (" << str(L"%.1f%%", (m_nw32 * 100.0) / wt) << L")" << Endl;
	}
	if (rt > 0)
	{
		log::info << str(L"%-12d", m_nr8)  << L" read 8-bit   (" << str(L"%.1f%%", (m_nr8 * 100.0) / rt)  << L")" << Endl;
		log::info << str(L"%-12d", m_nr16) << L" read 16-bit  (" << str(L"%.1f%%", (m_nr16 * 100.0) / rt) << L")" << Endl;
		log::info << str(L"%-12d", m_nr32) << L" read 32-bit  (" << str(L"%.1f%%", (m_nr32 * 100.0) / rt) << L")" << Endl;
	}

	if (!m_ua16.empty())
	{
		log::warning << L"Unaligned 16-bit reads from following PCs:" << Endl;
		for (const auto pc : m_ua16)
			log::warning << str(L"%08x", pc) << Endl;
	}
	if (!m_ua32.empty())
	{
		log::warning << L"Unaligned 32-bit reads from following PCs:" << Endl;
		for (const auto pc : m_ua32)
			log::warning << str(L"%08x", pc) << Endl;
	}
}

void BusAccess::writeU8(uint32_t pc, uint32_t address, uint8_t value)
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

void BusAccess::writeU16(uint32_t pc, uint32_t address, uint16_t value)
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
		log::error << L"Unaligned 16-bit write to " << str(L"%08x", address) << L" (PC " << str(L"%08x", pc) << L")" << Endl;
		break;
	}
	m_dcache->writeU32(wa, w);
	++m_nw16;
}

void BusAccess::writeU32(uint32_t pc, uint32_t address, uint32_t value)
{
	if ((address & 3) != 0)
		log::error << L"Unaligned 32-bit write to " << str(L"%08x", address) << L" (PC " << str(L"%08x", pc) << L")" << Endl;
	m_dcache->writeU32(address, value);
	++m_nw32;
}

uint8_t BusAccess::readU8(uint32_t pc, uint32_t address) const
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

uint16_t BusAccess::readU16(uint32_t pc, uint32_t address) const
{
	const uint32_t ra = address & ~3;
	const uint32_t rb = address & 3;

	++m_nr16;

	switch(rb)
	{
	case 0:
		{
			const uint32_t r = m_dcache->readU32(ra);
			return r & 0xffff;
		}

	case 2:
		{
			const uint32_t r = m_dcache->readU32(ra);
			return (r >> 16) & 0xffff;
		}

	default:
		{
			//log::warning << L"Unaligned 16-bit read from " << str(L"%08x", address) << L" (PC " << str(L"%08x", pc) << L")" << Endl;
			m_ua16.insert(pc);
			const uint8_t lb = readU8(pc, address);
			const uint8_t hb = readU8(pc, address + 1);
			return (hb << 8) | lb;
		}
		break;
	}

	return 0;	
}

uint32_t BusAccess::readU32(uint32_t pc, uint32_t address) const
{
	++m_nr32;

	if ((address & 3) != 0)
	{
		//log::warning << L"Unaligned 32-bit read from " << str(L"%08x", address) << L" (PC " << str(L"%08x", pc) << L")" << Endl;
		m_ua32.insert(pc);
		const uint16_t lw = readU16(pc, address);
		const uint16_t hw = readU16(pc, address + 2);
		return (hw << 16) | lw;
	}

	return m_dcache->readU32(address);
}
