/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include "verilated.h"
#include "Rv32T/Device.h"

class UART_TX : public Device
{
	T_RTTI_CLASS;

public:
	explicit UART_TX(CData& vpin);

	virtual void eval(VSoC* soc, uint64_t time) override final;

private:
	CData& m_vpin;
	int32_t m_state = 0;
	int32_t m_ticks = 0;
	int32_t m_bit = 0;
	uint8_t m_data = 0;
};

