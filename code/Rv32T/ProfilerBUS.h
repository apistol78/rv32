/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include "Rv32T/Device.h"

class ProfilerBUS : public Device
{
	T_RTTI_CLASS;

public:
	ProfilerBUS();

    virtual ~ProfilerBUS();

	virtual void eval(VSoC* soc, uint64_t time) override final;

private:
    struct
    {
        uint32_t read;
        uint32_t write;
    }
    m_deviceActive[13];

    uint32_t m_total = 0;
    uint32_t m_totalActive = 0;
};
