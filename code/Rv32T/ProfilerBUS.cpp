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
	L"SDRAM",
	L"VRAM",
	L"UART 0",
	L"UART 1",
	L"SD",
	L"I2C",
	L"AUDIO",
	L"DMA",
	L"TIMER",
	L"PLIC",
	L"SYSREG"
};

}

ProfilerBUS::ProfilerBUS()
{
	for (int32_t i = 0; i < sizeof_array(c_names); ++i)
		m_deviceActive[i] = 0;
}

ProfilerBUS::~ProfilerBUS()
{
	for (int32_t i = 0; i < sizeof_array(c_names); ++i)
	{
		const double pl = (m_deviceActive[i] * 100.0) / m_total;
		const double pa = (m_deviceActive[i] * 100.0) / m_totalActive;
		log::info << str(L"%-10S", c_names[i]) << L": " << str(L"%6.2f", pa) << L" %  " << str(L"%6.2f", pl) << L" %." << Endl;
	}

	const double p = (m_totalActive * 100.0) / m_total;
	log::info << L"Bus active " << str(L"%.2f", p) << L" %." << Endl;
}

void ProfilerBUS::eval(VSoC* soc, uint64_t time)
{
	if (soc->o_debug_bus_request)
	{
		for (int32_t i = 0; i < sizeof_array(c_names); ++i)
		{
			const int32_t bit = (sizeof_array(c_names) - 1 - i);
			if ((soc->o_debug_bus_select & (1 << bit)) != 0)
				m_deviceActive[i]++;
		}
		m_totalActive++;
	}
	m_total++;
}
