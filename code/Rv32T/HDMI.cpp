#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/HDMI.h"

// Verilated SoC
#include "SoC/VSoC.h"

using namespace traktor;

HDMI::HDMI()
{
	m_image = new drawing::Image(
		drawing::PixelFormat::getX8R8G8B8(),
		640,
		480
	);
	m_image->clear(Color4f(0.0f, 0.0f, 0.0f, 0.0f)); 
}

void HDMI::eval(VSoC* soc)
{
	auto& clk = soc->HDMI_TX_CLK;
	auto& de = soc->HDMI_TX_DE;
	auto& hs = soc->HDMI_TX_HS;
	auto& txi = soc->HDMI_TX_INT;
	auto& vs = soc->HDMI_TX_VS;
	auto& d = soc->HDMI_TX_D;

	if (vs == 0 && m_vsync == false)
	{
		m_dirty = true;
		m_vsync = true;
		m_vpos = 0;
	}
	else if (vs != 0 && m_vsync == true)
	{
		m_vsync = false;
	}

	if (hs == 0 && m_hsync == false)
	{
		m_hsync = true;
		m_hpos = 0;
		m_vpos++;
	}
	else if (hs != 0 && m_hsync == true)
	{
		m_hsync = false;
	}

	if (!m_clk && clk)
	{
		const int32_t x = m_hpos - 144;
		const int32_t y = m_vpos;
		if (x >= 0 && x < 640 && y >= 0 && y < 480)
		{
			uint32_t* id = (uint32_t*)m_image->getData();
			id[x + y * 640] = d;
			// m_dirty = true;
		}
		m_hpos++;
	}
	m_clk = (bool)clk;
}

bool HDMI::shouldDraw()
{
	const bool dirty = m_dirty;
	m_dirty = false;
	return dirty;
}