#pragma once

#include <Core/Ref.h>
#include <Core/Misc/AutoPtr.h>
#include "Rv32/Device.h"

namespace traktor
{
	namespace drawing
	{

		class Image;

	}
}

class Video : public Device
{
	T_RTTI_CLASS;

public:
	Video();

	virtual bool writeU8(uint32_t address, uint8_t value) override final;

	virtual bool writeU16(uint32_t address, uint16_t value) override final;

	virtual bool writeU32(uint32_t address, uint32_t value) override final;

	virtual uint8_t readU8(uint32_t address) const override final;

	virtual uint16_t readU16(uint32_t address) const override final;

	virtual uint32_t readU32(uint32_t address) const override final;

	traktor::drawing::Image* getImage() const { return m_image; }

private:
	traktor::Ref< traktor::drawing::Image > m_image;
};