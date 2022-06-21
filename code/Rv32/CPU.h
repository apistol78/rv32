#pragma once

#include <Core/Object.h>
#include <Core/Ref.h>

namespace traktor
{

class OutputStream;

}

class Bus;
class DCache;
class ICache;

enum CSR
{
	MTVEC = 0x305,
	MEPC = 0x341,
	MCAUSE = 0x342
};

class CPU : public traktor::Object
{
	T_RTTI_CLASS;

public:
	explicit CPU(Bus* bus, traktor::OutputStream* trace);

	void jump(uint32_t address);

	void setSP(uint32_t sp);

	bool tick();

	void interrupt();

	uint32_t pc() const { return m_pc; }

	uint32_t sp() const { return m_registers[2]; }

	uint32_t reg(uint32_t index) { return m_registers[index]; }

private:
	traktor::Ref< Bus > m_bus;
	traktor::Ref< DCache > m_dcache;
	traktor::Ref< ICache > m_icache;
	traktor::Ref< traktor::OutputStream > m_trace;
	uint32_t m_pc;
	uint32_t m_next;
	uint32_t m_registers[32];
	float m_flt_registers[32];
	uint32_t m_csr[4096];
	bool m_interrupt;
	bool m_waitForInterrupt;

	bool decode(uint32_t word);

	uint32_t readCSR(uint16_t csr) const;

	void writeCSR(uint16_t csr, uint32_t value);
};