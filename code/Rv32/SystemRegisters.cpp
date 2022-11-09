#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/CPU.h"
#include "Rv32/SystemRegisters.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"SystemRegisters", SystemRegisters, Device)

SystemRegisters::SystemRegisters(uint32_t memoryAvail)
:	m_memoryAvail(memoryAvail)
,	m_user(0)
,	m_rebootPending(false)
{
}

bool SystemRegisters::writeU32(uint32_t address, uint32_t value)
{
	switch (address >> 2)
	{
	// flags
	case 0:
		m_rebootPending = ((value & 0b0100) != 0);
		break;

	// leds
	case 1:
		break;

	// frequency
	case 2:
		break;

	// device id
	case 3:
		break;

	// ram size
	case 4:
		break;

	// user
	case 5:
		m_user = value;
		break;

	default:
		log::info << L"[SYSREG] Unimplemented write to " << str(L"%08x", address) << Endl;
	}
	return true;
}

uint32_t SystemRegisters::readU32(uint32_t address) const
{
	switch (address >> 2)
	{
	// flags
	case 0:
        return 0x00000000;

	// leds
    case 1:
        return 0x00000000;

	// frequency
    case 2:
		return 100000000;
	
	// device id
	case 3:
		return 5;

	// ram size
	case 4:
		return m_memoryAvail;

	// user
	case 5:
		return m_user;

	default:
		log::info << L"[SYSREG] Unimplemented read from " << str(L"%08x", address) << Endl;
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
