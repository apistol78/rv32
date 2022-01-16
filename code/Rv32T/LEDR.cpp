#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/LEDR.h"

// Verilated SoC
#include "SoC/VSoC.h"

using namespace traktor;

void LEDR::eval(VSoC* soc)
{
	if (soc->LEDR != m_current)
		log::info << L"LEDR: " << str(L"%08x", soc->LEDR) << Endl;
	m_current = soc->LEDR;
}
