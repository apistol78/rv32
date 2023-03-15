/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include <Core/Object.h>

class CPU;

class Device : public traktor::Object
{
	T_RTTI_CLASS;

public:
	virtual bool writeU32(uint32_t address, uint32_t value) = 0;

	virtual uint32_t readU32(uint32_t address) const = 0;

	virtual bool tick(CPU* cpu) = 0;
};
