#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/SD.h"

// Verilated SoC
#include "SoC/VSoC.h"

using namespace traktor;

void SD::eval(VSoC* soc)
{
    auto& clk = soc->SD_CLK;
    auto& cmd = soc->SD_CMD;
    auto& dat = soc->SD_DAT;

    if (m_lstclk != clk)
    {
        if (clk == 1)
        {
            if (dat)
                m_data |= 1 << m_bit;

            if (++m_bit >= 8)
            {
                //log::info << L"SD data " << m_data << Endl;
                m_bit = 0;
            }
        }

        m_lstclk = clk;
    }


}
