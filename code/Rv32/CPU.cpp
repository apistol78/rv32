#include <Core/Io/OutputStream.h>
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/Bus.h"
#include "Rv32/CPU.h"
#include "Rv32/Helpers.h"

using namespace traktor;

namespace
{

struct FormatI
{
	uint32_t rd;
	uint32_t rs1;
	int32_t imm;
};

FormatI parseFormatI(uint32_t word)
{
	FormatI f = {
		(word >> 7) & 0x1f,
		(word >> 15) & 0x1f,
		(int32_t)((word >> 20) & 0x000007ff)
	};

	if ((word & 0x80000000) == 0x80000000)
		f.imm |= 0xfffff800;
	
	return f;
}

struct FormatJ
{
	uint32_t rd;
	int32_t imm;
};

FormatJ parseFormatJ(uint32_t word)
{
	FormatJ f = {
		(word >> 7) & 0x1f,
		(int32_t)((word & 0x000ff000) | ((word & 0x00100000) >> 9) | ((word & 0x7fe00000) >> 20))
	};

	if ((word & 0x80000000) == 0x80000000)
		f.imm |= 0xfff00000;
	
	return f;
}

struct FormatR
{
	uint32_t rd;
	uint32_t rs1;
	uint32_t rs2;
};

FormatR parseFormatR(uint32_t word)
{
	return FormatR {
		(word >> 7) & 0x1f,
		(word >> 15) & 0x1f,
		(word >> 20) & 0x1f
	};
}

struct FormatS
{
	uint32_t rs1;
	uint32_t rs2;
	int32_t imm;
};

FormatS parseFormatS(uint32_t word)
{
	FormatS f = {
		(word >> 15) & 0x1f,
		(word >> 20) & 0x1f,
		(int32_t)(((word >> 20) & 0xfe0) | ((word >> 7) & 0x1f))
	};

	if ((word & 0x80000000) == 0x80000000)
		f.imm |= 0xfffff000;
	
	return f;
}

struct FormatB
{
	uint32_t rs1;
	uint32_t rs2;
	int32_t imm;
};

FormatB parseFormatB(uint32_t word)
{
	FormatB f = {
		(word >> 15) & 0x1f,
		(word >> 20) & 0x1f,
		(int32_t)(((word << 4) & 0x00000800) | ((word >> 20) & 0x000007e0) | ((word >> 7) & 0x0000001e))
	};

	if ((word & 0x80000000) == 0x80000000)
		f.imm |= 0xfffff000;
	
	return f;
}

struct FormatU
{
	uint32_t rd;
	int32_t imm;
};

FormatU parseFormatU(uint32_t word)
{
	FormatU f = {
		(word >> 7) & 0x1f,
		(int32_t)(word & 0xfffff000)
	};
	// check this!
	return f;
}

#define PC m_pc
#define PC_NEXT m_next

#define R(x) m_registers[x]

#define R_s(x) (int32_t&)m_registers[x]
#define R_u(x) (uint32_t&)m_registers[x]

#define MEM_RD(addr) m_bus->readU32(addr)
#define MEM_WR(addr, value) m_bus->writeU32(addr, value)

#define MEM_RD_U16(addr) m_bus->readU16(addr)
#define MEM_WR_U16(addr, value) m_bus->writeU16(addr, value)

#define MEM_RD_U8(addr) m_bus->readU8(addr);
#define MEM_WR_U8(addr, value) m_bus->writeU8(addr, value)

#define TRACE(s) if (m_trace) { *m_trace << s << Endl; }



uint32_t icache[64][2];

uint32_t getFromICache(uint32_t pc)
{
	uint32_t slot = pc & 63;
	if (icache[slot][0] == pc)
		return icache[slot][1];
	else
		return 0;
}

void putIntoICache(uint32_t pc, uint32_t word)
{
	uint32_t slot = pc & 63;
	icache[slot][0] = pc;
	icache[slot][1] = word;
}


}

T_IMPLEMENT_RTTI_CLASS(L"CPU", CPU, Object)

CPU::CPU(Bus* bus, OutputStream* trace)
:   m_bus(bus)
,	m_trace(trace)
,   m_pc(0x00000000)
{
	for (uint32_t i = 0; i < sizeof_array(m_registers); ++i)
		m_registers[i] = 0x00000000;

	m_registers[2] = 0x00010400;
}

void CPU::jump(uint32_t address)
{
	m_pc = address;
}

bool CPU::tick()
{
	uint32_t word;
	
	word = getFromICache(m_pc);
	if (word == 0)
	{
		word = m_bus->readU32(m_pc);
		putIntoICache(m_pc, word);
	}

	if (m_trace)
		*m_trace << L"STATE_DECODE, PC: " << str(L"%08x", m_pc) << L", SP: " << str(L"%08x", m_registers[2]) << Endl;

	if ((word & 0x3) == 0x3)
		m_next = m_pc + 4;
	else
		m_next = m_pc + 2;

	// zero register is hardcoded.
	R(0) = 0;

	if (!decode(word))
	{
		log::error << L"Decode failed at PC " << str(L"%08x", m_pc) << Endl;
		return false;
	}

	if (m_trace)
		*m_trace << L"STATE_DECODE_FINISH, PC: " << str(L"%08x", m_pc) << L", PC_NEXT: " << str(L"%08x", m_next) << Endl << Endl;

	m_pc = m_next;
	return true;
}

bool CPU::decode(uint32_t word)
{
	#include "Instructions.inl"
}