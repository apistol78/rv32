#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/LEDR.h"
#include "Rv32T/SoC/VSoC.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"LEDR", LEDR, Device)

void LEDR::eval(VSoC* soc, uint64_t /*time*/)
{
	if (soc->LEDR != m_current)
		log::info << L"LEDR: " << str(L"%08x", soc->LEDR) << Endl;
	m_current = soc->LEDR;
}
