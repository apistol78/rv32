#include <Core/Log/Log.h>
#include "Rv32/CPU.h"
#include "Rv32/Timer.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"TimerD", TimerD, Device)

bool TimerD::writeU32(uint32_t address, uint32_t value)
{
	return true;
}

uint32_t TimerD::readU32(uint32_t address) const
{
	switch (address)
	{
	case 0x00:
		return (uint32_t)(m_timer.getElapsedTime() * 1000.0);	// milliseconds
	case 0x04:
		return (uint32_t)m_tick;	// cycles low
	case 0x08:
		return (uint32_t)(m_tick >> 32);	// cycles high
	case 0x0c:
		return (uint32_t)m_compare;	// compare low
	case 0x10:
		return (uint32_t)(m_compare >> 32);	// compare high
	}
	return 0;
}

bool TimerD::tick(CPU* cpu)
{
	if (++m_tick == m_compare)
	{
		log::info << L"Timer compare interrupt" << Endl;
		cpu->interrupt();
	}
	return true;
}
