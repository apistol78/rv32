#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "Runtime/File.h"
#include "Runtime/HAL/UART.h"

extern int _end;

static uint8_t* heap = (uint8_t*)&_end;

void* _sbrk(int incr)
{
	uint8_t* prev_heap = heap;
	if (incr > 0)
		heap += (incr & ~3) + 4;
	return prev_heap;
}

int _open(const char* name, int flags, int mode)
{
	int32_t fd = file_open(name);
	return fd > 0 ? fd + 100 : -1;
}

int _close(int file)
{
	int32_t fd = file - 100;
	file_close(fd);
	return 0;
}

int _fstat(int file, struct stat* st)
{
	memset(st, 0, sizeof(struct stat));
	if (file <= 100)
	{
		st->st_dev = 0;
		st->st_mode = S_IFCHR;
		st->st_rdev = 0;
	}
	else
	{
		int32_t fd = file - 100;
		st->st_mode = S_IFBLK;
		st->st_size = file_size(fd);
	}
	return 0;
}

int _isatty(int file)
{
	return file <= 100 ? 1 : 0;
}

int _lseek(int file, int ptr, int dir)
{
	if (file >= 100)
	{
		int32_t fd = file - 100;
		return file_seek(fd, ptr, dir);
	}
	else
		return 0;
}

void _exit(int status)
{
	for(;;);
}

void _kill(int pid, int sig)
{
	return;
}

int _getpid(void)
{
	return -1;
}

int _write(int file, char* ptr, int len)
{
	if (file >= 100)
	{
		int32_t fd = file - 100;
		return file_write(fd, ptr, len);
	}
	else
	{
		for (int i = 0; i < len; ++i)
			uart_tx_u8(0, *ptr++);
		return len;
	}
}

int _read(int file, char* ptr, int len)
{
	if (file >= 100)
	{
		int32_t fd = file - 100;
		return file_read(fd, ptr, len);
	}
	else
	{
		for (int i = 0; i < len; ++i)
			*ptr++ = uart_rx_u8(0);
		return len;
	}
}

int mkdir(const char* path, mode_t mode)
{
	return -1;
}

void __attribute__((__used__)) crt_init()
{
	_sbrk(0);
}
