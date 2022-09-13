#include <Core/Io/FileSystem.h>
#include "Rv32/Audio.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"Audio", Audio, Device)

Audio::Audio()
{
    m_stream = FileSystem::getInstance().open(L"Rv32_audio.s16", File::FmWrite);
}

bool Audio::writeU32(uint32_t address, uint32_t value)
{
    const int16_t sv = (int16_t)(value * 32767.0f);
    if (m_written > 0 || abs(sv) > 256)
    {
        m_stream->write(&sv, sizeof(sv));
        m_stream->flush();
        m_written++;
    }

    m_queued++;
	return true;
}

uint32_t Audio::readU32(uint32_t address) const
{
    return m_queued;
}

bool Audio::tick(CPU* cpu)
{
    if (++m_counter >= 9072)
    {
        if (m_queued > 0)
            m_queued--;

        m_counter = 0;
    }
	return true;
}
