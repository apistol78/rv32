#pragma once

#include <Core/Misc/AutoPtr.h>
#include "Rv32/Device.h"

class Memory : public Device
{
	T_RTTI_CLASS;

public:
	explicit Memory(uint32_t capacity);

	void setReadOnly(bool readOnly);

	virtual bool writeU8(uint32_t address, uint8_t value) override final;

	virtual bool writeU16(uint32_t address, uint16_t value) override final;

	virtual bool writeU32(uint32_t address, uint32_t value) override final;

	virtual uint8_t readU8(uint32_t address) const override final;

	virtual uint16_t readU16(uint32_t address) const override final;

	virtual uint32_t readU32(uint32_t address) const override final;

	virtual bool tick(CPU* cpu) override final;

private:
	traktor::AutoArrayPtr< uint8_t > m_data;
	uint32_t m_capacity;
	bool m_readOnly = false;
};