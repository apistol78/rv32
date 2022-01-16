#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/UART_TX.h"

// Verilated SoC
#include "SoC/VSoC.h"

using namespace traktor;

void UART_TX::eval(VSoC* soc)
{
	const int32_t T = 100000000 / 115200;

	auto& tx = soc->UART_TX;

	switch (m_state)
	{
	case 0:
		{
			if (tx == 0)
			{
				m_ticks = T + T / 2;
				m_state = 1;
				m_bit = 0;
				m_data = 0;
			}
		}
		break;

	case 1:
		if (--m_ticks <= 0)
		{
			if (tx)
				m_data |= 1 << m_bit;
			m_ticks = T;
			if (++m_bit >= 8)
				m_state = 2;
		}
		break;

	case 2:
		if (--m_ticks <= 0)
			m_state = 3;
		break;

	case 3:
		if (tx == 1)
		{
			log::info << L"UART_TX " << (wchar_t)m_data << Endl;
			m_state = 0;
		}
		break;
	}
}
