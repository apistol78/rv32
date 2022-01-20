#pragma once

#include <Core/Config.h>

class VSoC;

class SD
{
public:
	void eval(VSoC* soc);

private:
    int32_t m_bit = 0;
    int32_t m_data = 0;
    int32_t m_state = 0;
    bool m_lstclk = true;
};
