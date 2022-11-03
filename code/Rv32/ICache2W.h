#pragma once

#include <Core/Ref.h>
#include "Rv32/ICache.h"

class Bus;

class ICache2W : public ICache
{
	T_RTTI_CLASS;

public:
	explicit ICache2W(Bus* bus);

    virtual ~ICache2W();

	virtual uint32_t readU32(uint32_t address) override final;

private:
    /*
    13 => 8192
    */

    struct Line
    {
        uint32_t tag[2];
        uint32_t word[2];
        bool valid[2];
        uint8_t last;
    };

	traktor::Ref< Bus > m_bus;
    Line m_data[8192];
    uint32_t m_hits;
    uint32_t m_misses;
};
