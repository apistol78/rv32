#include <Core/Io/FileSystem.h>
#include <Core/Io/IStream.h>
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32T/CRC.h"
#include "Rv32T/SD.h"
#include "Rv32T/SoC/VSoC.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"SD", SD, Device)

SD::SD()
{
	m_stream = FileSystem::getInstance().open(L"FS.img", File::FmRead);
}

void SD::eval(VSoC* soc, uint32_t time)
{
	auto& clk = soc->SD_CLK;
	auto& cmd = soc->SD_CMD_out;

	if (m_lstclk != clk)
	{
		if (clk)   // pos edge
		{
			// Read command.
			if (m_mode == 0)
			{
				if (cmd)
					m_data |= 0x80 >> m_bit;

				++m_bit;

				if (m_bit >= 8)
				{
					m_cmd.push_back(m_data);
					m_bit = 0;
					m_data = 0;

					process();
				}
				else if (m_cmd.empty())
				{
					if (m_bit == 2 && m_data != 0x40)
					{
						m_bit = 0;
						m_data = 0;
					}
				}
			}

			// Send response.
			else if (m_mode == 1)
			{
				bool bit = (m_response[m_rspcnt] & (0x80 >> m_rspbit)) != 0;
				soc->SD_CMD_in = bit ? 1 : 0;
				if (++m_rspbit >= 8)
				{
					m_rspbit = 0;
					if (++m_rspcnt >= m_send)
					{
						m_rspcnt = 0;
						m_send = 0;

						if (m_cmd[0] == (0x40 | 17))
							m_mode = 2;
						else
							m_mode = 0;

						m_cmd.resize(0);
					}
				}
			}

			else if (m_mode == 2)
			{
				if (m_blockOutputCount < -1)
				{
					soc->SD_DAT_in = 0xff;
					m_blockOutputCount++;
				}
				else if (m_blockOutputCount == -1)
				{
					soc->SD_DAT_in = 0x00;
					m_blockOutputCount = 0;
				}
				else
				{
					const int32_t offset = m_blockOutputCount >> 1;

					if (m_blockOutputCount & 1)
						soc->SD_DAT_in = m_block[offset] & 0x0f;
					else
						soc->SD_DAT_in = m_block[offset] >> 4;

					if (++m_blockOutputCount >= 512 * 2)
					{
						T_ASSERT(m_cmd.empty());
						m_mode = 0;
					}
				}
			}
		}
	}
	else
	{
		if (clk && m_lstcmd != cmd)
		{
			m_bit = 0;
			m_data = 0;			
		}
	}

	m_lstclk = clk;
	m_lstcmd = cmd;

	soc->KEY = m_mode;
}

void SD::process()
{
	if (m_cmd.size() < 6)
		return;

	m_rspcnt = 0;
	m_rspbit = 0;
	m_mode = 1;

	// 6 bytes resp
	std::memset(m_response, 0, 6);
	m_send = 6;

	uint8_t c = m_cmd.front();
	if (c == 0x40)  // cmd0
	{
		//log::info << L"cmd0" << Endl;
	}
	else if (c == (0x40 | 8))    // cmd8
	{
		//log::info << L"cmd8" << Endl;

		m_response[0] = 8;
		m_response[3] = m_cmd[3];
		m_response[4] = m_cmd[4];
		m_response[5] = (crc7(0, m_response, 5) << 1) | 0x01;
	}
	else if (c == (0x40 | 55))    // cmd55
	{
		//log::info << L"cmd55" << Endl;

		// 6 bytes resp
		m_response[0] = 55;
		m_response[5] = (crc7(0, m_response, 5) << 1) | 0x01;
	}
	else if (c == (0x40 | 41))    // acmd41
	{
		//log::info << L"acmd41" << Endl;

		// 6 bytes resp
		m_response[0] = 0x3f;
		m_response[1] = 0xff;
		m_response[2] = 0xff;
		m_response[3] = 0xff;
		m_response[4] = 0xff;
		m_response[5] |= 0xff;
	}
	else if (c == (0x40 | 2))    // cmd2
	{
		//log::info << L"cmd2" << Endl;
	}
	else if (c == (0x40 | 3))    // cmd3
	{
		//log::info << L"cmd3" << Endl;
	}
	else if (c == (0x40 | 9))    // cmd9
	{
		//log::info << L"cmd9" << Endl;
	}
	else if (c == (0x40 | 7))    // cmd7
	{
		//log::info << L"cmd7" << Endl;
	}
	else if (c == (0x40 | 16))    // cmd16
	{
		//log::info << L"cmd16" << Endl;
	}
	else if (c == (0x40 | 17))    // cmd17
	{
		//log::info << L"cmd17" << Endl;

		uint32_t addr =
			(m_cmd[1] << 24) |
			(m_cmd[2] << 16) |
			(m_cmd[3] << 8) |
			(m_cmd[4]);

		addr *= 512;

		std::memset(m_block, 0, sizeof(m_block));
		m_stream->seek(IStream::SeekSet, addr);
		m_stream->read(m_block, 512);

		m_blockOutputCount = -100;
	}
	else if (c == (0x40 | 6))    // acmd6
	{
		//log::info << L"acmd6" << Endl;
	}
	else if (c == (0x40 | 42))    // acmd42
	{
		//log::info << L"acmd42" << Endl;
	}
	else
		log::info << L"unkown command, " << str(L"%02x", c) << Endl;
}
