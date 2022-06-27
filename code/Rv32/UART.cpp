#include <Core/Log/Log.h>
#include "Rv32/UART.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"UART", UART, Device)

bool UART::writeU32(uint32_t address, uint32_t value)
{
	log::info << wchar_t(value & 0xff);
	return true;
}

uint32_t UART::readU32(uint32_t address) const
{
	if (address == 0x04)
	{
		// Status empty.
		return 0x00000002;
	}
	return 0;
}

bool UART::tick(CPU* cpu)
{
	return true;
}
