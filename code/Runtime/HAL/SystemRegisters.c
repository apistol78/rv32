#include "Runtime/HAL/SystemRegisters.h"

#define BASE (volatile uint32_t*)(SYSREG_BASE)

void sysreg_write(uint8_t reg, uint32_t value)
{
	(BASE)[reg] = value;
}

void sysreg_modify(uint8_t reg, uint32_t mask, uint32_t value)
{
	(BASE)[reg] = ((BASE)[reg] & ~mask) | (value & mask);
}

uint32_t sysreg_read(uint8_t reg)
{
	return (BASE)[reg];
}
