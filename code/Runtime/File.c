#include <stdio.h>

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
FIL fp;

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
	if (f_open(&fp, name, FA_READ) == FR_OK)
		return 1;
	else
	{
		printf("Unable to open file %s!\n", name);
		return 0;
	}
}

void file_close(int32_t fd)
{
	f_close(&fp);
}

int32_t file_size(int32_t fd)
{
	return f_size(&fp);
}

int32_t file_seek(int32_t file, int32_t offset, int32_t from)
{
	FRESULT result = FR_INVALID_PARAMETER;
	
	if (from == 0)
		result = f_lseek(&fp, offset);
	else if (from == 1)
		result = f_lseek(&fp, f_tell(&fp) + offset);
	else if (from == 2)
		result = f_lseek(&fp, f_size(&fp) + offset);

	if (result != FR_OK)
	{
		printf("file_seek failed, result = %d\n", result);
		return -1;
	}

	return f_tell(&fp);
}

int32_t file_write(int32_t file, const uint8_t* ptr, int32_t len)
{
	UINT bw;
	if (f_write(&fp, ptr, len, &bw) == FR_OK)
		return (int32_t)bw;
	else
		return -1;
}

int32_t file_read(int32_t file, uint8_t* ptr, int32_t len)
{
	UINT br;
	if (f_read(&fp, ptr, len, &br) == FR_OK)
		return (int32_t)br;
	else
		return -1;
}
