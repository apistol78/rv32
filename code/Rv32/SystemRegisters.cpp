#include <Core/Log/Log.h>
#include "Rv32/SystemRegisters.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"SystemRegisters", SystemRegisters, Device)

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
	case 0x00:
        return 0x00000000;
    case 0x04:
        return 0x00000000;
    case 0x08:
		return 100000000;	// frequency
	case 0x0c:
		return 5;			// device id
	}
	return 0;
}

bool SystemRegisters::tick(CPU* cpu)
{
	return true;
}
