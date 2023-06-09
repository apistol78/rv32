/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include <Core/Config.h>

class Measure
{
public:
	uint32_t value() const
	{
		return m_value;
	}

	uint32_t delta() const
	{
		return m_value - m_last;
	}

	void snapshot()
	{
		m_last = m_value;
	}

	Measure& operator += (int32_t delta)
	{
		m_value = delta;
		return *this;
	}

	Measure& operator ++ (int)
	{
		m_value++;
		return *this;
	}

private:
	uint32_t m_value = 0;
	uint32_t m_last = 0;
};
