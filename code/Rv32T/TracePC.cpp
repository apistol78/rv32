/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
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
	if (soc->SoC__DOT__cpu__DOT__execute__DOT__trace_pc != 0 && soc->SoC__DOT__cpu__DOT__execute__DOT__trace_pc != m_lastTracePC)
	{
		//*m_os << str(L"%8d", time/2) << L", " << str(L"%08x", soc->SoC__DOT__cpu__DOT__execute__DOT__trace_pc);
		*m_os << str(L"%08x", soc->SoC__DOT__cpu__DOT__execute__DOT__trace_pc);

		for (uint32_t i = 1; i < 32; ++i)
			*m_os << str(L" %08x", soc->SoC__DOT__cpu__DOT__registers__DOT__r[i]);
		
		*m_os << Endl;
		m_lastTracePC = soc->SoC__DOT__cpu__DOT__execute__DOT__trace_pc;
	}
}
