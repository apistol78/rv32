#pragma once

#include <Core/Ref.h>
#include <Core/Containers/CircularVector.h>
#include "Rv32/Device.h"

class UART : public Device
{
	T_RTTI_CLASS;

public:
	void enqueue(uint8_t b);

	virtual bool writeU32(uint32_t address, uint32_t value) override final;

	virtual uint32_t readU32(uint32_t address) const override final;

	virtual bool tick(CPU* cpu) override final;

private:
	mutable traktor::CircularVector< uint8_t, 1024 > m_data;
};