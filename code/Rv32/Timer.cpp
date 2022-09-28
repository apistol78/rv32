#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/CPU.h"
#include "Rv32/Timer.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"TimerD", TimerD, Device)

bool TimerD::writeU32(uint32_t address, uint32_t value)
{
	switch (address >> 2)
	{
	case 0x9:
		m_compare = (m_compare & 0xffffffff00000000) | value;
		break;
	case 0xa:
		m_compare = (m_compare & 0x00000000ffffffff) | ((uint64_t)value << 32ULL);
		break;
	case 0xe:
		m_enabled = (uint8_t)value;
		break;
	case 0xf:
		m_raised = true;
		break;
	default:
		log::info << L"Invalid timer address " << str(L"%08x", address) << L", write" << Endl;
	}
	return true;
}

uint32_t TimerD::readU32(uint32_t address) const
{
	switch (address >> 2)
	{
	case 0x0:
		//return (uint32_t)(m_timer.getElapsedTime() * 1000.0);	// milliseconds
		return (uint32_t)(m_time * 1000.0);
	case 0x1:
		return (uint32_t)m_cycles[0];			// cycles low
	case 0x2:
		return (uint32_t)(m_cycles[0] >> 32);	// cycles high
	case 0x3:
		return (uint32_t)m_cycles[1];			// cycles low
	case 0x4:
		return (uint32_t)(m_cycles[1] >> 32);	// cycles high
	case 0x5:
		return (uint32_t)m_cycles[2];			// cycles low
	case 0x6:
		return (uint32_t)(m_cycles[2] >> 32);	// cycles high
	case 0x7:
		return (uint32_t)m_cycles[3];			// cycles low
	case 0x8:
		return (uint32_t)(m_cycles[3] >> 32);	// cycles high
	case 0x9:
		return (uint32_t)m_compare;				// compare low
	case 0xa:
		return (uint32_t)(m_compare >> 32);		// compare high
	case 0xe:
		return m_enabled;
	default:
		log::info << L"Invalid timer address " << str(L"%08x", address) << L", read" << Endl;
	}
	return 0;
}

bool TimerD::tick(CPU* cpu)
{
	m_time += 1.0 / (100 * 1e6);

	if (m_enabled & 0x1)
		m_cycles[0]++;
	if (m_enabled & 0x2)
		m_cycles[1]++;
	if (m_enabled & 0x4)
		m_cycles[2]++;
	if (m_enabled & 0x8)
		m_cycles[3]++;

	if (m_raised || ((m_enabled & 0x1) && (m_cycles[0] == m_compare)))
		cpu->interrupt();

	m_raised = false;
	return true;
}
