#pragma once

#include <Core/Ref.h>
#include <Core/Containers/AlignedVector.h>
#include "Rv32/Device.h"

class UART : public Device
{
	T_RTTI_CLASS;

public:
	virtual bool writeU32(uint32_t address, uint32_t value) override final;

	virtual uint32_t readU32(uint32_t address) const override final;

	virtual bool tick(CPU* cpu) override final;
};