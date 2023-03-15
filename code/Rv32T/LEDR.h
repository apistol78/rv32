/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include "Rv32T/Device.h"

class LEDR : public Device
{
	T_RTTI_CLASS;

public:
	virtual void eval(VSoC* soc, uint64_t time) override final;

	uint32_t get() const { return m_current; }

private:
	uint32_t m_current = 0;
};
