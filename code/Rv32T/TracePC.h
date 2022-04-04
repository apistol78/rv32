#pragma once

#include "Rv32T/Device.h"

class TracePC : public Device
{
	T_RTTI_CLASS;

public:
	explicit TracePC(traktor::OutputStream* os);

	virtual void eval(VSoC* soc, uint64_t time) override final;

private:
	traktor::Ref< traktor::OutputStream > m_os;
	uint32_t m_lastTracePC;
};
