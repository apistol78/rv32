#pragma once

#include <Core/Object.h>
#include <Core/Ref.h>

namespace traktor
{

class OutputStream;

}

class Bus;

class CPU : public traktor::Object
{
    T_RTTI_CLASS;

public:
    CPU(Bus* bus, traktor::OutputStream* trace);

    void jump(uint32_t address);

    bool tick();

    uint32_t pc() const { return m_pc; }

    uint32_t sp() const { return m_registers[2]; }

private:
    traktor::Ref< Bus > m_bus;
    traktor::Ref< traktor::OutputStream > m_trace;
    uint32_t m_pc;
    uint32_t m_next;
    uint32_t m_registers[32];

    bool decode(uint32_t word);
};