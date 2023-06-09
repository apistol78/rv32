/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/HDMI.h"
#include "Rv32T/SoC/VSoC.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"HDMI", HDMI, Device)

HDMI::HDMI()
{
	m_image = new drawing::Image(
		drawing::PixelFormat::getX8R8G8B8(),
		640,
		400
	);
	m_image->clear(Color4f(0.0f, 0.0f, 0.0f, 0.0f)); 
}

void HDMI::eval(VSoC* soc, uint64_t /*time*/)
{
	const bool clk = (soc->HDMI_TX_CLK != 0);
	const bool de = (soc->HDMI_TX_DE != 0);
	const bool hs = (soc->HDMI_TX_HS == 0);
	const bool vs = (soc->HDMI_TX_VS != 0);
	const auto& d = soc->HDMI_TX_D;

	if (vs && !m_vsync)
	{
		m_vpos = 0;
		m_dirty = true;
		m_count++;
	}

	if (hs && !m_hsync)
	{
		m_hpos = 0;
		m_vpos++;
	}

	if (clk && !m_clk)
	{
		const int32_t x = m_hpos - (48 + 96);	// back + pulse
		const int32_t y = m_vpos - (35 + 2);
		if (de && x >= 0 && x < m_image->getWidth() && y >= 0 && y < m_image->getHeight())
		{
			uint32_t* id = (uint32_t*)m_image->getData();
			id[x + y * m_image->getWidth()] = d;
		}
		m_hpos++;
	}

	m_clk = (bool)clk;
	m_vsync = vs;
	m_hsync = hs;
}

bool HDMI::shouldDraw()
{
	const bool dirty = m_dirty;
	m_dirty = false;
	return dirty;
}