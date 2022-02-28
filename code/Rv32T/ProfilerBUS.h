#pragma once

#include "Rv32T/Device.h"

class ProfilerBUS : public Device
{
	T_RTTI_CLASS;

public:
	explicit ProfilerBUS();

    virtual ~ProfilerBUS();

	virtual void eval(VSoC* soc, uint32_t time) override final;

private:
    uint32_t m_active[13];
    uint32_t m_total;
};
