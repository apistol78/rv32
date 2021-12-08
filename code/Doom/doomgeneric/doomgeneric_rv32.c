#include <sys/stat.h>
#include "doomgeneric.h"

// doomgeneric stubs

void DG_Init() {
}

void DG_DrawFrame() {
}

void DG_SleepMs(uint32_t ms) {
}

uint32_t DG_GetTicksMs() {
		return 0;
}

int DG_GetKey(int* pressed, unsigned char* doomKey) {
		return 0;
}

void DG_SetWindowTitle(const char * title) {
}

int mkdir(const char *path, mode_t mode) {
		return -1;
}

// crt stubs

extern int _end;

void *_sbrk(int incr) {
	static unsigned char *heap = NULL;
	unsigned char *prev_heap;
	if (heap == NULL) {
		heap = (unsigned char *)&_end;
	}
	prev_heap = heap;
	heap += incr;
	if (heap >= (unsigned char*)0x00800000) {
		for(;;);
	}
	return prev_heap;
}

int _close(int file) {
	return -1;
}

int _fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int file) {
	return 1;
}

int _lseek(int file, int ptr, int dir) {
	return 0;
}

void _exit(int status) {
	for(;;);
}

void _kill(int pid, int sig) {
	for(;;);
	return;
}

int _getpid(void) {
	return -1;
}

int _write (int file, char * ptr, int len) {
	for (int i = 0; i < len; ++i) {
		*(char*)0x00800000 = *ptr++;
	}
	return len;
}

int _read (int file, char * ptr, int len) {
	return -1;
}
