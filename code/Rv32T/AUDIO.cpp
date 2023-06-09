/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include <Core/Io/FileSystem.h>
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/AUDIO.h"
#include "Rv32T/SoC/VSoC.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"AUDIO", AUDIO, Device)

AUDIO::AUDIO()
{
	m_stream = FileSystem::getInstance().open(L"Rv32T_audio.s16", File::FmWrite);
}

void AUDIO::eval(VSoC* soc, uint64_t time)
{
	auto& pwm = soc->AUDIO_PWM;

	const float f = 0.01f;
	m_value = m_value * (1.0f - f) + (pwm ? 1.0f : -1.0f) * f;

	const uint64_t ticks = 100000000ULL / 22050ULL; // 44100ULL;
	if ((time % ticks) == 0)
	{
		const int16_t sv = (int16_t)(m_value * 32767.0f);
		if (m_written > 0 || abs(sv) > 256)
		{
			m_stream->write(&sv, sizeof(sv));
			m_stream->flush();
			m_written++;
		}
	}
}
