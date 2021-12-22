#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

extern int _end;

void *_sbrk(int incr) {
	static unsigned char *heap = NULL;
	unsigned char *prev_heap;
	if (heap == NULL) {
		heap = (unsigned char *)&_end;
	}
	prev_heap = heap;
	heap += incr;
	return prev_heap;
}

char* __file_ptr = 0;
const int32_t __file_size = 23;

int _open(const char* name, int flags, int mode) {

	if (__file_ptr != 0)
		for(;;);

	__file_ptr = (char*)0x00700000;
	return 4;
}

int _close(int file) {
	__file_ptr = (char*)0;
	return 0;
}

int _fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;

	if (__file_ptr != 0)
		st->st_size = __file_size;

	return 0;
}

int _isatty(int file) {
	return 1;
}

int _lseek(int file, int ptr, int dir) {
	if (__file_ptr == 0)
		return -1;

	if (dir == 0)
		__file_ptr = (char*)0x00700000 + ptr;
	else if (dir == 1)
		__file_ptr += ptr;
	else if (dir == 2)
		__file_ptr = (char*)0x00700000 + __file_size + ptr;

	return (int)(__file_ptr - (char*)0x00700000);
}

void _exit(int status) {
	for(;;);
}

void _kill(int pid, int sig) {
	return;
}

int _getpid(void) {
	return -1;
}

int _write(int file, char * ptr, int len) {
	for (int i = 0; i < len; ++i) {
		*(char*)0x50000010 = *ptr++;
	}
	return len;
}

int _read(int file, char * ptr, int len) {
	if (__file_ptr == 0)
		return -1;

	int avail = (int)((char*)0x00700000 + __file_size - __file_ptr);
	if (len > avail)
		len = avail;

	for (int i = 0; i < len; ++i)
		*ptr++ = *__file_ptr++;

	return len;
}

/*
int mkdir(const char *path, mode_t mode) {
	return -1;
}
*/