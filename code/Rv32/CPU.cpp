#include <Core/Io/OutputStream.h>
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/Bus.h"
#include "Rv32/CPU.h"
#include "Rv32/DCache.h"
#include "Rv32/Helpers.h"
#include "Rv32/ICache.h"

using namespace traktor;

namespace
{

struct FormatCSR
{
	uint32_t rd;
	uint32_t rs1;
	uint16_t csr;
};

inline FormatCSR parseFormatCSR(uint32_t word)
{
	return FormatCSR {
		(word >> 7) & 0x1f,
		(word >> 15) & 0x1f,
		(uint16_t)((word >> 20) & 0xfff)
	};
}

struct FormatI
{
	uint32_t rd;
	uint32_t rs1;
	int32_t imm;
};

inline FormatI parseFormatI(uint32_t word)
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

inline FormatJ parseFormatJ(uint32_t word)
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

inline FormatR parseFormatR(uint32_t word)
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

inline FormatS parseFormatS(uint32_t word)
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

inline FormatB parseFormatB(uint32_t word)
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

inline FormatU parseFormatU(uint32_t word)
{
	FormatU f = {
		(word >> 7) & 0x1f,
		(int32_t)(word & 0xfffff000)
	};
	// check this!
	return f;
}

struct FormatR4
{
	uint32_t rd;
	uint32_t rs1;
	uint32_t rs2;
	uint32_t rs3;
};

inline FormatR4 parseFormatR4(uint32_t word)
{
	return FormatR4 {
		(word >> 7) & 0x1f,
		(word >> 15) & 0x1f,
		(word >> 20) & 0x1f,
		(word >> 27) & 0x1f
	};
}

#define PC m_pc
#define PC_NEXT m_next

#define R(x) m_registers[x]
#define R_s(x) (int32_t&)m_registers[x]
#define R_u(x) (uint32_t&)m_registers[x]

#define FR(x) m_flt_registers[x]
#define FR_u(x) (uint32_t&)m_flt_registers[x]

#define MEM_RD(addr) m_dcache->readU32(addr)
#define MEM_WR(addr, value) m_dcache->writeU32(addr, value)

#define MEM_RD_U16(addr) m_dcache->readU16(addr)
#define MEM_WR_U16(addr, value) m_dcache->writeU16(addr, value)

#define MEM_RD_U8(addr) m_dcache->readU8(addr);
#define MEM_WR_U8(addr, value) m_dcache->writeU8(addr, value)

#define TRACE(s) // if (m_trace) { *m_trace << s << Endl; }

}

T_IMPLEMENT_RTTI_CLASS(L"CPU", CPU, Object)

CPU::CPU(Bus* bus, OutputStream* trace)
:   m_bus(bus)
,	m_dcache(new DCache(bus))
,	m_icache(new ICache(bus))
,	m_trace(trace)
,   m_pc(0x00000000)
,	m_interrupt(false)
,	m_waitForInterrupt(false)
{
	for (uint32_t i = 0; i < sizeof_array(m_registers); ++i)
		m_registers[i] = 0x00000000;

	m_registers[2] = 0x00010400 - 4;
}

void CPU::jump(uint32_t address)
{
	m_pc = address;
}

void CPU::setSP(uint32_t sp)
{
	m_registers[2] = sp;
}

bool CPU::tick()
{
	// Check if CPU in low power mode and
	// are waiting for interrupt.
	if (m_waitForInterrupt && !m_interrupt)
		return true;

	if (m_interrupt)
	{
		log::info << str(L"%08x", m_pc) << Endl;

		writeCSR(CSR::MEPC, m_pc);
		writeCSR(CSR::MCAUSE, 0x80000000 | (1 << 7));	// Machine timer

		const uint32_t mtvec = readCSR(CSR::MTVEC);
		m_pc = mtvec;

		m_waitForInterrupt = false;
	}

	uint32_t word = m_icache->readU32(m_pc);

	if (m_trace)
	{
		*m_trace << str(L"%08x", m_pc);
		for (int32_t i = 1; i < 32; ++i)
			*m_trace << L" " << str(L"%08x", m_registers[i]);
		*m_trace << Endl;
	}

	if ((word & 0x3) == 0x3)
		m_next = m_pc + 4;
	else
		m_next = m_pc + 2;

	// zero register is hardcoded.
	R(0) = 0;

	if (!decode(word))
	{
		log::error << L"Decode (" << str(L"%08x", word) << L") failed at PC " << str(L"%08x", m_pc) << Endl;
		return false;
	}

	m_pc = m_next;

	if (!m_bus->tick(this))
		return false;

	return true;
}

void CPU::interrupt()
{
	m_interrupt = true;
}

bool CPU::decode(uint32_t word)
{
	#include "Instructions.inl"
}

uint32_t CPU::readCSR(uint16_t csr) const
{
	//log::info << L"read CSR " << str(L"%03x", csr) << Endl;
	return m_csr[csr];
}

void CPU::writeCSR(uint16_t csr, uint32_t value)
{
	//log::info << L"write CSR " << str(L"%03x", csr) << L" <= " << str(L"%08x", value) << Endl;
	m_csr[csr] = value;
}
