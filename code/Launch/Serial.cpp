#include <Core/Misc/String.h>
#include "Serial.h"

using namespace traktor;

T_IMPLEMENT_RTTI_CLASS(L"Serial", Serial, IStream)

#if defined(_WIN32)

bool Serial::open(int32_t port, const Configuration& configuration)
{
	if (m_hcomm != INVALID_HANDLE_VALUE)
		return false;

	m_hcomm = CreateFile(
		str(L"\\\\.\\COM%d", port).c_str(),
		GENERIC_READ | GENERIC_WRITE, 
		0, 
		0, 
		OPEN_EXISTING,
		0,
		0
	);
	if (m_hcomm == INVALID_HANDLE_VALUE)
		return false;

	DCB dcb;
	GetCommState(m_hcomm, &dcb);
	dcb.BaudRate = configuration.baudRate;
	dcb.StopBits = ONESTOPBIT;
	dcb.Parity = NOPARITY;
	dcb.ByteSize = configuration.byteSize;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	SetCommState(m_hcomm, &dcb);

	switch (configuration.stopBits)
	{
	case 1:
		dcb.StopBits = ONESTOPBIT;
		break;
	case 2:
		dcb.StopBits = TWOSTOPBITS;
		break;
	default:
		return false;
	}

	switch (configuration.parity)
	{
	case Parity::No:
		dcb.Parity = NOPARITY;
		break;
	case Parity::Odd:
		dcb.Parity = ODDPARITY;
		break;
	case Parity::Even:
		dcb.Parity = EVENPARITY;
		break;
	case Parity::Mark:
		dcb.Parity = MARKPARITY;
		break;
	case Parity::Space:
		dcb.Parity = SPACEPARITY;
		break;
	default:
		return false;
	}

	//PurgeComm(m_hcomm, PURGE_RXCLEAR | PURGE_TXCLEAR);

	COMMTIMEOUTS cto;                   
	cto.ReadIntervalTimeout = 0;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.ReadTotalTimeoutConstant = 0;
	cto.WriteTotalTimeoutMultiplier = 0;
	cto.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(m_hcomm, &cto);

	return true;
}

void Serial::close()
{
	if (m_hcomm != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hcomm);
		m_hcomm = INVALID_HANDLE_VALUE;
	}
}

bool Serial::canRead() const
{
	return true;
}

bool Serial::canWrite() const
{
	return true;
}

bool Serial::canSeek() const
{
	return false;
}

int64_t Serial::tell() const
{
	return 0;
}

int64_t Serial::available() const
{
	COMSTAT cs = { 0 };
	if (ClearCommError(m_hcomm, nullptr, &cs) == FALSE)
		return -1;
	return (int64_t)cs.cbInQue;
}

int64_t Serial::seek(SeekOriginType origin, int64_t offset)
{
	return 0;
}

int64_t Serial::read(void* block, int64_t nbytes)
{
	DWORD nread;
	if (ReadFile(m_hcomm, block, (DWORD)nbytes, (LPDWORD)&nread, nullptr) == FALSE)
		return -1;

	return (int64_t)nread;
}

int64_t Serial::write(const void* block, int64_t nbytes)
{
	DWORD nwritten;
	if (WriteFile(m_hcomm, block, (DWORD)nbytes, &nwritten, nullptr) == FALSE)
		return -1;

	return (int64_t)nwritten;
}

void Serial::flush()
{
	FlushFileBuffers(m_hcomm);
}

#elif defined(__LINUX__)

bool Serial::open(int32_t port, const Configuration& configuration)
{
	return true;
}

void Serial::close()
{
}

bool Serial::canRead() const
{
	return true;
}

bool Serial::canWrite() const
{
	return true;
}

bool Serial::canSeek() const
{
	return false;
}

int64_t Serial::tell() const
{
	return 0;
}

int64_t Serial::available() const
{
	return 0;
}

int64_t Serial::seek(SeekOriginType origin, int64_t offset)
{
	return 0;
}

int64_t Serial::read(void* block, int64_t nbytes)
{
	return -1;
}

int64_t Serial::write(const void* block, int64_t nbytes)
{
	return -1;
}

void Serial::flush()
{
}

#endif
