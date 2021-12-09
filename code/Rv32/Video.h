#pragma once

#include <Core/Ref.h>
#include <Core/Misc/AutoPtr.h>
#include "Rv32/Device.h"

namespace traktor::drawing
{
	class Image;
}

class Video : public Device
{
	T_RTTI_CLASS;

public:
	Video();

	virtual void writeU8(uint32_t address, uint8_t value) override final;

	virtual void writeU16(uint32_t address, uint16_t value) override final;

	virtual void writeU32(uint32_t address, uint32_t value) override final;

	virtual uint8_t readU8(uint32_t address) const override final;

	virtual uint16_t readU16(uint32_t address) const override final;

	virtual uint32_t readU32(uint32_t address) const override final;

	traktor::drawing::Image* getImage() const { return m_image; }

	bool shouldPresent() { bool dirty = m_dirty; m_dirty = false; return dirty; }

private:
	traktor::Ref< traktor::drawing::Image > m_image;
	bool m_dirty = true;
};