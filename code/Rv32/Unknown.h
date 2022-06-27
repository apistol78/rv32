#pragma once

#include "Rv32/Device.h"

class Unknown : public Device
{
	T_RTTI_CLASS;

public:
	explicit Unknown(const std::wstring& deviceName, bool mute);

	virtual bool writeU32(uint32_t address, uint32_t value) override final;

	virtual uint32_t readU32(uint32_t address) const override final;

	virtual bool tick(CPU* cpu) override final;

private:
	std::wstring m_deviceName;
	bool m_mute;
};