#pragma once

#include <Core/Object.h>

class VSoC;

class Device : public traktor::Object
{
    T_RTTI_CLASS;

public:
    virtual void eval(VSoC* soc, uint64_t time) = 0;
};
