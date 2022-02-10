#include <stdio.h>
#include <string.h>

#include <ff.h>
#include <diskio.h>

#include "Runtime/File.h"
#include "Runtime/HAL/SD.h"

// FatFs hooks

DSTATUS disk_initialize (BYTE pdrv)
{
	return 0;
}

DSTATUS disk_status (BYTE pdrv)
{
	return 0;
}

DRESULT disk_read (BYTE pdrv, BYTE* buff, LBA_t sector, UINT count)
{
	if (sd_read_block512(sector, buff, 512) == 512)
		return RES_OK;
	else
	{
		printf("SD block read failed!\n");
		return RES_ERROR;
	}
}

DRESULT disk_write (BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count)
{
	return RES_NOTRDY;
}

DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff)
{
	return RES_OK;
}

// HAL

FATFS fs;

FIL fps[32];
uint32_t fpa = 0;

static FIL* file_alloc()
{
	for (int32_t i = 0; i < 32; ++i)
	{
		if ((fpa & (1 << i)) == 0)
		{
			fpa |= (1 << i);
			memset(&fps[i], 0, sizeof(FIL));
			printf("allocated file %d\n", i);
			return &fps[i];
		}
	}
	return 0;
}

static void file_free(FIL* fp)
{
	int32_t index = fp - fps;
	printf("freed file %d\n", index);
	fpa &= ~(1 << index);
}

static int32_t file_index(FIL* fp)
{
	return (int32_t)(fp - fps) + 1;
}

static FIL* file_from_index(int32_t index)
{
	if (index >= 1 && index <= 32)
		return &fps[index - 1];
	else
		return 0;
}

int32_t file_init()
{
	int32_t result;
	if ((result = f_mount(&fs, "", 1)) != FR_OK)
	{
		printf("Unable to mount file system!, result = %d!\n", result);
		return 1;
	}
	return 0;
}

int32_t file_open(const char* name)
{
	FIL* fp = file_alloc();
	if (!fp)
		return 0;

	printf("file_open %s\n", name);
	if (f_open(fp, name, FA_READ) == FR_OK)
		return file_index(fp);
	else
	{
		printf("Unable to open file %s!\n", name);

		if (strcmp("./id1/pak0.pak", name) == 0)
		{
			printf("HACK, trying with file in root...\n");
			
			if (f_open(fp, "pak0.pak", FA_READ) == FR_OK)
				return 1;

			printf("failed also\n");
		}

		file_free(fp);
		return 0;
	}
}

void file_close(int32_t fd)
{
	printf("file_close\n");
	FIL* fp = file_from_index(fd);
	f_close(fp);
	file_free(fp);
}

int32_t file_size(int32_t fd)
{
	FIL* fp = file_from_index(fd);
	return f_size(fp);
}

int32_t file_seek(int32_t fd, int32_t offset, int32_t from)
{
	FRESULT result = FR_INVALID_PARAMETER;
	
	FIL* fp = file_from_index(fd);
	if (!fp)
	{
		printf("file_seek failed, no such file %d\n", fd);
		return 0;
	}

	printf("file_seek, fd: %d, offset: %d, from: %d\n", fd, offset, from);

	if (from == 0)
		result = f_lseek(fp, offset);
	else if (from == 1)
		result = f_lseek(fp, f_tell(fp) + offset);
	else if (from == 2)
		result = f_lseek(fp, f_size(fp) + offset);

	if (result != FR_OK)
	{
		printf("file_seek failed (%d, %d, %d), result = %d\n", fd, offset, from, result);
		return -1;
	}

	return f_tell(fp);
}

int32_t file_write(int32_t fd, const uint8_t* ptr, int32_t len)
{
	UINT bw;
	FIL* fp = file_from_index(fd);
	if (f_write(fp, ptr, len, &bw) == FR_OK)
		return (int32_t)bw;
	else
		return -1;
}

int32_t file_read(int32_t fd, uint8_t* ptr, int32_t len)
{
	UINT br;
	FIL* fp = file_from_index(fd);
	if (f_read(fp, ptr, len, &br) == FR_OK)
		return (int32_t)br;
	else
		return -1;
}
