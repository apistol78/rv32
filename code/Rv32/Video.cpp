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
		640,
		400
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
	if (address == 0x00000100)
		ThreadManager::getInstance().getCurrentThread()->sleep(value);
	else if (address == 0x00000200)
		m_dirty = true;
	else if (address >= 0x10000000)
	{
		uint32_t offset = address - 0x10000000;

		uint32_t x = offset % 640;
		uint32_t y = offset / 640;

		m_image->setPixel(x, y, Color4f::fromColor4ub(
			Color4ub(value)
		));
	}
	else
		log::info << L"VIDEO | 32-bit 0x" << str(L"%08x", value) << L" (" << (int32_t)value << L")" << Endl;
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
	if (address == 0x00000100)
		return (uint32_t)(m_timer.getElapsedTime() * 1000.0);
	else
		return 0;
}
