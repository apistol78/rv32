#pragma once

#include "Rv32/Device.h"

class Unknown : public Device
{
	T_RTTI_CLASS;

public:
	explicit Unknown(const std::wstring& deviceName);

	virtual bool writeU8(uint32_t address, uint8_t value) override final;

	virtual bool writeU16(uint32_t address, uint16_t value) override final;

	virtual bool writeU32(uint32_t address, uint32_t value) override final;

	virtual uint8_t readU8(uint32_t address) const override final;

	virtual uint16_t readU16(uint32_t address) const override final;

	virtual uint32_t readU32(uint32_t address) const override final;

private:
	std::wstring m_deviceName;
};