#pragma once

#include "Rv32/Device.h"

class UART : public Device
{
	T_RTTI_CLASS;

public:
	virtual bool writeU8(uint32_t address, uint8_t value) override final;

	virtual bool writeU16(uint32_t address, uint16_t value) override final;

	virtual bool writeU32(uint32_t address, uint32_t value) override final;

	virtual uint8_t readU8(uint32_t address) const override final;

	virtual uint16_t readU16(uint32_t address) const override final;

	virtual uint32_t readU32(uint32_t address) const override final;

private:
    mutable uint32_t m_count = 0;
};