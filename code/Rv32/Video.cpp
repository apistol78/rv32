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
:	m_readOffset(0)
{
	m_image = new drawing::Image(
		drawing::PixelFormat::getR8G8B8X8(),
		320,
		200
	);
	m_image->clear(Color4f(0.0f, 0.0f, 0.0f, 0.0f));

	m_framebuffer.resize(320 * 400);
}

bool Video::writeU32(uint32_t address, uint32_t value)
{
	if (address < 0x00800000)
		*(uint32_t*)&m_framebuffer[address] = value;
	else if (address < 0x00800400)
		m_palette[(address / 4) & 255] = Color4f::fromColor4ub(Color4ub(value));
	else
		m_readOffset = value;

	return true;
}

uint32_t Video::readU32(uint32_t address) const
{
	if (address < 0x00800000)
		return *(uint32_t*)&m_framebuffer[address];
	else
		return 0;
}

bool Video::tick(CPU* cpu)
{
	return true;
}

drawing::Image* Video::getImage()
{
	for (uint32_t y = 0; y < 200; ++y)
	{
		for (uint32_t x = 0; x < 320; x += 4)
		{
			const uint32_t offset = m_readOffset + x + y * 320;
			const uint32_t value = *(uint32_t*)&m_framebuffer[offset];
			m_image->setPixelUnsafe(x + 3, y, m_palette[(value & 0xff000000) >> 24]);
			m_image->setPixelUnsafe(x + 2, y, m_palette[(value & 0x00ff0000) >> 16]);
			m_image->setPixelUnsafe(x + 1, y, m_palette[(value & 0x0000ff00) >> 8]);
			m_image->setPixelUnsafe(x + 0, y, m_palette[(value & 0x000000ff)]);
		}
	}
	return m_image;
}
