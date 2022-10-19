#pragma once

#include <Core/Timer/Timer.h>
#include "Rv32/Device.h"

class TimerD : public Device
{
	T_RTTI_CLASS;

public:
	virtual bool writeU32(uint32_t address, uint32_t value) override final;

	virtual uint32_t readU32(uint32_t address) const override final;

	virtual bool tick(CPU* cpu) override final;

private:
	traktor::Timer m_timer;
	uint64_t m_cycles = 0;
	uint64_t m_compare = 0;
	uint32_t m_ms = 0;
	uint32_t m_countdown = 0;
};