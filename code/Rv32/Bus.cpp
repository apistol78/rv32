#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/Bus.h"
#include "Rv32/Memory.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"Bus", Bus, Object)

void Bus::map(uint32_t start, uint32_t end, bool cacheable, Device* device)
{
	m_mappedDevices.push_back({ start, end, cacheable, device });
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
		log::error << L"No device at " << str(L"%08x", address) << L", trying to read U32." << Endl;
		m_error = true;
		return 0;
	}
}

bool Bus::tick(CPU* cpu)
{
	for (auto& mappedDevice : m_mappedDevices)
	{
		if (!mappedDevice.device->tick(cpu))
			return false;
	}
	return true;
}

const Bus::MappedDevice* Bus::findMappedDevice(uint32_t address) const
{
	for (auto& mappedDevice : m_mappedDevices)
	{
		if (address >= mappedDevice.start && address < mappedDevice.end)
			return &mappedDevice;
	}
	return nullptr;
}
