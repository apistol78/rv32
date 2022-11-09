#pragma once

#include "Rv32/Device.h"

class SystemRegisters : public Device
{
	T_RTTI_CLASS;

public:
	explicit SystemRegisters(uint32_t memoryAvail);

	virtual bool writeU32(uint32_t address, uint32_t value) override final;

	virtual uint32_t readU32(uint32_t address) const override final;

	virtual bool tick(CPU* cpu) override final;

private:
	uint32_t m_memoryAvail;
	uint32_t m_user;
	bool m_rebootPending;
};
