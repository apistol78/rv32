/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include <cstring>
#include <Core/Io/ChunkMemory.h>
#include <Core/Io/ChunkMemoryStream.h>
#include <Core/Io/FileSystem.h>
#include <Core/Io/IStream.h>
#include <Core/Io/StreamCopy.h>
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/CRC.h"
#include "Rv32/SD.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"SD", SD, Device)

SD::SD()
{
	m_memory = new ChunkMemory();
	m_stream = new ChunkMemoryStream(m_memory, true, true);

	// Read entire image into memory so we can modify it without damaging file.
	Ref< IStream > f = FileSystem::getInstance().open(L"FS.img", File::FmRead);
	StreamCopy(m_stream, f).execute();
}

bool SD::writeU32(uint32_t address, uint32_t value)
{
	const uint8_t mask = (uint8_t)(value >> 8);
	const uint8_t write = (uint8_t)value;

	if (mask & 0x01)
		m_r_clk = (value & 0x01) != 0;
	if (mask & 0x02)
		m_r_cdir = (value & 0x02) != 0;
	if (mask & 0x04)
		m_r_ddir = (value & 0x04) != 0;
	if (mask & 0x08)
		m_r_cmd = (value & 0x08) != 0;
	if (mask & 0xf0)
		m_r_dat = (value & 0xf0) >> 4;

	return true;
}

uint32_t SD::readU32(uint32_t address) const
{
	uint32_t v = 0;

	if (address == 0x00)
	{
		v |= (m_r_clk ? 0x01 : 0x00);
		v |= (m_r_cdir ? 0x02 : 0x00);
		v |= (m_r_ddir ? 0x04 : 0x00);
		v |= (m_r_cmd ? 0x08 : 0x00);
		v |= m_r_dat << 4;
	}
	else if (address == 0x08)
	{
		// Read data byte.
	}
	else if (address == 0x0c)
	{
		// Read data dword.
	}

	return v;
}

bool SD::tick(CPU* cpu)
{
	bool& cmd = m_r_cmd;
	uint8_t& dat = m_r_dat;

	if (m_lstclk != m_r_clk)
	{
		if (m_r_clk)   // pos edge
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
				//soc->SD_CMD_in = bit ? 1 : 0;
				cmd = bit;

				if (++m_rspbit >= 8)
				{
					m_rspbit = 0;
					if (++m_rspcnt >= m_send)
					{
						m_rspcnt = 0;
						m_send = 0;

						if (m_cmd[0] == (0x40 | 17))
							m_mode = 2;
						else if (m_cmd[0] == (0x40 | 24))
							m_mode = 3;
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
					//soc->SD_DAT_in = 0xff;
					dat = 0xff;
					m_blockOutputCount++;
				}
				else if (m_blockOutputCount == -1)
				{
					//soc->SD_DAT_in = 0x00;
					dat = 0x00;
					m_blockOutputCount = 0;
				}
				else
				{
					const int32_t offset = m_blockOutputCount >> 1;

					if (m_blockOutputCount & 1)
						//soc->SD_DAT_in = m_block[offset] & 0x0f;
						dat = m_block[offset] & 0x0f;
					else
						//soc->SD_DAT_in = m_block[offset] >> 4;
						dat = m_block[offset] >> 4;

					if (++m_blockOutputCount >= 512 * 2)
					{
						T_ASSERT(m_cmd.empty());
						m_mode = 0;
					}
				}
			}

			else if (m_mode == 3)
			{
				if (m_blockOutputCount < 0)
				{
					// // Data in should be zero
					// if (dat != 0x00)
					// 	log::warning << L"[SD] SD_DAT should be zero" << Endl;

					// m_blockOutputCount = 0;

					m_blockOutputCount++;
				}
				else if (m_blockOutputCount < 512 * 2)
				{
					const int32_t offset = m_blockOutputCount >> 1;

					if (m_blockOutputCount & 1)
						m_block[offset] |= (dat & 0x0f);
					else
						m_block[offset] |= (dat & 0x0f) << 4;

					++m_blockOutputCount;
				}
				else if (m_blockOutputCount < 512 * 2 + 16)
				{
					//log::info << L"[SD] CRC " << str(L"%x", dat & 0x0f) << Endl;
					m_blockOutputCount++;
				}
				else if (m_blockOutputCount < 512 * 2 + 16 + 1)
				{
					// Data in should be one
					if (dat == 0x00)
						log::warning << L"[SD] SD_DAT should be one" << Endl;

					dat = 0xff;

					m_blockOutputCount++;
				}
				else
				{
					log::info << L"[SD] Writing block..." << Endl;
					m_stream->write(m_block, 512);
					m_mode = 0;
				}			
			}
		}
	}
	else
	{
		if (m_r_clk && m_lstcmd != cmd)
		{
			m_bit = 0;
			m_data = 0;			
		}
	}

	m_lstclk = m_r_clk;
	m_lstcmd = cmd;

	return true;
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
		// log::info << L"cmd17" << Endl;

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
	else if (c == (0x40 | 24))
	{
		m_response[0] = 24;
		m_response[5] = (crc7(0, m_response, 5) << 1) | 0x01;

		uint32_t addr =
			(m_cmd[1] << 24) |
			(m_cmd[2] << 16) |
			(m_cmd[3] << 8) |
			(m_cmd[4]);

		log::info << L"[SD] CMD24, addr " << addr << Endl;

		addr *= 512;

		std::memset(m_block, 0, sizeof(m_block));
		m_stream->seek(IStream::SeekSet, addr);	

		m_blockOutputCount = -9;	
	}
	else if (c == (0x40 | 6))    // acmd6
	{
		//log::info << L"acmd6" << Endl;
	}
	else if (c == (0x40 | 42))    // acmd42
	{
		//log::info << L"acmd42" << Endl;
	}
	// else
	// 	log::info << L"[SD] Unknown command, " << str(L"%02x", c) << Endl;
}
