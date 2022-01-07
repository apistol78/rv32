#include <Core/Log/Log.h>
#include <Core/Misc/String.h>
#include <Core/Misc/TString.h>
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
	m_fd = ::open(wstombs(str(L"/dev/ttyUSB%d", port)).c_str(), O_RDWR);
	if (m_fd < 0)
	{
		log::error << L"open failed" << Endl;
		return false;
	}

	struct termios tty;
	if (tcgetattr(m_fd, &tty) != 0)
	{
		log::error << L"tcgetattr failed" << Endl;
		return false;
	}

	tty.c_cflag &= ~PARENB;	// no parity
	tty.c_cflag &= ~CSTOPB;	// one stop bit
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS5 | CS6 | CS7 | CS8;	// 8 bits per byte
	tty.c_cflag &= ~CRTSCTS;	// no rts/cts control
	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo
	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
	// tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT IN LINUX)
	// tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT IN LINUX)
	tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 0;

	if (configuration.baudRate == 9600)
	{
		cfsetispeed(&tty, B9600);
		cfsetospeed(&tty, B9600);
	}
	else if (configuration.baudRate == 115200)
	{
		cfsetispeed(&tty, B115200);
		cfsetospeed(&tty, B115200);
	}
	else
	{
		log::error << L"Unsupported baud rate." << Endl;
		return false;
	}

	if (tcsetattr(m_fd, TCSANOW, &tty) != 0)
	{
		log::error << L"tcsetattr failed" << Endl;
		return false;
	}

	log::info << L"Serial opened succcessfully." << Endl;
	return true;
}

void Serial::close()
{
	::close(m_fd);
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
	return ::read(m_fd, block, nbytes);
}

int64_t Serial::write(const void* block, int64_t nbytes)
{
	return ::write(m_fd, block, nbytes);
}

void Serial::flush()
{
}

#endif
