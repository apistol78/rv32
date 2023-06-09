/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#pragma once

#include <Core/Ref.h>
#include <Core/Containers/AlignedVector.h>
#include <Core/Io/IStream.h>
#include <Core/Io/ChunkMemory.h>
#include "Rv32T/Device.h"

class SD : public Device
{
	T_RTTI_CLASS;

public:
	SD();

	virtual void eval(VSoC* soc, uint64_t time) override final;

private:
	int32_t m_mode = 0;
	int32_t m_bit = 0;
	int32_t m_data = 0;
	bool m_lstclk = true;
	bool m_lstcmd = false;
	traktor::AlignedVector< uint8_t > m_cmd;
	uint8_t m_response[1024];
	int32_t m_send = 0;
	int32_t m_rspcnt = 0;
	int32_t m_rspbit = 0;
	int32_t m_blockOutputCount = 0;
	int32_t m_dly = 0;
	uint8_t m_block[512];
	traktor::Ref< traktor::IStream > m_stream;
	traktor::Ref< traktor::ChunkMemory > m_memory;

	void process();
};
