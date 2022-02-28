#pragma once

#include "Rv32T/Device.h"

class UART_TX : public Device
{
	T_RTTI_CLASS;

public:
	virtual void eval(VSoC* soc, uint32_t time) override final;

private:
	int32_t m_state = 0;
	int32_t m_ticks = 0;
	int32_t m_bit = 0;
	uint8_t m_data = 0;
};

