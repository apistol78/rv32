#pragma once

#include <Core/Config.h>

class VSoC;

class UART_TX
{
public:
	void eval(VSoC* soc);

private:
	int32_t m_state = 0;
	int32_t m_ticks = 0;
	int32_t m_bit = 0;
	uint8_t m_data = 0;
};

