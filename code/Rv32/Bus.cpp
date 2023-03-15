/*
 TRAKTOR
 Copyright (c) 2023 Anders Pistol.

 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at https://mozilla.org/MPL/2.0/.
*/
#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/Bus.h"
#include "Rv32/Memory.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"Bus", Bus, Object)

void Bus::map(uint32_t start, uint32_t end, bool cacheable, bool tick, Device* device)
{
	m_mappedDevices.push_back({ start, end, cacheable, device });
	if (tick)
		m_tickDevices.push_back(device);
}

Device* Bus::device(uint32_t address) const
{
	auto mappedDevice = findMappedDevice(address);
	if (mappedDevice)
		return mappedDevice->device;
	else
		return nullptr;	
}

bool Bus::cacheable(uint32_t address) const
{
	auto mappedDevice = findMappedDevice(address);
	if (mappedDevice)
		return mappedDevice->cacheable;
	else
		return false;
}

bool Bus::writeU32(uint32_t address, uint32_t value)
{
	auto mappedDevice = findMappedDevice(address);
	if (mappedDevice)
	{
		if (!mappedDevice->device->writeU32(address - mappedDevice->start, value))
			m_error = true;
	}
	else
	{
		log::error << L"No device at 0x" << str(L"%08x", address) << L", trying to write 0x" << str(L"%08x", value) << L"." << Endl;
		m_error = true;
	}
	return !m_error;
}

uint32_t Bus::readU32(uint32_t address) const
{
	auto mappedDevice = findMappedDevice(address);
	if (mappedDevice)
		return mappedDevice->device->readU32(address - mappedDevice->start);
	else
	{
		log::error << L"No device at 0x" << str(L"%08x", address) << L", trying to read." << Endl;
		m_error = true;
		return 0;
	}
}

bool Bus::tick(CPU* cpu) const
{
	for (auto device : m_tickDevices)
	{
		if (!device->tick(cpu))
			return false;
	}
	return true;
}

const Bus::MappedDevice* Bus::findMappedDevice(uint32_t address) const
{
	if (m_last)
	{
		if (address >= m_last->start && address < m_last->end)
			return m_last;		
	}
	for (auto& mappedDevice : m_mappedDevices)
	{
		if (address >= mappedDevice.start && address < mappedDevice.end)
		{
			m_last = &mappedDevice;
			return m_last;
		}
	}
	m_last = nullptr;
	return nullptr;
}
