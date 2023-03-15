/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/UART_RX.h"
#include "Rv32T/SoC/VSoC.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"UART_RX", UART_RX, Device)

UART_RX::UART_RX(IStream* target, CData& vpin)
:   m_target(target)
,	m_vpin(vpin)
{
}

void UART_RX::eval(VSoC* soc, uint64_t /*time*/)
{
	const int32_t NTICKS = 100000000 / 115200;

	auto& rx = m_vpin; // soc->UART_RX;

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
}
