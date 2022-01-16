#pragma once

#include <Core/Ref.h>
#include <Drawing/Image.h>

class VSoC;

class HDMI
{
public:
    HDMI();

	void eval(VSoC* soc);

    traktor::drawing::Image* getImage() { return m_image; }

private:
    traktor::Ref< traktor::drawing::Image > m_image;
    bool m_vsync = false;
    bool m_hsync = false;
    int32_t m_hpos = 0;
    int32_t m_vpos = 0;
    bool m_clk = false;
};
