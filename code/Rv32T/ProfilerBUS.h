#pragma once

#include "Rv32T/Device.h"

class ProfilerBUS : public Device
{
	T_RTTI_CLASS;

public:
	ProfilerBUS();

    virtual ~ProfilerBUS();

	virtual void eval(VSoC* soc, uint64_t time) override final;

private:
    struct
    {
        uint32_t read;
        uint32_t write;
    }
    m_deviceActive[13];

    uint32_t m_total = 0;
    uint32_t m_totalActive = 0;
};
