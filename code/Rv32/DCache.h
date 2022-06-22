#pragma once

#include <Core/Object.h>
#include <Core/Ref.h>

class Bus;

class DCache : public traktor::Object
{
	T_RTTI_CLASS;

public:
	explicit DCache(Bus* bus);

    virtual ~DCache();

	void writeU32(uint32_t address, uint32_t value);

	uint32_t readU32(uint32_t address);

	void flush();

private:
    struct Line
    {
        uint32_t address;
        uint32_t word;
        bool valid;
		bool dirty;
    };

	traktor::Ref< Bus > m_bus;
    Line m_data[65536];
    uint32_t m_hits;
    uint32_t m_misses;
};
