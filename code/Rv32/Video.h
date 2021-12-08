#pragma once

#include <Core/Misc/AutoPtr.h>
#include "Rv32/Device.h"

class Video : public Device
{
	T_RTTI_CLASS;

public:
	virtual void writeU8(uint32_t address, uint8_t value) override final;

	virtual void writeU32(uint32_t address, uint32_t value) override final;

	virtual uint8_t readU8(uint32_t address) const override final;

	virtual uint32_t readU32(uint32_t address) const override final;
};