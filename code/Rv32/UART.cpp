#include <Core/Log/Log.h>
#include "Rv32/UART.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"UART", UART, Device)

bool UART::writeU8(uint32_t address, uint8_t value)
{
	log::info << L"INVALID ACCESS, CANNOT WRITE BYTE TO UART!" << Endl;
	return false;
}

bool UART::writeU16(uint32_t address, uint16_t value)
{
	log::info << L"INVALID ACCESS, CANNOT WRITE HALF TO UART!" << Endl;
	return true;
}

bool UART::writeU32(uint32_t address, uint32_t value)
{
	log::info << wchar_t(value & 0xff);
	return true;
}

uint8_t UART::readU8(uint32_t address) const
{
	log::info << L"INVALID ACCESS, CANNOT READ BYTE FROM UART!" << Endl;
	return 0;
}

uint16_t UART::readU16(uint32_t address) const
{
	log::info << L"INVALID ACCESS, CANNOT READ HALF FROM UART!" << Endl;
	return 0;
}

uint32_t UART::readU32(uint32_t address) const
{
	return 0;
}

bool UART::tick()
{
	return true;
}
