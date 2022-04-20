#pragma once

#include <Core/Ref.h>
#include <Core/Containers/AlignedVector.h>
#include "Rv32/Device.h"

class Bus : public Device
{
	T_RTTI_CLASS;

public:
	void map(uint32_t start, uint32_t end, Device* device);

	Device* device(uint32_t address) const;

	virtual bool writeU8(uint32_t address, uint8_t value) override final;

	virtual bool writeU16(uint32_t address, uint16_t value) override final;

	virtual bool writeU32(uint32_t address, uint32_t value) override final;

	virtual uint8_t readU8(uint32_t address) const override final;

	virtual uint16_t readU16(uint32_t address) const override final;

	virtual uint32_t readU32(uint32_t address) const override final;

	virtual bool tick(CPU* cpu) override final;

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
