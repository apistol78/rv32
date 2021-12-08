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

int _open(const char* name, int flags, int mode) {
	printf("_open %s\n", name);
	return 4;
}

int _close(int file) {
	printf("_close %d\n", file);
	return 0;
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
	return;
}

int _getpid(void) {
	return -1;
}

int _write(int file, char * ptr, int len) {
	for (int i = 0; i < len; ++i) {
		*(char*)0x00800000 = *ptr++;
	}
	return len;
}

int _read(int file, char* ptr, int len) {
	printf("_read %d, %d bytes\n", file, len);
	for (int i = 0; i < len; ++i)
		*ptr++ = 'A';

	return len;
}



void main()
{
	unsigned char v = 'E';

	FILE* fp = fopen("test.wad", "rb");
	if (fp)
	{
		fread(&v, 1, 1, fp);
		fclose(fp);
	}
	else
		printf("failed to open file\n");

	*(char*)0x00800000 = v;
	*(char*)0x00800000 = '\n';

	for(;;);
}