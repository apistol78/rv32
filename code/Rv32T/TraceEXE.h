#pragma once

#include "Rv32T/Device.h"

class TraceEXE : public Device
{
	T_RTTI_CLASS;

public:
    explicit TraceEXE(traktor::OutputStream* os);

	virtual void eval(VSoC* soc, uint32_t time) override final;

private:
	traktor::Ref< traktor::OutputStream > m_os;
    uint32_t m_lastTraceTag;
};
