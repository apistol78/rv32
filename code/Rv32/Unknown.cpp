#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/Unknown.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"Unknown", Unknown, Device)

Unknown::Unknown(const std::wstring& deviceName)
:	m_deviceName(deviceName)
{
}

bool Unknown::writeU8(uint32_t address, uint8_t value)
{
	log::info << m_deviceName << L" : WRITE U8, address " << str(L"%08x", address) << L", value " << str(L"%02x", value) << Endl;
	return true;
}

bool Unknown::writeU16(uint32_t address, uint16_t value)
{
	log::info << m_deviceName << L" : WRITE U16, address " << str(L"%08x", address) << L", value " << str(L"%04x", value) << Endl;
	return true;
}

bool Unknown::writeU32(uint32_t address, uint32_t value)
{
	log::info << m_deviceName << L" : WRITE U32, address " << str(L"%08x", address) << L", value " << str(L"%08x", value) << Endl;
	return true;
}

uint8_t Unknown::readU8(uint32_t address) const
{
	log::info << m_deviceName << L" : READ U8, address " << str(L"%08x", address) << Endl;
	return 0;
}

uint16_t Unknown::readU16(uint32_t address) const
{
	log::info << m_deviceName << L" : READ U16, address " << str(L"%08x", address) << Endl;
	return 0;
}

uint32_t Unknown::readU32(uint32_t address) const
{
	log::info << m_deviceName << L" : READ U32, address " << str(L"%08x", address) << Endl;
	return 0;
}