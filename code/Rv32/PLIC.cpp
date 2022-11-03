#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/CPU.h"
#include "Rv32/PLIC.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"PLIC", PLIC, Device)

bool PLIC::writeU32(uint32_t address, uint32_t value)
{
	log::info << L"PLIC write " << str(L"%08x", address) << L", value " << value << Endl;
	if (address == 0x00002000)
	{
		log::info << L"[PLIC] Set enable bits" << Endl;
	}
	else if (address == 0x00200004)
	{
		log::info << L"[PLIC] Complete context" << Endl;
	}	
	return true;
}

uint32_t PLIC::readU32(uint32_t address) const
{
	log::info << L"PLIC read " << str(L"%08x", address) << Endl;
	if (address == 0x00200004)
	{
		log::info << L"[PLIC] Claim context" << Endl;
	}
	return 0;
}

bool PLIC::tick(CPU* cpu)
{
	if (m_interrupt)
	{
		cpu->interrupt(EXTERNAL);
		m_interrupt = false;
	}
	return true;
}

void PLIC::raise(uint32_t channel)
{
	// log::info << L"[PLIC] Raise " << channel << Endl;
	m_interrupt = true;
}
