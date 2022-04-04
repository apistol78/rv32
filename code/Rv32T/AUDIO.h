#pragma once

#include <Core/Ref.h>
#include <Core/Io/IStream.h>
#include "Rv32T/Device.h"

class AUDIO : public Device
{
	T_RTTI_CLASS;

public:
	AUDIO();
	
	virtual void eval(VSoC* soc, uint64_t time) override final;

	const uint32_t written() const { return m_written; }

private:
	traktor::Ref< traktor::IStream > m_stream;
	float m_value = 0.0f;
	uint32_t m_written = 0;
};

