#pragma once

#include <Core/Object.h>
#include <Core/Ref.h>

class Bus;

class ICache : public traktor::Object
{
	T_RTTI_CLASS;

public:
	explicit ICache(Bus* bus);

    virtual ~ICache();

	uint32_t readU32(uint32_t address);

private:
	traktor::Ref< Bus > m_bus;
    bool m_valid[64];
    uint32_t m_data[64][2];
    uint32_t m_hits;
    uint32_t m_misses;
};
