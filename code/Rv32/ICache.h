#pragma once

#include <Core/Object.h>

class ICache : public traktor::Object
{
	T_RTTI_CLASS;

public:
	virtual uint32_t readU32(uint32_t address) = 0;
};
