#pragma once

#include "Rv32T/Device.h"

class LEDR : public Device
{
	T_RTTI_CLASS;

public:
	virtual void eval(VSoC* soc, uint64_t time) override final;

	uint32_t get() const { return m_current; }

private:
	uint32_t m_current = 0;
};
