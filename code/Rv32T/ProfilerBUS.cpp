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
		m_deviceActive[i] = { 0, 0 };
}

ProfilerBUS::~ProfilerBUS()
{
	for (int32_t i = 0; i < sizeof_array(c_names); ++i)
	{
		const uint32_t rwc = m_deviceActive[i].read + m_deviceActive[i].write;
		
		const double pra = (m_deviceActive[i].read * 100.0) / m_totalActive;	// percent of bus active cycles
		const double prl = (m_deviceActive[i].read * 100.0) / m_total;			// percent of runtime cycles
		const double pwa = (m_deviceActive[i].write * 100.0) / m_totalActive;	// percent of bus active cycles
		const double pwl = (m_deviceActive[i].write * 100.0) / m_total;			// percent of runtime cycles

		log::info << str(L"%-10S", c_names[i]) << L": " <<
			str(L"%6.2f", pra) << L" / " << str(L"%5.2f", pwa) << L" %  " <<
			str(L"%6.2f", prl) << L" / " << str(L"%5.2f", pwl) << L" %." << Endl;
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
			{
				if (soc->o_debug_bus_rw == 0)
					m_deviceActive[i].read++;
				else
					m_deviceActive[i].write++;
			}
		}
		m_totalActive++;
	}
	m_total++;
}
