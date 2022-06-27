#pragma once

#include <Core/Object.h>
#include <Core/Ref.h>
#include <Core/Containers/StaticVector.h>

class CPU;
class Device;

class Bus : public traktor::Object
{
	T_RTTI_CLASS;

public:
	void map(uint32_t start, uint32_t end, bool cacheable, Device* device);

	Device* device(uint32_t address) const;

	bool cacheable(uint32_t address) const;

	bool writeU32(uint32_t address, uint32_t value);

	uint32_t readU32(uint32_t address) const;

	bool tick(CPU* cpu);

	bool error() const { return m_error; }

private:
	struct MappedDevice
	{
		uint32_t start;
		uint32_t end;
		bool cacheable;
		traktor::Ref< Device > device;
	};

	traktor::StaticVector< MappedDevice, 32 > m_mappedDevices;
	mutable bool m_error = false;

	const MappedDevice* findMappedDevice(uint32_t address) const;
};
