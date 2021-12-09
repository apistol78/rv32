#include <cstring>
#include "Rv32/Memory.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"Memory", Memory, Device)

Memory::Memory(uint32_t capacity)
{
	m_data.reset(new uint8_t [capacity]);
	std::memset(m_data.ptr(), 0, capacity);
}

void Memory::writeU8(uint32_t address, uint8_t value)
{
	*(uint8_t*)(m_data.c_ptr() + address) = value;
}

void Memory::writeU16(uint32_t address, uint16_t value)
{
	*(uint16_t*)(m_data.c_ptr() + address) = value;
}

void Memory::writeU32(uint32_t address, uint32_t value)
{
	*(uint32_t*)(m_data.c_ptr() + address) = value;
}

uint8_t Memory::readU8(uint32_t address) const
{
	return *(const uint8_t*)(m_data.c_ptr() + address);
}

uint16_t Memory::readU16(uint32_t address) const
{
	return *(const uint16_t*)(m_data.c_ptr() + address);
}

uint32_t Memory::readU32(uint32_t address) const
{
	return *(const uint32_t*)(m_data.c_ptr() + address);
}
