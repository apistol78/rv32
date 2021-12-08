#include <stdio.h>
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

int _close(int file) {
	for(;;);
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
	for(;;);
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


void main()
{
	const char* test = "Foo\n";
	printf("Hello world %d!\n", 123);
	for(;;);
}