#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/UART_TX.h"
#include "Rv32T/SoC/VSoC.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"UART_TX", UART_TX, Device)

UART_TX::UART_TX(CData& vpin)
:	m_vpin(vpin)
{
}

void UART_TX::eval(VSoC* soc, uint64_t /*time*/)
{
	const int32_t T = 100000000 / 115200;

	auto& tx = m_vpin; // soc->UART_TX;

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
			if (!iscntrl(m_data))
				log::info << (wchar_t)m_data;
			else
			{
				if (m_data != 10 && m_data != 13)
					log::info << L"<" << (int32_t)m_data << L">";

				if (m_data == '\n')
					log::info << Endl;
			}

			m_state = 0;
		}
		break;
	}
}
