#pragma once

#include <Core/Object.h>
#include <Core/Ref.h>
#include <Core/Containers/AlignedVector.h>

class Device;

class Bus : public traktor::Object
{
	T_RTTI_CLASS;

public:
	void map(uint32_t start, uint32_t end, Device* device);

	void writeU8(uint32_t address, uint8_t value);

	void writeU16(uint32_t address, uint16_t value);

	void writeU32(uint32_t address, uint32_t value);

	uint8_t readU8(uint32_t address) const;

	uint16_t readU16(uint32_t address) const;

	uint32_t readU32(uint32_t address) const;

	bool error() const { return m_error; }

private:
	struct MappedDevice
	{
		uint32_t start;
		uint32_t end;
		traktor::Ref< Device > device;
	};

	traktor::AlignedVector< MappedDevice > m_mappedDevices;
	mutable bool m_error = false;

	const MappedDevice* findMappedDevice(uint32_t address) const;
};
