#pragma once

#include <Core/Object.h>

class Device : public traktor::Object
{
	T_RTTI_CLASS;

public:
	virtual bool writeU8(uint32_t address, uint8_t value) = 0;

	virtual bool writeU16(uint32_t address, uint16_t value) = 0;

	virtual bool writeU32(uint32_t address, uint32_t value) = 0;

	virtual uint8_t readU8(uint32_t address) const = 0;

	virtual uint16_t readU16(uint32_t address) const = 0;

	virtual uint32_t readU32(uint32_t address) const = 0;

	virtual bool tick() = 0;
};
