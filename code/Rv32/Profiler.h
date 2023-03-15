/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include <Core/Object.h>
#include <Core/Containers/SmallMap.h>

class Profiler : public traktor::Object
{
	T_RTTI_CLASS;

public:
	virtual ~Profiler();

	void record(uint32_t pc);

private:
	traktor::SmallMap< uint32_t, uint32_t > m_data;
};
