#include <algorithm>
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/Profiler.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"Profiler", Profiler, Object)

Profiler::~Profiler()
{
	AlignedVector< std::pair< uint32_t, uint32_t > > data;

	for (auto it : m_data)
		data.push_back({ it.first, it.second });

	std::sort(data.begin(), data.end(), [](const std::pair< uint32_t, uint32_t >& lh, const std::pair< uint32_t, uint32_t >& rh) {
		return lh.second > rh.second;
	});

	uint64_t total = 0;
	for (const auto& it : data)
		total += it.second;

	int32_t num = 0;
	for (const auto& it : data)
	{
		double p = (double)(it.second * 100.0) / total;
		log::info << str(L"%d. %08x", num, it.first) << L" : " << str(L"%.1f%%", p) << Endl;
		if (++num >= 16)
			break;
	}
}

void Profiler::record(uint32_t pc)
{
	m_data[pc & ~15U]++;
}
