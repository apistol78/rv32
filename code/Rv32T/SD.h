#pragma once

#include <Core/Ref.h>
#include <Core/Containers/AlignedVector.h>
#include <Core/Io/IStream.h>

class VSoC;

class SD
{
public:
    SD();

	void eval(VSoC* soc);

private:
    int32_t m_mode = 0;
    int32_t m_bit = 0;
    int32_t m_data = 0;
    bool m_lstclk = true;
    traktor::AlignedVector< uint8_t > m_cmd;
    uint8_t m_response[1024];
    int32_t m_send = 0;
    int32_t m_rspcnt = 0;
    int32_t m_rspbit = 0;
    int32_t m_blockOutputCount = 0;
    int32_t m_dly = 0;
    uint8_t m_block[512];
    traktor::Ref< traktor::IStream > m_stream;

    void process();
};