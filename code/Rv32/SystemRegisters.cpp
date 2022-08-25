#include <Core/Log/Log.h>
#include "Rv32/CPU.h"
#include "Rv32/SystemRegisters.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"SystemRegisters", SystemRegisters, Device)

SystemRegisters::SystemRegisters(uint32_t memoryAvail)
:	m_memoryAvail(memoryAvail)
,	m_rebootPending(false)
{
}

bool SystemRegisters::writeU32(uint32_t address, uint32_t value)
{
	if (address == 0x14)
		m_rebootPending = true;
	return true;
}

uint32_t SystemRegisters::readU32(uint32_t address) const
{
	switch (address)
	{
	case 0:
        return 0x00000000;
    case 1 << 2:
        return 0x00000000;
    case 2 << 2:
		return 100000000;	// frequency
	case 3 << 2:
		return 5;			// device id
	case 4 << 2:
		return m_memoryAvail;
	}
	return 0;
}

bool SystemRegisters::tick(CPU* cpu)
{
	if (m_rebootPending)
	{
		m_rebootPending = false;
		cpu->reset();
	}
	return true;
}
