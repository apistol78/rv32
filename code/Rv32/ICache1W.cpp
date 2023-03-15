/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/ICache1W.h"
#include "Rv32/Bus.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"ICache1W", ICache1W, ICache)

ICache1W::ICache1W(Bus* bus)
:	m_bus(bus)
,   m_hits(0)
,   m_misses(0)
{
	for (int32_t i = 0; i < sizeof_array(m_data); ++i)
		m_data[i].valid = false;
}

ICache1W::~ICache1W()
{
	log::info << L"ICache1W " << m_hits << L"/" << m_misses << L" (" << str(L"%.2f%%", (m_hits * 100.0) / (m_hits + m_misses)) << L")" << Endl;
}

uint32_t ICache1W::readU32(uint32_t address)
{
	const uint32_t tag = (address >> 2) & (sizeof_array(m_data) - 1);
	Line& line = m_data[tag];

	if (line.valid && line.address == address)
	{
		m_hits++;
		return line.word;
	}

	const uint32_t word = m_bus->readU32(address);
	
	line.address = address;
	line.word = word;
	line.valid = true;

	m_misses++;
	return word;
}
