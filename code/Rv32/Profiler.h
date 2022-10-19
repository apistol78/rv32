#pragma once

#include <Core/Object.h>
#include <Core/Containers/SmallMap.h>

class Profiler : public traktor::Object
{
	T_RTTI_CLASS;

public:
	virtual ~Profiler();

	void record(uint32_t pc);

private:
	traktor::SmallMap< uint32_t, uint32_t > m_data;
};
