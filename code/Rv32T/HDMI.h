/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include <Core/Ref.h>
#include <Drawing/Image.h>
#include "Rv32T/Device.h"

class HDMI : public Device
{
	T_RTTI_CLASS;

public:
	HDMI();

	virtual void eval(VSoC* soc, uint64_t time) override final;

	traktor::drawing::Image* getImage() { return m_image; }

	bool shouldDraw();

	uint32_t frameCount() const { return m_count; }

private:
	traktor::Ref< traktor::drawing::Image > m_image;
	bool m_vsync = false;
	bool m_hsync = false;
	int32_t m_hpos = 0;
	int32_t m_vpos = 0;
	bool m_clk = false;
	bool m_dirty = false;
	uint32_t m_count = 0;
};
