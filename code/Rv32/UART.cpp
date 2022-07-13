#include <Core/Log/Log.h>
#include "Rv32/UART.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"UART", UART, Device)

void UART::enqueue(uint8_t b)
{
	if (m_data.full())
		m_data.pop_front();
	m_data.push_back(b);
}

bool UART::writeU32(uint32_t address, uint32_t value)
{
	log::info << wchar_t(value & 0xff);
	return true;
}

uint32_t UART::readU32(uint32_t address) const
{
	if (address == 0x00)
	{
		if (!m_data.empty())
		{
			const uint8_t b = m_data.front();
			m_data.pop_front();
			return b;
		}
		else
			return 0;
	}
	else if (address == 0x04)
	{
		return m_data.empty() ? 0x00000002 : 0x00000000;
	}
	else
		return 0;
}

bool UART::tick(CPU* cpu)
{
	return true;
}
