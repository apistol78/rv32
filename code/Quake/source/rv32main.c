#include <stdio.h>
#include <stdint.h>

#ifndef _PC
#include "Runtime/Runtime.h"
#include "Runtime/HAL/Timer.h"
#endif

void quake_main(int argc,char **argv);

uint64_t get_time(void)
{
#ifndef _PC
	return timer_get_ms() * 1000ULL;
#else
	return 30 * 1000ULL;
#endif
}

int main(void)
{
#ifndef _PC
	runtime_init();
#endif
	quake_main(0,0);
	while(1);
}

//#ifdef _PC

FILE* myfopen(const char *filename, const char *atr)
{
	//printf("### fopen %s\n", filename);
	return fopen(filename, atr);
}

size_t myfread(void *ptr, size_t s, size_t nm, FILE *f)
{
	//printf("### fread %d, %d\n", s, nm);
	return fread(ptr, s, nm, f);
}

int myfclose(FILE *f)
{
	return fclose(f);
}

int myfseek(FILE *f, long ofs, int whence)
{
	//printf("### fseek %d, %d\n", ofs, whence);
	return fseek(f, ofs, whence);
}

long myftell(FILE *f)
{
	//printf("### ftell\n");
	return ftell(f);
}

int mygetc(FILE *f)
{
	//printf("### getc\n");
	return getc(f);
}

//#endif

