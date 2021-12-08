#include <cstdio>
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/Video.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"Video", Video, Device)

void Video::writeU8(uint32_t address, uint8_t value)
{
    //log::info << L"VIDEO | " << str(L"%02x", value) << L" (" << (int32_t)value << L")" << Endl;
    putchar(value);
}

void Video::writeU32(uint32_t address, uint32_t value)
{
    log::info << L"VIDEO | " << str(L"%08x", value) << L" (" << (int32_t)value << L")" << Endl;
}

uint8_t Video::readU8(uint32_t address) const
{
	return 0;
}

uint32_t Video::readU32(uint32_t address) const
{
	return 0;
}
