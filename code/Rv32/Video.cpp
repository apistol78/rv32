#include <cstdio>
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include <Core/Thread/Thread.h>
#include <Core/Thread/ThreadManager.h>
#include <Drawing/Image.h>
#include <Drawing/PixelFormat.h>
#include "Rv32/Video.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"Video", Video, Device)

Video::Video()
{
	m_image = new drawing::Image(
		drawing::PixelFormat::getR8G8B8X8(),
		320,
		200
	);
	m_image->clear(Color4f(0.0f, 0.0f, 0.0f, 0.0f));
}

bool Video::writeU8(uint32_t address, uint8_t value)
{
	log::info << L"VIDEO | 8-bit " << str(L"%02x", value) << L" (" << (wchar_t)value << L")" << Endl;
	return true;
}

bool Video::writeU16(uint32_t address, uint16_t value)
{
	log::info << L"VIDEO | 16-bit " << str(L"%04x", value) << L" (" << (int32_t)value << L")" << Endl;
	return true;
}

bool Video::writeU32(uint32_t address, uint32_t value)
{
	if (address >= 0x01000000)
	{
		m_palette[(address / 4) & 255] = Color4f::fromColor4ub(
			Color4ub(value)
		);
	}
	else if (address < (320 * 200) * 4)
	{
		const uint32_t offset = address;
		const uint32_t x = offset % 320;
		const uint32_t y = offset / 320;

		m_image->setPixel(x + 3, y, m_palette[(value & 0xff000000) >> 24]);
		m_image->setPixel(x + 2, y, m_palette[(value & 0x00ff0000) >> 16]);
		m_image->setPixel(x + 1, y, m_palette[(value & 0x0000ff00) >> 8]);
		m_image->setPixel(x + 0, y, m_palette[(value & 0x000000ff)]);
	}
	return true;
}

uint8_t Video::readU8(uint32_t address) const
{
	return 0;
}

uint16_t Video::readU16(uint32_t address) const
{
	return 0;
}

uint32_t Video::readU32(uint32_t address) const
{
	return 0;
}
