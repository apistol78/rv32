#pragma once

#include <Core/Config.h>

class VSoC;

class LEDR
{
public:
	void eval(VSoC* soc);

private:
	uint32_t m_current = 0;
};
