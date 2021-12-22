#pragma once

#if defined(_WIN32)
#	include <windows.h>
#elif defined(__LINUX__)
#endif
#include <Core/Io/IStream.h>

class Serial : public traktor::IStream
{
	T_RTTI_CLASS;

public:
	enum class Parity
	{
		No,
		Odd,
		Even,
		Mark,
		Space
	};

	enum class DtrControl
	{
		Disable,
		Enable,
		Handshake
	};

	struct Configuration
	{
		uint32_t baudRate;
		uint8_t stopBits;
		Parity parity;
		uint8_t byteSize;
		DtrControl dtrControl;
	};

	bool open(int32_t port, const Configuration& configuration);

	virtual void close() override final;

	virtual bool canRead() const override final;

	virtual bool canWrite() const override final;

	virtual bool canSeek() const override final;

	virtual int64_t tell() const override final;

	virtual int64_t available() const override final;

	virtual int64_t seek(SeekOriginType origin, int64_t offset) override final;

	virtual int64_t read(void* block, int64_t nbytes) override final;

	virtual int64_t write(const void* block, int64_t nbytes) override final;

	virtual void flush() override final;

private:
#if defined(_WIN32)
	HANDLE m_hcomm = INVALID_HANDLE_VALUE;
#endif
};
