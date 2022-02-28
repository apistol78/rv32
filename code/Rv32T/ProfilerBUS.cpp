#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/ProfilerBUS.h"
#include "Rv32T/SoC/VSoC.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"ProfilerBUS", ProfilerBUS, Device)

namespace
{

const wchar_t* c_names[] =
{
	L"ROM",
	L"RAM",
	L"SDRAM",
	L"VRAM",
	L"LED",
	L"UART 0",
	L"UART 1",
	L"GPIO",
	L"SD",
	L"I2C",
	L"DMA",
	L"TIMER",
	L"PLIC"
};

}

ProfilerBUS::ProfilerBUS()
:   m_total(0)
{
	for (int i = 0; i < 13; ++i)
		m_active[i] = 0;
}

ProfilerBUS::~ProfilerBUS()
{
	for (int i = 0; i < 13; ++i)
	{
		double p = (m_active[i] * 100.0) / m_total;
		log::info << str(L"%-7S", c_names[i]) << L": " << str(L"%.2f", p) << L"%" << Endl;
	}
}

void ProfilerBUS::eval(VSoC* soc, uint32_t time)
{
	if (soc->SoC__DOT__debug_request)
	{
		for (int i = 0; i < 13; ++i)
		{
			if ((soc->SoC__DOT__debug_select & (1 << (12-i))) != 0)
				m_active[i]++;
		}
	}
	++m_total;
}
