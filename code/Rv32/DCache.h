#pragma once

#include <Core/Object.h>
#include <Core/Ref.h>

class Bus;

class DCache : public traktor::Object
{
	T_RTTI_CLASS;

public:
	explicit DCache(Bus* bus);

	void writeU8(uint32_t address, uint8_t value);

	void writeU16(uint32_t address, uint16_t value);

	void writeU32(uint32_t address, uint32_t value);

	uint8_t readU8(uint32_t address) const;

	uint16_t readU16(uint32_t address) const;

	uint32_t readU32(uint32_t address) const;

private:
	traktor::Ref< Bus > m_bus;
};
