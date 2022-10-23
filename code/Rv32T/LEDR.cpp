#include "Rv32T/LEDR.h"
#include "Rv32T/SoC/VSoC.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"LEDR", LEDR, Device)

void LEDR::eval(VSoC* soc, uint64_t /*time*/)
{
	m_current = soc->LEDR;
}
