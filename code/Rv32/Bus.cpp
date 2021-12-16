#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include "Rv32/Bus.h"
#include "Rv32/Device.h"
#include "Rv32/Memory.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"Bus", Bus, Object)

void Bus::map(uint32_t start, uint32_t end, Device* device)
{
	m_mappedDevices.push_back({ start, end, device });
}

void Bus::writeU8(uint32_t address, uint8_t value)
{
	auto mappedDevice = findMappedDevice(address);
	if (mappedDevice)
	{
		if (!mappedDevice->device->writeU8(address - mappedDevice->start, value))
			m_error = true;
	}
	else
	{
		log::error << L"No device at 0x" << str(L"%08x", address) << L", trying to write 0x" << str(L"%02x", value) << L"." << Endl;
		m_error = true;
	}
}

void Bus::writeU16(uint32_t address, uint16_t value)
{
	auto mappedDevice = findMappedDevice(address);
	if (mappedDevice)
	{
		if (!mappedDevice->device->writeU16(address - mappedDevice->start, value))
			m_error = true;
	}
	else
	{
		log::error << L"No device at 0x" << str(L"%08x", address) << L", trying to write 0x" << str(L"%04x", value) << L"." << Endl;
		m_error = true;
	}
}

void Bus::writeU32(uint32_t address, uint32_t value)
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
}

uint8_t Bus::readU8(uint32_t address) const
{
	auto mappedDevice = findMappedDevice(address);
	if (mappedDevice)
		return mappedDevice->device->readU8(address - mappedDevice->start);
	else
	{
		log::error << L"No device at " << str(L"%08x", address) << L", trying to read U8." << Endl;
		m_error = true;
		return 0;
	}
}

uint16_t Bus::readU16(uint32_t address) const
{
	auto mappedDevice = findMappedDevice(address);
	if (mappedDevice)
		return mappedDevice->device->readU16(address - mappedDevice->start);
	else
	{
		log::error << L"No device at " << str(L"%08x", address) << L", trying to read U16." << Endl;
		m_error = true;
		return 0;
	}
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

const Bus::MappedDevice* Bus::findMappedDevice(uint32_t address) const
{
	for (auto& mappedDevice : m_mappedDevices)
	{
		if (address >= mappedDevice.start && address < mappedDevice.end)
			return &mappedDevice;
	}
	return nullptr;
}
