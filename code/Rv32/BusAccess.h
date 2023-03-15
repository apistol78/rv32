/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include <Core/Object.h>
#include <Core/Ref.h>
#include <Core/Containers/SmallSet.h>

class DCache;

class BusAccess : public traktor::Object
{
	T_RTTI_CLASS;

public:
	explicit BusAccess(DCache* dcache);

	virtual ~BusAccess();

	void writeU8(uint32_t pc, uint32_t address, uint8_t value);

	void writeU16(uint32_t pc, uint32_t address, uint16_t value);

	void writeU32(uint32_t pc, uint32_t address, uint32_t value);

	uint8_t readU8(uint32_t pc, uint32_t address) const;

	uint16_t readU16(uint32_t pc, uint32_t address) const;

	uint32_t readU32(uint32_t pc, uint32_t address) const;

private:
	traktor::Ref< DCache > m_dcache;
	mutable uint64_t m_nw8 = 0;
	mutable uint64_t m_nw16 = 0;
	mutable uint64_t m_nw32 = 0;
	mutable uint64_t m_nr8 = 0;
	mutable uint64_t m_nr16 = 0;
	mutable uint64_t m_nr32 = 0;
	mutable traktor::SmallSet< uint32_t > m_ua16;
	mutable traktor::SmallSet< uint32_t > m_ua32;
};
