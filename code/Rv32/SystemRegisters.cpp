#include <Core/Log/Log.h>
#include "Rv32/SystemRegisters.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"SystemRegisters", SystemRegisters, Device)

SystemRegisters::SystemRegisters(uint32_t memoryAvail)
:	m_memoryAvail(memoryAvail)
{
}

bool SystemRegisters::writeU8(uint32_t address, uint8_t value)
{
	return true;
}

bool SystemRegisters::writeU16(uint32_t address, uint16_t value)
{
	return true;
}

bool SystemRegisters::writeU32(uint32_t address, uint32_t value)
{
	return true;
}

uint8_t SystemRegisters::readU8(uint32_t address) const
{
	return 0;
}

uint16_t SystemRegisters::readU16(uint32_t address) const
{
	return 0;
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
	return true;
}
