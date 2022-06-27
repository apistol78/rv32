#pragma once

#include <Core/Ref.h>
#include <Core/Math/Color4f.h>
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

	virtual bool writeU32(uint32_t address, uint32_t value) override final;

	virtual uint32_t readU32(uint32_t address) const override final;

	virtual bool tick(CPU* cpu) override final;

	traktor::drawing::Image* getImage() const { return m_image; }

private:
	traktor::Ref< traktor::drawing::Image > m_image;
	traktor::Color4f m_palette[256];
};