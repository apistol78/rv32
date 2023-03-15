/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
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
	m_framebuffer.resize(640 * 400 * 2);
}

bool Video::writeU32(uint32_t address, uint32_t value)
{
	if ((address & 0x00f00000) == 0x00f00000)
	{
		if ((address & 0xf) == 0x0)
			m_readOffset = value;
		else if ((address & 0xf) == 0x4)
		{
			m_width = value;
			m_image = nullptr;
		}
		else if ((address & 0xf) == 0x8)
		{
			m_skip = value;
			m_image = nullptr;
		}
	}
	else if ((address & 0x00f00000) == 0x00e00000)
	{
		const uint32_t idx = (address >> 2) & 255;
		m_palette[idx] = Color4f::fromColor4ub(Color4ub(value));
	}
	else
	{
		if (address < 640 * 400 * 2)
			*(uint32_t*)&m_framebuffer[address] = value;
	}
	return true;
}

uint32_t Video::readU32(uint32_t address) const
{
	if (address < 640 * 400 * 2)
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
	const uint32_t w = (m_skip & 0b01) ? 320 : 640;
	const uint32_t h = (m_skip & 0b10) ? 200 : 400;

	if (!m_image)
	{
		m_image = new drawing::Image(
			drawing::PixelFormat::getR8G8B8X8(),
			w,
			h
		);
	}

	uint32_t offset = m_readOffset;
	for (uint32_t y = 0; y < h; ++y)
	{
		for (uint32_t x = 0; x < w; x += 4)
		{
			const uint32_t value = *(uint32_t*)&m_framebuffer[offset];
			m_image->setPixelUnsafe(x + 3, y, m_palette[(value & 0xff000000) >> 24]);
			m_image->setPixelUnsafe(x + 2, y, m_palette[(value & 0x00ff0000) >> 16]);
			m_image->setPixelUnsafe(x + 1, y, m_palette[(value & 0x0000ff00) >> 8]);
			m_image->setPixelUnsafe(x + 0, y, m_palette[(value & 0x000000ff)]);
			offset += 4;
		}
	}
	return m_image;
}
