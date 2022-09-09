#include "Rv32/Audio.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"Audio", Audio, Device)

bool Audio::writeU32(uint32_t address, uint32_t value)
{
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
