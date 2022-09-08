#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "Runtime/File.h"
#include "Runtime/Kernel.h"
#include "Runtime/HAL/UART.h"

extern int _end;

static uint8_t* heap = (uint8_t*)&_end;

void* __attribute__((used)) _sbrk(int incr)
{
	kernel_enter_critical();
	uint8_t* prev_heap = heap;
	if (incr > 0)
		heap += (incr & ~3) + 4;
	kernel_leave_critical();
	return prev_heap;
}

int __attribute__((used)) _open(const char* name, int flags, int mode)
{
	int32_t fd = -1;

	if ((flags & O_ACCMODE) == O_RDONLY)
		fd = file_open(name, FILE_MODE_READ);
	else if ((flags & O_WRONLY) == O_WRONLY)
		fd = file_open(name, FILE_MODE_WRITE);

	return fd > 0 ? fd + 100 : -1;
}

int __attribute__((used)) _close(int file)
{
	if (file > 100)
	{
		int32_t fd = file - 100;
		file_close(fd);
	}
	return 0;
}

int __attribute__((used)) _fstat(int file, struct stat* st)
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
		st->st_blksize = 256;
		st->st_blocks = file_size(fd) / 256;
	}
	return 0;
}

int __attribute__((used)) _isatty(int file)
{
    switch (file)
	{
    case STDOUT_FILENO:
    case STDERR_FILENO:
    case STDIN_FILENO:
        return 1;
    default:
        return 0;
    }
}

int __attribute__((used)) _lseek(int file, int ptr, int dir)
{
	if (file > 100)
	{
		int32_t fd = file - 100;
		return file_seek(fd, ptr, dir);
	}
	else
		return 0;
}

void __attribute__((used)) _exit(int status)
{
	for(;;);
}

int __attribute__((used)) _kill(int pid, int sig)
{
	return -1;
}

int __attribute__((used)) _getpid(void)
{
	return 1;
}

int __attribute__((used)) _write(int file, char* ptr, int len)
{
	if (file > 100)
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

int __attribute__((used)) _read(int file, char* ptr, int len)
{
	if (file > 100)
	{
		int32_t fd = file - 100;

		int nbytes = 0;
		for (int o = 0; o < len; o += 256)
		{
			int nread = len - o;
			if (nread > 256)
				nread = 256;

			int nret = file_read(fd, ptr + o, nread);
			if (nret <= 0)
				break;

			nbytes += nret;
		}
		return nbytes;
	}
	else
	{
		for (int i = 0; i < len; ++i)
			*ptr++ = uart_rx_u8(0);
		return len;
	}
}

int __attribute__((used)) mkdir(const char* path, mode_t mode)
{
	return -1;
}

void crt_init()
{
	// do nothing; we need to reference this
	// compile unit regardless.
}
