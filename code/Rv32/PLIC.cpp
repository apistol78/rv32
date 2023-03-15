/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/CPU.h"
#include "Rv32/PLIC.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"PLIC", PLIC, Device)

bool PLIC::writeU32(uint32_t address, uint32_t value)
{
	if (address == 0x00002000)
		m_enable = value >> 1;
	else if (address == 0x00200004)
	{
		log::info << L"[PLIC] Complete context" << Endl;
		m_raised &= ~(1 << (value & 3));
	}	
	return true;
}

uint32_t PLIC::readU32(uint32_t address) const
{
	if (address == 0x00200004)
	{
		log::info << L"[PLIC] Claim context" << Endl;
		for (int i = 0; i < 4; ++i)
		{
			if ((m_raised & (1 << i)) != 0)
				return i;
		}
	}
	return 0;
}

bool PLIC::tick(CPU* cpu)
{
	if (m_raised != m_issued)
	{
		cpu->interrupt(EXTERNAL);
		m_issued = m_raised;
	}
	return true;
}

void PLIC::raise(uint32_t channel)
{
	m_raised |= 1 << channel;
}
