#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/TraceWB.h"
#include "Rv32T/SoC/VSoC.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"TraceWB", TraceWB, Device)

TraceWB::TraceWB(OutputStream* os)
:   m_os(os)
,	m_lastTraceTag(~0U)
{
}

void TraceWB::eval(VSoC* soc, uint64_t time)
{
	if (soc->SoC__DOT__writeback_debug_tag != m_lastTraceTag)
	{
		*m_os << str(L"%8d", time/2) << L", " << str(L"%08x", soc->SoC__DOT__cpu__DOT__fetch__DOT__pc) << L", ";
		*m_os << str(L"%016x", soc->SoC__DOT__cpu__DOT__memory__DOT__data);
		*m_os << Endl;
		m_lastTraceTag = soc->SoC__DOT__writeback_debug_tag;
	}
}
