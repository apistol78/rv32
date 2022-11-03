#pragma once

#include <Core/Ref.h>
#include "Rv32/ICache.h"

class Bus;

class ICache1W : public ICache
{
	T_RTTI_CLASS;

public:
	explicit ICache1W(Bus* bus);

    virtual ~ICache1W();

	virtual uint32_t readU32(uint32_t address) override final;

private:
    struct Line
    {
        uint32_t address;
        uint32_t word;
        bool valid;
    };

	traktor::Ref< Bus > m_bus;
    Line m_data[16384];
    uint32_t m_hits;
    uint32_t m_misses;
};
