/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include <Core/Io/FileSystem.h>
#include <Core/Io/IStream.h>
#include <Core/Io/StringReader.h>
#include <Core/Io/AnsiEncoding.h>
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/Bus.h"
#include "Rv32/BusAccess.h"
#include "Rv32/CPU.h"
#include "Rv32/DCache.h"
#include "Rv32/LoadHEX.h"

using namespace traktor;

bool loadHEX(const std::wstring& fileName, CPU& cpu, Bus& bus)
{
	std::wstring tmp;

	Ref< IStream > f = FileSystem::getInstance().open(fileName, File::FmRead);
	if (!f)
	{
		log::error << L"Unable to open HEX \"" << fileName << L"\"." << Endl;
		return false;
	}

	// Temporary dcache & bus access so we can write bytes.
	DCache dcache(&bus);
	BusAccess busAccess(&dcache);	

	uint32_t segment = 0x00000000;
	uint32_t upper = 0x00000000;
	uint32_t start = 0xffffffff;
	uint32_t end = 0x00000000;

	StringReader sr(f, new AnsiEncoding());
	while (sr.readLine(tmp) >= 0)
	{
		if (tmp.empty())
			continue;

		// Ensure start byte is correct.
		if (tmp[0] != L':')
			continue;
		tmp = tmp.substr(1);

		// Parse header.
		int32_t ln = parseString< int32_t >(L"0x" + tmp.substr(0, 2));
		int32_t addr = parseString< int32_t >(L"0x" + tmp.substr(2, 4));
		int32_t type = parseString< int32_t >(L"0x" + tmp.substr(6, 2));

		if (type == 0x00)
		{
			for (int32_t i = 8; i < 8 + ln * 2; i += 2)
			{
				int32_t v = parseString< int32_t >(L"0x" + tmp.substr(i, 2));
				busAccess.writeU8(0, (upper | addr) + segment, (uint8_t)v);
				start = std::min(start, (upper | addr) + segment);
				end = std::max(end, (upper | addr) + segment);
				addr++;
			}
		}
		else if (type == 0x01)
			break;
		else if (type == 0x02)
		{
			segment = parseString< int32_t >(L"0x" + tmp.substr(8, 4));
			segment *= 16;
		}
		else if (type == 0x03)
			continue;
		else if (type == 0x04)
		{
			upper = parseString< int32_t >(L"0x" + tmp.substr(8, 4));
			upper <<= 16;
		}
		else if (type == 0x05)
		{
			const uint32_t linear = parseString< uint32_t >(L"0x" + tmp.substr(8, 8));
			cpu.jump(linear);
		}
		else
			log::warning << L"Unhandled HEX record type " << type << L"." << Endl;
	}

	if (start <= end)
		log::info << L"HEX loaded into " << str(L"0x%08x", start) << L" - " << str(L"0x%08x", end) << L"." << Endl;

	dcache.flush();
	return true;
}
