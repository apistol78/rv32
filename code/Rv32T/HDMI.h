#pragma once

#include <Core/Ref.h>
#include <Drawing/Image.h>
#include "Rv32T/Device.h"

class HDMI : public Device
{
	T_RTTI_CLASS;

public:
	HDMI();

	virtual void eval(VSoC* soc, uint32_t time) override final;

	traktor::drawing::Image* getImage() { return m_image; }

	bool shouldDraw();

private:
	traktor::Ref< traktor::drawing::Image > m_image;
	bool m_vsync = false;
	bool m_hsync = false;
	int32_t m_hpos = 0;
	int32_t m_vpos = 0;
	bool m_clk = false;
	bool m_dirty = false;
};
