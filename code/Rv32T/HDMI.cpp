#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/HDMI.h"

// Verilated SoC
#include "SoC/VSoC.h"

using namespace traktor;

HDMI::HDMI()
{
	m_image = new drawing::Image(
		drawing::PixelFormat::getR8G8B8X8(),
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
        uint8_t r = (d >> 16) & 255;
        uint8_t g = (d >> 8) & 255;
        uint8_t b = d & 255;

        m_image->setPixel(m_hpos, m_vpos, Color4f::fromColor4ub(
            Color4ub(r, g, b)
        ));

        m_hpos++;
    }
    m_clk = (bool)clk;
}
