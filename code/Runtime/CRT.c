#include <sys/stat.h>
#include <unistd.h>
#include "Runtime/HAL/Common.h"
#include "Runtime/HAL/File.h"
#include "Runtime/HAL/UART.h"

extern int _end;

void *_sbrk(int incr)
{
	static uint8_t* heap = 0;
	uint8_t* prev_heap;
	if (heap == 0)
		heap = (uint8_t*)&_end;
	prev_heap = heap;
	heap += incr;
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
	// st->st_mode = S_IFCHR;
	// if (__file_ptr != 0)
	// 	st->st_size = __file_size;
	return 0;
}

int _isatty(int file)
{
	return file <= 100 ? 1 : 0;
}

int _lseek(int file, int ptr, int dir)
{
	return file_seek(file, ptr, dir);
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
			uart_tx_u8(*ptr++);
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
			*ptr++ = uart_rx_u8();
		return len;
	}
}

int mkdir(const char* path, mode_t mode)
{
	return -1;
}
