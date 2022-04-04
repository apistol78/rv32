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

	const float f = 0.001f;
	m_value = m_value * (1.0f - f) + (pwm ? 1.0f : -1.0f) * f;

	const uint64_t ticks = 100000000ULL / 44100ULL;
	if ((time % ticks) == 0)
	{
		int16_t sv = (int16_t)(m_value * 32767.0f);
		m_stream->write(&sv, sizeof(sv));
		m_written++;
	}
}
