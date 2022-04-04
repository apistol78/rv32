#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/TracePC.h"
#include "Rv32T/SoC/VSoC.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"TracePC", TracePC, Device)

TracePC::TracePC(OutputStream* os)
:   m_os(os)
,	m_lastTracePC(~0U)
{
}

void TracePC::eval(VSoC* soc, uint64_t time)
{
	if (soc->SoC__DOT__cpu__DOT__fetch__DOT__pc != m_lastTracePC)
	{
		*m_os << str(L"%8d", time/2) << L", " << str(L"%08x", soc->SoC__DOT__cpu__DOT__fetch__DOT__pc);

		for (uint32_t i = 0; i < 64; ++i)
			*m_os << str(L", %08x", soc->SoC__DOT__cpu__DOT__registers__DOT__r[i]);
		
		*m_os << Endl;
		m_lastTracePC = soc->SoC__DOT__cpu__DOT__fetch__DOT__pc;
	}
}
