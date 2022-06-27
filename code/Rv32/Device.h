#pragma once

#include <Core/Object.h>

class CPU;

class Device : public traktor::Object
{
	T_RTTI_CLASS;

public:
	virtual bool writeU32(uint32_t address, uint32_t value) = 0;

	virtual uint32_t readU32(uint32_t address) const = 0;

	virtual bool tick(CPU* cpu) = 0;
};
