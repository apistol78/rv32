#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/TraceEXE.h"
#include "Rv32T/SoC/VSoC.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"TraceEXE", TraceEXE, Device)

TraceEXE::TraceEXE(OutputStream* os)
:   m_os(os)
,	m_lastTraceTag(~0U)
{
}

void TraceEXE::eval(VSoC* soc, uint64_t time)
{
	if (soc->SoC__DOT__execute_debug_tag != m_lastTraceTag)
	{
		*m_os << str(L"%8d", time/2) << L", " << str(L"%08x", soc->SoC__DOT__cpu__DOT__fetch__DOT__pc) << L", ";

		for (int i = 0; i < sizeof_array(soc->SoC__DOT__cpu__DOT__decode__DOT__data); ++i)
			*m_os << str(L"%08x", soc->SoC__DOT__cpu__DOT__decode__DOT__data[i]);

		*m_os << L", " << str(L"%08x", soc->SoC__DOT__cpu__DOT__forward__DOT__rs1);
		*m_os << L", " << str(L"%08x", soc->SoC__DOT__cpu__DOT__forward__DOT__rs2);
		*m_os << L", " << str(L"%08x", soc->SoC__DOT__cpu__DOT__forward__DOT__rs3);

		*m_os << Endl;
		m_lastTraceTag = soc->SoC__DOT__execute_debug_tag;
	}
}
