#include <cstdio>

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include <Core/Log/Log.h>
#include <Core/Misc/String.h>

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

void Video::writeU8(uint32_t address, uint8_t value)
{
	//log::info << L"VIDEO | " << str(L"%02x", value) << L" (" << (int32_t)value << L")" << Endl;
	log::info << (wchar_t)value;
}

void Video::writeU16(uint32_t address, uint16_t value)
{
	log::info << L"VIDEO | " << str(L"%04x", value) << L" (" << (int32_t)value << L")" << Endl;
}

void Video::writeU32(uint32_t address, uint32_t value)
{
	if (address == 0x00000100)
		usleep(value * 1000);
	else if (address == 0x00000200)
	{
		m_image->save(L"Capture.png");
		log::info << L"Framebuffer captured." << Endl;
	}
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
		log::info << L"VIDEO | " << str(L"%08x", value) << L" (" << (int32_t)value << L")" << Endl;
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
	{
		struct timeval tp;
		struct timezone tzp;
		gettimeofday(&tp, &tzp);
		return (tp.tv_sec * 1000) + (tp.tv_usec / 1000);        
	}
	else
		return 0;
}
