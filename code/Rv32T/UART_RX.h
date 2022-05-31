#pragma once

#include "verilated.h"

#include <Core/Ref.h>
#include <Core/Io/IStream.h>
#include "Rv32T/Device.h"

class UART_RX : public Device
{
	T_RTTI_CLASS;

public:
    explicit UART_RX(traktor::IStream* target, CData& vpin);

	virtual void eval(VSoC* soc, uint64_t time) override final;

private:
    traktor::Ref< traktor::IStream > m_target;
	CData& m_vpin;
	int32_t m_state = 0;
	int32_t m_ticks = 0;
	int32_t m_bit = 0;
	uint8_t m_data = 0;
};

