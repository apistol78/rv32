#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/UART_RX.h"
#include "Rv32T/SoC/VSoC.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"UART_RX", UART_RX, Device)

UART_RX::UART_RX(IStream* target)
:   m_target(target)
{
}

void UART_RX::eval(VSoC* soc, uint64_t /*time*/)
{
	const int32_t NTICKS = 100000000 / 115200;

	auto& rx = soc->UART_RX;

	switch (m_state)
	{
	case 0:		// Wait for data from stream.
		{
			if (m_target->available() > 0)
			{
				m_target->read(&m_data, 1);
				m_bit = 0;
				m_ticks = 0;
				m_state = 1;
				log::info << L"[RX] Got byte " << str(L"%02x", m_data) << L" from UDP..." << Endl;
			}
		}
		break;

	case 1:		// Start bit.
		{
			rx = 0;
			if (m_ticks++ >= NTICKS)
			{
				log::info << L"[RX] Start bit finished, data bit 8" << Endl;
				m_bit = 0;
				m_ticks = 0;
				m_state = 2;
			}
		}
		break;

	case 2:		// Data bit
		{
			rx = (m_data & 0x80) ? 1 : 0;
			if (m_ticks++ >= NTICKS)
			{
				if (++m_bit >= 8)
				{
					log::info << L"[RX] Stop bit" << Endl;
					m_ticks = 0;
					m_state = 3;
				}
				else
				{
					log::info << L"[RX] Data bit " << (7 - m_bit) << Endl;
					m_data >= 1;
				}
			}
		}
		break;

	case 3:		// Stop bit
		{
			rx = 1;
			if (m_ticks++ >= NTICKS)
			{
				log::info << L"[RX] Finished, back to idle" << Endl;
				m_bit = 0;
				m_ticks = 0;
				m_state = 0;
			}
		}
	}

	// switch (m_state)
	// {
	// case 0:
	// 	{
	// 		if (tx == 0)
	// 		{
	// 			m_ticks = T + T / 2;
	// 			m_state = 1;
	// 			m_bit = 0;
	// 			m_data = 0;
	// 		}
	// 	}
	// 	break;

	// case 1:
	// 	if (--m_ticks <= 0)
	// 	{
	// 		if (tx)
	// 			m_data |= 1 << m_bit;
	// 		m_ticks = T;
	// 		if (++m_bit >= 8)
	// 			m_state = 2;
	// 	}
	// 	break;

	// case 2:
	// 	if (--m_ticks <= 0)
	// 		m_state = 3;
	// 	break;

	// case 3:
	// 	if (tx == 1)
	// 	{
	// 		if (!iscntrl(m_data))
	// 			log::info << (wchar_t)m_data;
	// 		else
	// 		{
	// 			if (m_data != 10 && m_data != 13)
	// 				log::info << L"<" << (int32_t)m_data << L">";

	// 			if (m_data == '\n')
	// 				log::info << Endl;
	// 		}

	// 		m_state = 0;
	// 	}
	// 	break;
	// }
}
