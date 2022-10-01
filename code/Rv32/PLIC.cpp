#include <Core/Log/Log.h>
#include "Rv32/PLIC.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"PLIC", PLIC, Device)

bool PLIC::writeU32(uint32_t address, uint32_t value)
{
	return true;
}

uint32_t PLIC::readU32(uint32_t address) const
{
	if (address == 0x00200004)
	{
		log::info << L"Claim context" << Endl;
	}
	return 0;
}

bool PLIC::tick(CPU* cpu)
{
	return true;
}
