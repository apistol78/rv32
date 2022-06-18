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
    struct Line
    {
        uint32_t address;
        uint32_t word;
        bool valid;
    };

	traktor::Ref< Bus > m_bus;
    Line m_data[65536];
    uint32_t m_hits;
    uint32_t m_misses;
};
