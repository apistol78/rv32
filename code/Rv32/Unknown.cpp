/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/Unknown.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"Unknown", Unknown, Device)

Unknown::Unknown(const std::wstring& deviceName, bool mute)
:	m_deviceName(deviceName)
,	m_mute(mute)
{
}

bool Unknown::writeU32(uint32_t address, uint32_t value)
{
	if (!m_mute)
		log::info << m_deviceName << L" : WRITE U32, address " << str(L"%08x", address) << L", value " << str(L"%08x", value) << Endl;
	return true;
}

uint32_t Unknown::readU32(uint32_t address) const
{
	if (!m_mute)
		log::info << m_deviceName << L" : READ U32, address " << str(L"%08x", address) << Endl;
	return 0;
}

bool Unknown::tick(CPU* cpu)
{
	return true;
}
